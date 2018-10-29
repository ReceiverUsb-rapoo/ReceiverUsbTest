#include "tcpserver.h"
#include "threadhandle.h"
#include <QTimer>


TcpServer::TcpServer(QObject *parent,int numConnections) :
    QTcpServer(parent)
{
    tcpClient = NULL;
    pThread = NULL;
    tcpClient = new  QHash<int, TcpSocket *>;
    setMaxPendingConnections(numConnections);

    pThread = new QThread;
    connect(this,&TcpServer::destroyed,
            pThread,&QThread::deleteLater);
    moveToThread(pThread);
    pThread->start();

//    QTimer *time = new QTimer;
//    connect(time,&QTimer::timeout,
//            this,&TcpServer::slotTime);
//    connect(this,&TcpServer::destroyed,
//            time,&QTimer::deleteLater);
//    time->start(100);
}

TcpServer::~TcpServer()
{
    closeServer();
}

void TcpServer::setMaxPendingConnections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections);//调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->maxConnections = numConnections;
}

void TcpServer::StartListen(quint16 iPort, const QString &strHostIP)
{

    if(strHostIP == "Any")
    {
        listen(QHostAddress::AnyIPv4, iPort);
    }
    else
    {
        listen(QHostAddress(strHostIP), iPort);
    }
    qDebug()<<"start listen"<<serverPort()<<isListening();
}

void TcpServer::closeServer()
{

    if(pThread != NULL)
    {
        clear();//暂时存放，后续会改


        pThread->quit();
        pThread->wait();
        pThread->deleteLater();
        pThread = NULL;
    }

    if(tcpClient != NULL)
    {
        delete tcpClient;
        tcpClient = NULL;
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    if (tcpClient->size() > maxPendingConnections())//继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    auto th = ThreadHandle::getClass().getThread();
    auto tcpTemp = new TcpSocket(socketDescriptor);
    QString ip = tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();
    qDebug()<<"new ip port:"<<ip<<port<<tcpTemp->peerName();
    m_comHash.insert(ip,socketDescriptor);

    connect(tcpTemp,&TcpSocket::sockDisConnect,this,&TcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(this,&TcpServer::sentDisConnect,tcpTemp,&TcpSocket::disConTcp);//断开信号

 //   connect(tcpTemp,&TcpSocket::sigReadData,this,&TcpServer::slotReadData);
    connect(this,&TcpServer::sigSentData,tcpTemp,&TcpSocket::sigWrite);


    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中


    emit connectClient(socketDescriptor,ip,port);
}

void TcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    emit sockDisConnect(handle,ip,prot);
}


void TcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
}

//void TcpServer::slotReadData(STRUCT_MSGDATA d)
//{
////    qDebug()<<"slotReadData sigDataRecieved";
//    emit sigDataRecieved(d);
//}

void TcpServer::slotTime()
{
    foreach (int id, tcpClient->keys()) {
        QByteArray d;

        d.append(QString::number(tcpClient->value(id)->peerPort()));
        emit sigSentData(d,m_comHash.key(id));
    }
}
