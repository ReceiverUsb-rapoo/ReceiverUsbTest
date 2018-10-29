#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);

    //    setReadBufferSize(4096*4);

    connect(this,&TcpSocket::readyRead,this,&TcpSocket::slotReadData);
    connect(this,&TcpSocket::sigWrite,this,&TcpSocket::slotSentData);

    dis = connect(this,&TcpSocket::disconnected,
                  [&](){
        qDebug() << "disconnect" ;
        emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
        this->deleteLater();
    });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect"<<(this->peerAddress().toString());

    //包处理
//    m_BagDataHandle = new BagDataHandle;
//    connect(m_BagDataHandle, SIGNAL(sig_ReadJsonData(QByteArray)), this, SLOT(slot_HandleBagData(QByteArray)));
}

TcpSocket::~TcpSocket()
{

}


void TcpSocket::slotSentData(const QByteArray data,const QString ip)
{
    qDebug()<<"slotSentData"<<ip<<peerAddress().toString();
//    QByteArray sendData = m_BagDataHandle->WriteBag(data);

    if(peerAddress().toString() == ip)
    {
//        qDebug()<<"slot sent data to write"<<sendData;
///        write(sendData);
    }
}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::slot_HandleBagData(QByteArray BagData)
{
////    STRUCT_MSGDATA d;
//    d.byte_Data = BagData;
//    d.str_Ip = peerAddress().toString();
//    d.n_Port = peerPort();
////    qDebug()<<"slot Read DATA"<<d.ip<<d.data;
//    emit sigReadData(d);
}

void TcpSocket::slotReadData()
{
    QByteArray temp;
    while (bytesAvailable()) {
        temp.append(readAll());
    }

//    m_BagDataHandle->ReadBag(temp);
}

QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
        QTime time;
        time.start();

        QElapsedTimer tm;
        tm.start();
        while(tm.elapsed() < 100)
        {}
        data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}
