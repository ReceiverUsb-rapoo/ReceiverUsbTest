#include "tcpserver.h"
#include "threadhandle.h"
#include <QTimer>

TcpServer::TcpServer(QObject *parent,int numConnections) :
    QTcpServer(parent)
{
    m_pHashTcpClient = NULL;
    m_pThread = NULL;
    m_pHashTcpClient = new QHash<int, TcpSocket *>;
    SetMaxPendingConnections(numConnections);

//    m_pThread = new QThread;
//    connect(this,&TcpServer::destroyed,
//            m_pThread,&QThread::deleteLater);
//    moveToThread(m_pThread);
//    m_pThread->start();

//    QTimer *time = new QTimer;
//    connect(time,&QTimer::timeout,
//            this,&TcpServer::slotTime);
//    connect(this,&TcpServer::destroyed,
//            time,&QTimer::deleteLater);
//    time->start(100);
}

TcpServer::~TcpServer()
{
    CloseListen();
    CloseServer();
}

void TcpServer::SetMaxPendingConnections(int n_NumConnections)
{
    //调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->QTcpServer::setMaxPendingConnections(n_NumConnections);
    this->m_nMaxConnections = n_NumConnections;
}

bool TcpServer::StartListen(quint16 us_Port, const QString &str_HostIP)
{
    if(str_HostIP == "Any"){
        return listen(QHostAddress::AnyIPv4, us_Port);
    }

    qDebug()<<"StartListen str_HostIP"<<str_HostIP;
    qDebug()<<"StartListen us_Port"<<us_Port;


    return listen(QHostAddress(str_HostIP), us_Port);
}

void TcpServer::CloseListen()
{
    if(this->isListening()){
        this->close();
    }
}

void TcpServer::CloseServer()
{
    if(m_pThread != NULL) {
        //暂时存放，后续会改
        slot_Clear();
        m_pThread->quit();
        m_pThread->wait();
        m_pThread->deleteLater();
        m_pThread = NULL;
    }

    if(m_pHashTcpClient != NULL){
        delete m_pHashTcpClient;
        m_pHashTcpClient = NULL;
    }
}

//多线程必须在此函数里捕获新连接
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    //继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    if (m_pHashTcpClient->size() > maxPendingConnections()){
        QTcpSocket o_QTcpSocket;
        o_QTcpSocket.setSocketDescriptor(socketDescriptor);
        o_QTcpSocket.disconnectFromHost();
        return;
    }

    QThread *p_QThread = ThreadHandle::getClass().GetThread();
    TcpSocket *p_TcpSocket = new TcpSocket(socketDescriptor);

    QString str_Ip = p_TcpSocket->peerAddress().toString();
    qint16 us_Port = p_TcpSocket->peerPort();
    qDebug()<<"new ip port:"<<str_Ip<<us_Port<<p_TcpSocket->peerName();
    m_HashCom.insert(str_Ip,socketDescriptor);

    //NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(p_TcpSocket, &TcpSocket::sig_SockDisConnect,
            this, &TcpServer::slot_SockDisConnectSlot);
    //断开信号
    connect(this, &TcpServer::sig_SentDisConnect,
            p_TcpSocket, &TcpSocket::slot_DisConTcp);
    connect(p_TcpSocket, &TcpSocket::sig_ReadData,
            this, &TcpServer::slot_ReadData);
    connect(this, &TcpServer::sig_SentData,
            p_TcpSocket, &TcpSocket::sig_Write);

    //把tcp类移动到新的线程，从线程管理类中获取
    p_TcpSocket->moveToThread(p_QThread);
    //插入到连接信息中
    m_pHashTcpClient->insert(socketDescriptor,p_TcpSocket);

    emit sig_ConnectClient(socketDescriptor,str_Ip,us_Port);
}

void TcpServer::slot_SockDisConnectSlot(int n_Handle,
                                        const QString & str_Ip,
                                        quint16 us_Prot,
                                        QThread * p_QThread)
{
    //连接管理中移除断开连接的socket
    m_pHashTcpClient->remove(n_Handle);
    //告诉线程管理类那个线程里的连接断开了
    ThreadHandle::getClass().RemoveThread(p_QThread);
    emit sig_SockDisConnect(n_Handle,str_Ip,us_Prot);
}


void TcpServer::slot_Clear()
{
    emit this->sig_SentDisConnect(-1);
    ThreadHandle::getClass().Clear();
    m_pHashTcpClient->clear();
}

void TcpServer::slot_ReadData(STRUCT_TCPDATA struct_TcpData)
{
//    qDebug()<<"ReadData  "<<struct_TcpData.str_IP<<"  "<<QString(struct_TcpData.byte_Data);

    emit sig_DataRecieved(struct_TcpData);
}

void TcpServer::slot_SlotTime()
{
    foreach (int n_ID, m_pHashTcpClient->keys()) {
        QByteArray byte_Data;

        byte_Data.append(QString::number(m_pHashTcpClient->value(n_ID)->peerPort()));
        emit sig_SentData(byte_Data,m_HashCom.key(n_ID));
    }
}
