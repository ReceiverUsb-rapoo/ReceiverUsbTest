#ifndef BOX_H
#define BOX_H
#include <QObject>
#include "tcp/tcpserverinstancegetter.h"

enum BOX_OPERATOR{
    OPENBOX,
    CLOSEBOX,
    ERRORBOX
};

const QByteArray OpenCmd = "open";
const QByteArray CloseCmd = "close";

const QByteArray Open_ACK_Ok = "open/ok";
const QByteArray Close_ACK_OK = "close/ok";

const QByteArray Open_ACK_Faile = "open/ng";
const QByteArray Close_ACK_Faile = "close/ng";

class Box : public QObject
{
    Q_OBJECT
public:
    explicit Box(QObject *parent = NULL);
    ~Box();

    bool SetIP(const QString &str_IP);

    bool GetIP(QString &str_IP);

    bool SetSequenceNumber(const ushort &us_SequenceNumber);

    bool GetSequenceNumber(ushort &us_SequenceNumber);

    void OpenBox();

    void CloseBox();

private:
    void InitBox();

signals:
    void sig_SendData(QByteArray byte_Data,
                      QString str_IP);

    void sig_BoxOperator(ushort us_SequenceNumber,
                         BOX_OPERATOR box_Operator);

public slots:
    void slot_BoxReceiveData(STRUCT_TCPDATA struct_TcpData);

private slots:

private:
    TcpServerInstanceGetter m_oTcpServerInstanceGetter;
    TcpServer *m_pTcpServer;

    QString m_strIP;
    ushort m_usSequenceNumber;
    int m_nCountResendOpen;
    int m_nCountResendClose;
};

#endif // BOX_H
