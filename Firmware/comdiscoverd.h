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
    //发送 新匹配com连接
    void sig_ComDiscoverd(QString str_Port,
                          uint un_Pid,
                          uint un_Vid);
    //发送 已匹配com断开
    void sig_ComRemove(QString str_Port,
                       uint un_Pid,
                       uint un_Vid);

public slots:


private slots:
    //定时器  用于定时枚举com设备
    void slot_TimerOut();

private:
    QTimer *m_pQTimer;                      
    QList<QSerialPortInfo> m_listPortInfo;  //已枚举com的信息集
};

#endif // COMDISCOVERD_H
