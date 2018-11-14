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

    bool SetIP(const QString &str_IP);

    bool GetIP(QString &str_IP);

    void SendSupplementData(ushort us_FWStation,
                            const QString &str_Data);

private:
    void InitSupplementRobot();

signals:
    void sig_SendData(QByteArray byte_Data,
                      QString str_IP);

    void sig_GetRequest(ushort us_FWStation,
                        QString str_Request);

public slots:
    void slot_SupplementReceiveData(STRUCT_TCPDATA struct_TcpData);

private slots:

private:
    TcpServerInstanceGetter m_oTcpServerInstanceGetter;
    TcpServer *m_pTcpServer;

    QString m_strIP;
};

#endif // SUPPLEMENTROOBOT_H
