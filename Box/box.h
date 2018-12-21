#ifndef BOX_H
#define BOX_H
#include <QObject>
#include "tcp/tcpserverinstancegetter.h"
#include "box_define.h"

class Box : public QObject
{
    Q_OBJECT
public:
    explicit Box(QObject *parent = NULL);
    ~Box();

    //设置箱子，本地服务器IP
    bool SetIP(const QString &str_IP);
    //获取箱子，本地服务器IP
    bool GetIP(QString &str_IP);
    //设置箱子序号
    bool SetSequenceNumber(const ushort &us_SequenceNumber);
    //获取箱子序号
    bool GetSequenceNumber(ushort &us_SequenceNumber);
    //打开 箱子
    void OpenBox();
    //关闭 箱子
    void CloseBox();

private:
    //启动本类，初始化箱子
    void InitBox();

signals:
    //发送数据
    void sig_SendData(QByteArray byte_Data,
                      QString str_IP);
    //发送箱子动作
    void sig_BoxOperator(ushort us_SequenceNumber,
                         BOX_OPERATOR box_Operator);

public slots:
    //接收箱子传送的数据
    void slot_BoxReceiveData(STRUCT_TCPDATA struct_TcpData);

private slots:

private:
    TcpServerInstanceGetter m_oTcpServerInstanceGetter; //本地tcp服务器类管理类 
    TcpServer *m_pTcpServer;    //本地服务器类

    QString m_strIP;            //箱子IP
    ushort m_usSequenceNumber;  //箱子动作序号
    int m_nCountResendOpen;     //箱子打开次数
    int m_nCountResendClose;    //箱子关闭次数
};

#endif // BOX_H
