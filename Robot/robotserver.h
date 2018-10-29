#ifndef ROBOTSERVER_H
#define ROBOTSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QThread>
#include "stdint.h"

class RobotServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RobotServer(QObject *parent = 0);
    ~RobotServer();

    bool Init(const quint16 &us_Port, const QString &str_HostIP = "Any");
    bool SendData(const QString &str_Data, int n_TimeOut = 1000);
    bool DisConnect();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

signals:
    void sig_GetData(QString);
    void sig_Msg(QString);

private slots:
    void slot_NewConnect();
    void slot_ReceiveData();
    void slot_RemoveSocket();

private:
    QTcpSocket *m_pQTcpSocket;
    QString m_strReceiveData;
    QThread *m_pQThread;

    QString m_strIP;
    quint16 m_usPort;
};

#endif // ROBOTSERVER_H
