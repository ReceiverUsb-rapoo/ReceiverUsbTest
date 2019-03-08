#ifndef POWERTEST_H
#define POWERTEST_H
#include <QThreadPool>
#include <QMutex>
#include <QTimer>
#include <QObject>
#include <QMap>
#include "libusb-1.0/libusb.h"

class PowerTest : public QObject
{
    Q_OBJECT
    friend class PowerTestInstanceGetter;

private:
    explicit PowerTest(QObject *parent = 0);

public:
    ~PowerTest();

public:
    //设置功率测试配置
    bool SetPowerTestConfig(const QMap<int,int> &map_StationPort);
    //打开设备
    bool OpenUsbDevice(const QMap<int, libusb_device *> &map_LDevice);
    //关闭所有已打开的设备
    bool CloseUsbDevice();
    //开始功率测试 
    bool StartPowerTest();
    //获取功率测试结果
    bool GetPowerTestResult(QMap<int,bool> &map_OpenDeviceResult,
                            QMap<int, bool> &map_SendCmdResult);

    //for other thread 指定位置发送功率cmd  并发
    bool WriteSendCmdResult(int n_Station, bool b_Result);
    //for other thread 传出设备句柄 并发
    bool StartSinglePowerTest(libusb_device_handle *p_LDHandle);

private:
    //写入cmd 数据
    bool WriteCmdInDongle(libusb_device_handle *p_LDHandle,
                          uchar *uc_Data,
                          ushort us_Length);
    //获取cmd 数据
    bool ReadDetailsFromDongle(libusb_device_handle *p_LDHandle,
                               uchar *uc_Data,
                               ushort us_Length);
    //发送cmd
    bool SendCmdToDogle(libusb_device_handle *p_LDHandle,
                        uchar *uc_Data,
                        ushort us_Length);
    //判断能否进入命令模式
    bool JudgeEnterCommandModel(libusb_device_handle *p_LDHandle);
    //进入测试模式
    bool EnterTestModel(libusb_device_handle *p_LDHandle);
    //测试单载波模式
    bool TestSignalCarrier(libusb_device_handle *p_LDHandle);
    //完成功率测试
    bool CompletePowerTest();
    //睡眠时间
    void WorkSleep(uint un_Msec);

signals:    
    //发送cmd完成
    void sig_SendCmdComplete();

private slots:
    //超时处理
    void slot_TimeOut();

private:
    QThreadPool m_oQThreadPool;     //线程池实例化对象
    QMutex m_oQMutex;               //锁
    QTimer *m_pQTimer;              //功率测试超时定时器  

    QMap<int, libusb_device_handle * > m_mapLDHandle;   //<位置,设备句柄>
    QMap<int, bool> m_mapSendCmdResult;                 //<位置,发送cmd结果>
    QMap<int, bool> m_mapOpenDeviceResult;              //<位置,打开设备结果>
    QMap<int, int> m_mapStationPort;                    //<位置,com端口id>

    bool m_bSendComplete;                               //发送cmd完成
};

#endif // POWERTEST_H
