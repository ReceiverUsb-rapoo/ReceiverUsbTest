#ifndef MASTERCONTROL_H
#define MASTERCONTROL_H
#include <QObject>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"
#include "counttestresultinstancegetter.h"
#include "DataFile/configfile.h"
#include "starttest_msgqueue.h"

class MasterControl : public QObject
{
    Q_OBJECT
public:
    explicit MasterControl(QObject *parent = 0);
    ~MasterControl();

public:
    //初始化测试
    bool InitTest();
    //开始一次测试
    bool StartOneTest();
    //开始自动测试
    bool StartAutoTest();
    //停止测试
    bool StopTest();

public:
    //获取所有固件工作序号
    void GetAllFWSequenceNumber(QList<ushort> &list_SequenceNumber);
    //获取所有箱子工作序号
    void GetAllBoxSequenceNumber(QList<ushort> &list_SequenceNumber);

private:
    //初始化主控
    bool InitMasterControl();
    //设置外围设备配置参数
    bool SetEquitmentConfig();
    //更新在线固件状态
    bool UpdataFWDevice();
    //更新在限箱子状态
    bool UpdataBox();
    //更新抓取机器人
    bool UpdataCatchRobot();
    //更新分料机器人
    bool UpdataSupplementRobot();
    //设置所有固件配置参数 
    bool SetAllFWPCConfig();
    //设置所有UsbControl配置参数
    bool SetAllUsbControlConfig();
    //开始观察设备状态（是否在线）
    bool ConnectAllPath();
    //开始观察测试状态
    void ConnectFuntion();
    //断开观察测试状态
    void DisconnctFuntion();
    //断开观察设备状态（是否在线）
    bool DisConnectAllPath();
    //初始化枚举和载波cmd发送 
    bool InitUsbEnumAndSendTest();
    //退出枚举和载波cmd发送
    bool ExitUsbEnumAndSendTest();
    //开始一组枚举测试 
    bool StartUsbEnumTestByOneGroup(const ushort &us_SequenceNumber,
                                    const QList<int> &list_OneGroupPowerTest);
    //开始一组功率测试
    bool StartUsbPowerTestByOneGroup(const ushort &us_SequenceNumber,
                                     const QList<int> &list_OneGroupSendTest);

//    void ShowResultStatisticalData();

private:
    //打开箱子
    bool OpenBox(const ushort &us_SequenceNumber);
    //关闭箱子
    bool CloseBox(const ushort &us_SequenceNumber);
    //发送抓取机器人动作
    bool SendCatchRobotAction(const ushort &us_FWStation,
                              const QString &str_RobotAction);
    //发送分料数据
    bool SendSupplementRobotData(const ushort &us_FWStation,
                                 const QString &str_Data);

private:
    //等待枚举测试完成
    void WaitUsbEnumTestFinish();
    //等待功率测试完成
    void WaitUsbPowerTestFinish();
    //延时时间
    void WorkSleep(uint un_Msec);

signals:
    //发送 添加匹配新固件
    void sig_FWDiscoverd();
    //发送 移除已匹配固件
    void sig_FWRemove();
    //发送 添加匹配新箱子
    void sig_BoxDiscoverd();
    //发送 添加抓取机器人
    void sig_BoxRemove();
    //发送 添加分料机器人
    void sig_CRobotDiscoverd();
    //发送 移除已匹配箱子
    void sig_CRobotRemove();
    //发送 移除抓取机器人
    void sig_SRobotDiscoverd();
    //发送 移除分料机器人
    void sig_SRobotRemove();

signals:
    //发送 开始测试通知
    void sig_StartTest(ushort us_SequenceNumber);
    //发送 完成测试通知
    void sig_CompleteTest(ushort us_SequenceNumber);

public slots:
    //发现新com 处理
    void slot_FirmwareDiscoverd();
    //发现com断开 处理
    void slot_FirmwareRemove();
    //添加匹配新箱子
    void slot_BoxDiscoverd();
    //移除已匹配箱子 
    void slot_BoxRemove();
    //添加抓取机器人
    void slot_CatchRobotDiscoverd();
    //移除抓取机器人
    void slot_CatchRobotRemove();
    //添加分料机器人 
    void slot_SupplementRobotDiscoverd();
    //移除分料机器人
    void slot_SupplementRobotRemove();

public slots:
    //收到 箱子动作
    void slot_BoxOperatorUpdata(ushort us_SequenceNumber);
    //收到 取料机器人动作
    void slot_CatchRobotGetActionUpdata(ushort us_SequenceNumber);
    //收到 分料机器人请求信号
    void slot_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber);

public slots:
    //收到 枚举usb完成
    void slot_EnumUsbComplete();
    //收到 载波cmd发送完成
    void slot_SendPowerTestComplete();
    //收到 开始测试通知
    void slot_StartTestNotice(ushort us_SequenceNumber);
    //收到 开始一组枚举测试
    void slot_StartOneGroupEnumTest(ushort us_SequenceNumber);
    //收到 开始一组功率测试
    void slot_StartOneGroupPowerTest(ushort us_SequenceNumber);
    //收到 完成测试
    void slot_CompleteTest(ushort us_SequenceNumber);

private slots:


private:
    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;   //观察者类管理类对象
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;   //操作者类管理类对象
    CountTestResultInstanceGetter m_oCountTestResultInstanceGetter; //计算结果类管理类对象
    StartTest_MsgQueue m_oStartTest_MsgQueue;   //开始测试消息队列

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;
    CountTestData *m_pCountTestData;

    QMap<ushort, STRUCT_USBCONTROLCONFIG> m_mapUsbControlConfig;    //UsbControl参数 包括点位Port ID <工作序号,配置参数>

    bool m_bEnumTestWorkState;          //枚举测试状态 （只能单一测试） 
    bool m_bPowerTestWorkState;         //功率测试状态 （只能单一测试）

    ushort m_usWorkingSequenceNumber;       //当前工作序号
    bool m_bBoxSwitchClose;                 //自动测试流程 屏蔽 箱子 开关 
    bool m_bRobotSwitchClose;               //自动测试流程 屏蔽 机器人 开关
    QMap<ushort, bool> m_mapRetestSwitch;   //重测开关 <工作序号,重测开关>
    bool m_bRetested;   //当前工作序号重测开关

    bool m_bAutoTest;   //自动测试锁

    bool m_bFirstTest;  //第一次测试状态
    uint m_unStartTimes;    //测试序号 （第一次，第二次......）
};

#endif // MASTERCONTROL_H
