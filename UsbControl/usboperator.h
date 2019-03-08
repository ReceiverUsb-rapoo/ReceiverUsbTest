#ifndef USBOPERATOR_H
#define USBOPERATOR_H
#include <QObject>
#include <QMap>
#include <QThread>
#include "libusb-1.0/libusb.h"

class UsbOperator : public QThread
{
    Q_OBJECT
public:
    explicit UsbOperator(QThread *parent = 0);
    ~UsbOperator();
    //初始化 libusb_init
    bool InitUsbOperator();
    //退出，清除所有 linusb_exit
    bool ExitUsbOperator();
    //开始新usb操作，重置数据
    bool StartNewOperator();
    //获取设备地址
    bool GetDevice(QMap<int, libusb_device *> &map_LDevice);
    //获取设备com端口ID
    bool GetDevicePort(QList<int> &list_DevicePort);
    //设置查找usb设备配置参数
    bool SetFindUsbDeviceConfig(const uint &un_Pid,
                                const uint &un_Vid,
                                const int &n_DeviceNumber,
                                const int &n_Time);

private:
    //延时函数 睡眠时间
    void WorkSleep(uint un_Msec);
    //查找usb设备com端口ID
    bool FindUsbDevicePort(const uint &un_Pid,
                           const uint &un_Vid,
                           QMap<int, libusb_device *> &map_LDevice,
                           QList<int> &list_DevicePort);
    //完成枚举（run）
    void RunComplete();

protected:
    //枚举
    void run();

signals:
    //发送 完成枚举
    void sig_RunComplete();

private slots:

private:
    libusb_context  *m_pLibusbContext;  //libusb 初始化返回句柄
    bool m_bLibusbState;                //libusb 状态
    uint m_unPid;                       //usb PID
    uint m_unVid;                       //usb PID
    int m_nRunTime;                     //枚举最长时间
    int m_nDeviceNumber;                //枚举个数
    QMap<int, libusb_device * > m_mapLDevice;   //<位置,设备地址>
    QList<int> m_listDevicePort;    //枚举完成 所有com端口ID
};

#endif // USBOPERATOR_H
