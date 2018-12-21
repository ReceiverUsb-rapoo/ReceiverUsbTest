#ifndef USBCONTROL_H
#define USBCONTROL_H
#include "QObject"
#include "enumusb.h"
#include "powertest.h"
#include "powertestinstancegetter.h"

const int OneGroupUsbNumber = 20;

class UsbControl : public QObject
{
    Q_OBJECT
    friend class DeviceOperator;

private:
    explicit UsbControl(QObject *parent = 0);

public:
    ~UsbControl();
    //初始化
    bool InitUsbControl();
    //设置枚举测试配置参数
    bool SetEnumTestUsbConfig(const uint &un_Pid,
                              const uint &un_Vid,
                              const int &n_DeviceNumber,
                              const int &n_Time,
                              const QMap<int,int> &map_StationPort);
    //设置功率测试配置参数
    bool SetPowerTestConfig(const QMap<int,int> &map_StationPort);
    //开始枚举usb
    bool StartUsbEnumTest();
    //开始功率测试 
    bool StartPowerTest();
    //获取枚举结果
    bool GetEnumResult(QMap<int,bool> &map_EnumResult);
    //获取测试结果 
    bool GetTestResult(QMap<int,bool> &map_EnumResult,
                       QMap<int,bool> &map_OpenDeviceResult,
                       QMap<int, bool> &map_SendCmdResult);
    //退出，清除所有
    bool ExitUsbControl();
    //退出枚举测试 
    bool ExitEnumUsbTest();
    //退出功率测试
    bool ExitPowerTest();
private:
    //延时时间 
    void WorkSleep(ushort un_Msec);

signals:
    //发送 枚举完成
    void sig_EnumUsbComplete();
    //发送 功率测试完成
    void sig_SendPowerTestComplete();

public slots:
    //收到 枚举测试完成 
    void slot_EnumUsbTestComplete();
    //收到 功率测试完成
    void slot_SendPowerTestomplete();

private slots:

private:
    EnumUsb *m_pEnumUsb;
    PowerTest *m_pPowerTest;
    PowerTestInstanceGetter m_oPowerTestInstanceGetter;     //功率测试类管理类
};

#endif // USBCONTROL_H
