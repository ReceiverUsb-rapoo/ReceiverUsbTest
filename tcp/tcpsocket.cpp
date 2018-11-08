#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

//构造函数在主线程执行，lambda在子线程
TcpSocket::TcpSocket(qintptr p_SocketDescriptor, QObject *parent) :
    QTcpSocket(parent),m_SocketID(p_SocketDescriptor)
{
    this->setSocketDescriptor(p_SocketDescriptor);
    //    setReadBufferSize(4096*4);

    connect(this,&TcpSocket::readyRead,
            this,&TcpSocket::slot_ReadData);
    connect(this,&TcpSocket::sig_Write,
            this,&TcpSocket::slot_SentData);

    m_Dis = connect(this,&TcpSocket::disconnected,
                  [&](){
        qDebug() << "disconnect" ;       
        //发送断开连接的用户信息
        emit sig_SockDisConnect(m_SocketID,
                                this->peerAddress().toString(),
                                this->peerPort(),
                                QThread::currentThread());
        this->deleteLater();
    });

    connect(&m_FutureWatcher,&QFutureWatcher<QByteArray>::finished,
            this,&TcpSocket::slot_StartNext);
    connect(&m_FutureWatcher,&QFutureWatcher<QByteArray>::canceled,
            this,&TcpSocket::slot_StartNext);

    qDebug() << "new connect"<<(this->peerAddress().toString());
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::slot_SentData(const QByteArray byte_Data,
                              const QString str_Ip)
{
    if(peerAddress().toString() == str_Ip){
        qDebug()<<"slot sent data to write"<<byte_Data;
        write(byte_Data);
    }

    qDebug()<<"slotSentData"<<str_Ip<<peerAddress().toString();
}

void TcpSocket::slot_DisConTcp(int n_ID)
{
    //-1为全部断开
    if (n_ID == m_SocketID){
        this->disconnectFromHost();
    }
    else if (n_ID == -1){
        //先断开连接的信号槽，防止二次析构
        disconnect(m_Dis);
        this->disconnectFromHost();
        this->deleteLater();
    }
}


void TcpSocket::slot_ReadData()
{
    QByteArray byte_BagData;
    while(bytesAvailable()) {
        byte_BagData.append(readAll());
    }

    STRUCT_TCPDATA struct_TcpData;
    struct_TcpData.byte_Data = byte_BagData;
    struct_TcpData.str_IP = peerAddress().toString();
    struct_TcpData.us_Port = peerPort();
    qDebug()<<"slot Read DATA"<<struct_TcpData.str_IP<<struct_TcpData.byte_Data;

    emit sig_ReadData(struct_TcpData);
}

QByteArray TcpSocket::HandleData(QByteArray byte_Data,
                                 const QString &str_Ip,
                                 qint16 s_Port)
{
    QTime o_QTime;
    o_QTime.start();

    QElapsedTimer o_QElapsedTimer;
    o_QElapsedTimer.start();
    while(o_QElapsedTimer.elapsed() < 100){

    }

    byte_Data = str_Ip.toUtf8() +
            " " +
            QByteArray::number(s_Port) + " " +
            byte_Data + " " +
            QTime::currentTime().toString().toUtf8();

    return byte_Data;
}

void TcpSocket::slot_StartNext()
{
    this->write(m_FutureWatcher.future().result());
    if (!m_QueueDatas.isEmpty()){
        m_FutureWatcher.setFuture(QtConcurrent::run(this,
                                                    &TcpSocket::HandleData,
                                                    m_QueueDatas.dequeue(),
                                                    this->peerAddress().toString(),
                                                    this->peerPort()));
    }
}
