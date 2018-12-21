#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QHash>
#include "tcpsocket.h"

//继承QTCPSERVER以实现多线程TCPscoket的服务器。
//如果socket的信息处理直接处理的话，很多新建的信号和槽是用不到的

class TcpServer : public QTcpServer
{
    Q_OBJECT
    friend class TcpServerInstanceGetter;
private:
    explicit TcpServer(QObject *parent = 0,int numConnections = 10000);

public:
    ~TcpServer();

    //重写设置最大连接数函数
    void SetMaxPendingConnections(int n_NumConnections);
    //开始监听port
    bool StartListen(quint16 us_Port,
                     const QString &str_HostIP = "Any");
    //停止监听port
    void CloseListen();
    //关闭服务器
    void CloseServer();

signals:
    //向scoket发送消息, 数据， ip
    void sig_SentData(QByteArray byte_Data,
                      QString str_IP);
    //server接受到的数据
    void sig_DataRecieved(STRUCT_TCPDATA struct_TcpData);

    //发送新用户连接信息
    void sig_ConnectClient(int n_ID,
                           QString str_IP,
                           quint16 us_Port);
    //断开连接的用户信息
    void sig_SockDisConnect(int n_ID,
                            QString str_IP,
                            quint16 us_Port);
    //断开特定连接，并释放资源，-1为断开所有。
    void sig_SentDisConnect(int n_ID);

public slots:
    //断开所有连接，线程计数器请0
    void slot_Clear();

private slots:
    void slot_ReadData(STRUCT_TCPDATA struct_TcpData);

    void slot_SlotTime();

protected slots:
    //断开连接的用户信息
    void slot_SockDisConnectSlot(int n_Handle,
                                 const QString & str_Ip,
                                 quint16 us_Prot,
                                 QThread *p_QThread);

protected:
    //覆盖已获取多线程
    void incomingConnection(qintptr socketDescriptor);
private:
    //管理连接的map
    QHash<int,TcpSocket *> * m_pHashTcpClient;
    int m_nMaxConnections;
    QThread * m_pThread;
    //端口绑定 tcpSocket的socketDescriptor
    QHash<QString,qintptr> m_HashCom;
};

#endif // TCPSERVER_H
