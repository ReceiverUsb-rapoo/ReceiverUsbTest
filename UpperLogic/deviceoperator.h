#ifndef DEVICEOPERATOR_H
#define DEVICEOPERATOR_H
#include <QObject>
#include "Firmware/firmware.h"
#include "UsbControl/usbcontrol.h"
#include "upperdefine.h"
#include "Box/box.h"
#include "Robot/catchrobot.h"
#include "Robot/supplementroobot.h"


class DeviceOperator : public QObject
{
    Q_OBJECT
    friend class DeviceOperatorInstanceGetter;

private:
    explicit DeviceOperator(QObject *parent = NULL);

public:
    ~DeviceOperator();
    //设置固件对象列表 
    bool SetFWObjectPointer(QList<Firmware *> &list_FWObjectPointer);
    //获取UsbControl对象指针
    bool GetUsbControlObjectPointer(UsbControl * &p_UsbControl);
    //按工作序号设置固件配置
    bool SetFWPCConfig(const ushort &us_SequenceNumber,
                       STRUCT_PCTESTCONFIG &struct_PCConfig);
    //设置箱子对像指针
    bool SetBoxObjectPoint(QList<Box *> &list_Box);
    //设置抓取机器人对象指针
    bool SetCatchRobotObjectPoint(CatchRobot * &p_CatchRobot);
    //设置分料机器人对象指针
    bool SetSupplementRobotObjectPoint(SupplementRoobot * &p_SupplementRobot);

//    bool SetFWPCConfig();

//    bool SetUsbControlObjectPointer(UsbControl *p_UsbControl);

//    bool RemoveUsbControlObjectPoint();

public:
    //USB
    //创建UsbControl对象
    bool CreatUsbControlObject();
    //删除UsbControl对象 
    bool DeleteUsbControlObject();
    //设置枚举测试参数
    bool SetEnumTestUsbConfig(const uint &un_Pid,
                              const uint &un_Vid,
                              const int &n_DeviceNumber,
                              const int &n_Time,
                              const QMap<int,int> &map_StationPort);
    //设置功率测试参数
    bool SetPowerTestConfig(const QMap<int,int> &map_StationPort);
    //开始枚举
    bool StartUsbEnumTest();
    //开始功率测试
    bool StartSendPowerTest();
    //退出枚举测试
    bool ExitUsbEnumTest();
    //退出功率测试
    bool ExitSendPowerTest();
    //退出usb测试
    bool ExitUsbTest();

public:
    //us_SequenceNumber == us_FWStation
    //打开箱子
    bool OpenBox(const ushort &us_SequenceNumber);
    //关闭箱子
    bool CloseBox(const ushort &us_SequenceNumber);
    //发送抓取动作
    bool SendCatchRobotAction(const ushort &us_FWStation,
                              const QString &str_RobotAction);
    //发送分料数据
    bool SendSupplementRobotData(const ushort &us_FWStation,
                                 const QString &str_Data);

public:
    //FW
    //获取固件信息
    bool GetFWInfo(const ushort &us_SequenceNumber);
    //复位固件
    bool RestartFW(const ushort &us_SequenceNumber);
    //初始化固件
    bool InitDUTTest(const ushort &us_SequenceNumber);
    //退出/停止 DUT测试
    bool ExitDUTTest(const ushort &us_SequenceNumber);
    //开始一次DUT测试
    bool StartOneTest(const ushort &us_SequenceNumber);
    //查询固件状态
    bool InquireMachineState(const ushort &us_SequenceNumber);
    //完成枚举通知
    bool CompleteEnumTest(const ushort &us_SequenceNumber);
    //进入调试模式
    bool EnterDebugModel(const ushort &us_SequenceNumber);
    //退出调试模式
    bool ExitDebugModel(const ushort &us_SequenceNumber);
    //测试整个DUT流程
    bool StartWholeDUTTest(const ushort &us_SequenceNumber);
    //校准功率
    bool AdjustPower(const ushort &us_SequenceNumber,
                     char c_Funtion,
                     short s_ReferenceVoltage_1,
                     short s_ReferenceVoltage_2,
                     uchar uc_Station,
                     char c_AdjustPoint,
                     char c_dp_Compensation);
    //读取校准功率
    bool ReadPowerData(const ushort &us_SequenceNumber);
    //清除校准功率
    bool ClearPowerConfig(const ushort &us_SequenceNumber,
                          const int &n_Station);
    //测试功率自检
    bool TestPowerSelfTest(const ushort &us_SequenceNumber);
    //测试枚举
    bool TestEnum(const ushort &us_SequenceNumber,
                  const ENUM_POWERONGROUP &Group,
                  const short &s_Time);
    //控制上电
    bool PowerOnSwitch(const ushort &us_SequenceNumber,
                       const ENUM_POWERONSWITCH &Switch,
                       const ENUM_POWERONGROUP &Group);
    //pc-ack 开始一组功率测试回复
    bool PCACK_StartOneGroupPowerTest(const ushort &us_SequenceNumber);

private:
    //延时时间 
    void WorkSlepp(ushort un_Msec);
    //获取固件指正 按工作 序号  
    bool GetFWPointer(const ushort &us_SequenceNumber,
                      Firmware * &p_FW);
    //获取箱子指正 按工作 序号
    bool GetBoxPointer(const ushort &us_SequenceNumber,
                       Box * &p_Box);

signals:

public slots:

private slots:

private:
    QList<Firmware *> m_listFirmware;       //固件对象列表
    UsbControl *m_pUsbControl;              //usb测试控制对象

    QList<Box *> m_listBox;                 //箱子对象列表
    CatchRobot *m_pCatchRobot;              //抓取机器人对象 
    SupplementRoobot *m_pSupplementRoobot;  //分料机器人对象 
};

#endif // DEVICEOPERATOR_H
