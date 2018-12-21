#ifndef CATCHROBOT_H
#define CATCHROBOT_H
#include <QObject>
#include "tcp/tcpserverinstancegetter.h"
#include "robot_define.h"

class CatchRobot : public QObject
{
    Q_OBJECT
public:
    explicit CatchRobot(QObject *parent = NULL);
    ~CatchRobot();
    //设置IP
    bool SetIP(const QString &str_IP);
    //获取IP
    bool GetIP(QString &str_IP);
    //发送机器人动作
    void SendAction(const ushort &us_FWStation,
                    const QString &str_RobotAction);

private:
    //初始化抓取机器人
    void InitCatchRobot();

signals:
    //发送数据 to 机器人
    void sig_SendData(QByteArray byte_Data,
                      QString str_IP);
    //获取数据 from 机器人
    void sig_GetAction(ushort us_FWStation,
                       QString str_Action);

public slots:
    //处理抓取机器人发送过来的数据 
    void slot_CatchRobotReceiveData(STRUCT_TCPDATA struct_TcpData);

private slots:

private:
    TcpServerInstanceGetter m_oTcpServerInstanceGetter;     //本地TCP服务器管理类     
    TcpServer *m_pTcpServer;    //本地TCP服务器

    QString m_strIP;    //机器人ipIP
};

#endif // CATCHROBOT_H
