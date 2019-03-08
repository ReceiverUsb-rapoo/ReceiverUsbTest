#ifndef SUPPLEMENTROOBOT_H
#define SUPPLEMENTROOBOT_H
#include <QObject>
#include "tcp/tcpserverinstancegetter.h"
#include "robot_define.h"

class SupplementRoobot : public QObject
{
    Q_OBJECT
public:
    explicit SupplementRoobot(QObject *parent = NULL);
    ~SupplementRoobot();
    //设置IP
    bool SetIP(const QString &str_IP);
    //获取IP
    bool GetIP(QString &str_IP);
    //发送机器人测试数据
    void SendSupplementData(ushort us_FWStation,
                            const QString &str_Data);

private:
    //初始化分料机器人
    void InitSupplementRobot();

signals:
    //发送数据 to 机器人
    void sig_SendData(QByteArray byte_Data,
                      QString str_IP);
    //获取数据 from 机器人
    void sig_GetRequest(ushort us_FWStation,
                        QString str_Request);

public slots:
    //处理分料机器人发送过来的数据 
    void slot_SupplementReceiveData(STRUCT_TCPDATA struct_TcpData);

private slots:

private:
    TcpServerInstanceGetter m_oTcpServerInstanceGetter;     //本地TCP服务器管理类 
    TcpServer *m_pTcpServer;    //本地TCP服务器

    QString m_strIP;    //机器人IP
};

#endif // SUPPLEMENTROOBOT_H
