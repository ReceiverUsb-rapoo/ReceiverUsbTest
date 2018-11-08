#ifndef CATCHROBOT_H
#define CATCHROBOT_H
#include <QObject>
#include "tcp/tcpserverinstancegetter.h"

const QByteArray CatchRobot_ACK = "";

class CatchRobot : public QObject
{
    Q_OBJECT
public:
    explicit CatchRobot(QObject *parent = NULL);
    ~CatchRobot();

    bool SetIP(const QString &str_IP);

    bool GetIP(QString &str_IP);

    void SendCatchData(const QString &str_Data);

private:
    void InitCatchRobot();

signals:
    void sig_SendData(QByteArray byte_Data,
                      QString str_IP);


public slots:
    void slot_CatchRobotReceiveData(STRUCT_TCPDATA struct_TcpData);

private slots:

private:
    TcpServerInstanceGetter m_oTcpServerInstanceGetter;
    TcpServer *m_pTcpServer;

    QString m_strIP;

};

#endif // CATCHROBOT_H
