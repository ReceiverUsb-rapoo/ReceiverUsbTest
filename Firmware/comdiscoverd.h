#ifndef COMDISCOVERD_H
#define COMDISCOVERD_H
#include <QTimer>
#include <QSerialPortInfo>
#include <QString>

/*
struct STRUCT_PORTINFO
{
    QString str_PortName;
    ushort us_PID;
    ushort us_VID;

    STRUCT_PORTINFO(){
        str_PortName = "";
        us_PID = 0;
        us_VID = 0;
    }
};
*/

class ComDiscoverd : public QObject
{
    Q_OBJECT
public:
    explicit ComDiscoverd(QObject *parent = 0);
    ~ComDiscoverd();

signals:
    void sig_ComDiscoverd(QString str_Port,
                          uint un_Pid,
                          uint un_Vid);

    void sig_ComRemove(QString str_Port,
                       uint un_Pid,
                       uint un_Vid);

public slots:


private slots:
    void slot_TimerOut();

private:
    QTimer *m_pQTimer;
    QList<QSerialPortInfo> m_listPortInfo;
};

#endif // COMDISCOVERD_H
