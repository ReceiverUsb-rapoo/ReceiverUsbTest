#include "boxserver.h"
#include "QDebug"

BoxServer::BoxServer(QTcpServer *parent)
    :QTcpServer(parent)
{
    m_strReceiveData = "";
    m_pQTcpSocket = NULL;
    m_pQThread = NULL;
    this->setMaxPendingConnections(5);

    m_pQThread = new QThread;
    this->moveToThread(m_pQThread);
    m_pQThread->start();
}

BoxServer::~BoxServer()
{
    if(m_pQThread != NULL){
        m_pQThread->quit();
        m_pQThread->wait();
        m_pQThread->deleteLater();
    }

    if(DisConnect()){
        m_pQTcpSocket = NULL;
    }

}

bool BoxServer::Init(const quint16 &us_Port, const QString &str_HostIP)
{
    m_usPort = us_Port;

    if(str_HostIP == "Any"){
        if(!this->listen(QHostAddress::Any, m_usPort)){
            emit sig_Msg("Connect Faile" + str_HostIP);
            return false;
        }
    }
    else{
        m_strIP = str_HostIP;
        if(!this->listen(QHostAddress(m_strIP), m_usPort)){
            emit sig_Msg("Connect Faile" + str_HostIP);
            return false;
        }
    }

    connect(this, SIGNAL(newConnection()), this, SLOT(slot_NewConnect()));
    return true;
}

bool BoxServer::SendData(const QString &str_Data, int n_TimeOut)
{
    if(m_pQTcpSocket == NULL){
        return false;
    }

    if(m_pQTcpSocket->state() == 3){
        if(m_pQTcpSocket->write(str_Data.toLatin1()) != 1){
            if(m_pQTcpSocket->waitForBytesWritten(n_TimeOut)){
                return true;
            }
            return SendData(str_Data);
        }
    }
    else{
        if(this->waitForNewConnection(1000)){
            return SendData(str_Data);
        }
        else{
            return false;
        }
    }
    return true;
}

bool BoxServer::DisConnect()
{
    if(m_pQTcpSocket != NULL){
        if(m_pQTcpSocket->state() == 3){
            m_pQTcpSocket->disconnectFromHost();
            m_pQTcpSocket->waitForDisconnected();
        }
        m_pQTcpSocket->close();
    }
    return true;
}

void BoxServer::incomingConnection(qintptr socketDescriptor)
{
    m_pQTcpSocket = new QTcpSocket;
    m_pQTcpSocket->setSocketDescriptor(socketDescriptor);

//    QString str_IP = m_pQTcpSocket->peerAddress().toString();
//    qint16 us_Port = m_pQTcpSocket->peerPort();

    connect(m_pQTcpSocket, SIGNAL(readyRead()), this, SLOT(slot_ReceiveData));
    connect(m_pQTcpSocket, SIGNAL(disconnected()), this, SLOT(slot_RemoveSocket()));

    m_pQTcpSocket->moveToThread(this->thread());
}

void BoxServer::slot_NewConnect()
{
    if(m_pQTcpSocket->isOpen()){
        emit sig_Msg(m_pQTcpSocket->localAddress().toString() +
                     "New Connect......, Server" + m_strIP);
    }
}

void BoxServer::slot_ReceiveData()
{
    QByteArray byte_Data = m_pQTcpSocket->readAll();
    m_strReceiveData = QString::fromLocal8Bit(byte_Data);

    emit sig_GetData(m_strReceiveData);
}

void BoxServer::slot_RemoveSocket()
{
    if(m_pQTcpSocket->isOpen()){
        m_pQTcpSocket->close();
    }
}
