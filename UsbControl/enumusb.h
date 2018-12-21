#ifndef ENUMUSB_H
#define ENUMUSB_H
#include "usboperator.h"

class EnumUsb : public QObject
{
    Q_OBJECT
public:
    explicit EnumUsb(QObject *parent = 0);
    ~EnumUsb();
    //初始化
    bool InitEnumUsb();
    //退出
    bool CloseEmumUsb();
    //获取已匹配设备地址
    bool GetDevice(QMap<int, libusb_device *> &map_LDevice);
    //设置枚举配置
    bool SetEnumUsbConfig(const uint &un_Pid,
                          const uint &un_Vid,
                          const int &n_DeviceNumber,
                          const int &n_Time,
                          const QMap<int,int> &map_StationPort);
    //开始新的枚举
    bool StartNewEnumUsb();
    //获取枚举结果
    bool GetEnumResult(QMap<int,bool> &map_EnumResult);

private:
    //睡眠时间
    void WorkSleep(ushort un_Msec);
    //枚举完成
    bool EnumComplete();

signals:
    //发送 枚举完成
    void sig_EnumComplete();

public slots:
    //收到 枚举完成
    void slot_EnumComplete();

private slots:

private:
    UsbOperator *m_pUsbOperator;
    QMap<int,int> m_mapStationPort;     //<位置,com端口ID>，枚举结果判定依据
};

#endif // ENUMUSB_H
