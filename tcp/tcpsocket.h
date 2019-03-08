#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>

struct STRUCT_TCPDATA
{
    QString str_IP;
    ushort us_Port;
    QByteArray byte_Data;

    STRUCT_TCPDATA(){
        str_IP = "";
        us_Port = 0;
        byte_Data = "";
    }
};

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr p_SocketDescriptor, QObject *parent = 0);
    ~TcpSocket();

private:
    //用来处理数据的函数
    QByteArray HandleData(QByteArray byte_Data,
                          const QString & str_Ip,
                          qint16 s_Port);

signals:
    //收到tcp数据
    void sig_ReadData(STRUCT_TCPDATA);
    //写入数据 数据 + ip
    void sig_Write(const QByteArray,const QString);
    //NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    void sig_SockDisConnect(const int,
                            const QString &,
                            const quint16,
                            QThread *);
public slots:
    //发送信号的槽
    void slot_SentData(const QByteArray byte_Data,
                       const QString str_Ip);
    //断开指定连接                   
    void slot_DisConTcp(int n_ID);

protected slots:
    //接收数据
    void slot_ReadData();
    //处理下一个
    void slot_StartNext();
protected:
    QFutureWatcher<QByteArray> m_FutureWatcher;
    QQueue<QByteArray> m_QueueDatas;
private:
    qintptr m_SocketID;
    QMetaObject::Connection m_Dis;
};

#endif // TCPSOCKET_H
