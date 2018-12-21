#ifndef FIREWAREDEBUG_H
#define FIREWAREDEBUG_H
#include <QDialog>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"
#include "DataFile/configfile.h"

/*
* 固件调试ui
* 进调试模式 大部分cmd数据可视化，观察数据以便后期调试 
*/

namespace Ui {
class FirewareDebug;
}

class FirewareDebug : public QDialog
{
    Q_OBJECT
public:
    explicit FirewareDebug(ushort us_SequenceNumber, QWidget *parent = 0);
    ~FirewareDebug();
    //设置工作序号
    void SetSequenceNumber(ushort us_SequenceNumber);

private:
    //初始化
    void InitFirewareDebug();
    //退出
    void ExitFirewareDebug();
    //数据转换显示 map to QString
    void ShowDataByMap(QString str_DataName,
                       QMap<int, bool> &map_Data,
                       QString str_Unit);
    //数据显示长度补全 1位自动添加空格（1 ） 
    QString AutoCoverDouble(int n_Data);
    //数据位置显示补全 1位自动补0（01）
    QString AutoCoverDouble(QString str_Info);
    //显示调试以及数据信息
    void ShowInfo(QString str_Info);
    //初始化枚举和功率测试
    bool InitUsbEnumAndSendTest();
    //开始枚举测试 map包含测试位置 
    bool StartUsbEnumTest(const uint &un_Pid,
                          const uint &un_Vid,
                          const int &n_DeviceNumber,
                          const int &n_Time,
                          const QMap<int, int> &map_StationPort);
    //开始一组功率测试 map包含测试位置
    bool StartUsbSendTest(const QMap<int, int> &map_StationPort);
    //退出枚举和功率测试
    bool ExitUsbEnumAndSendTest();
    //开始一组枚举测试
    bool StartUsbEnumTestByOneGroup(const QList<int> &list_OneGroupPowerTest);
    //开始一组功率测试
    bool StartUsbPowerTestByOneGroup(const QList<int> &list_OneGroupSendTest);
    //获取一组port端口ID
    bool GetOneGroupStationPort(const ENUM_POWERONGROUP &GroupStation,
                                QMap<int, int> &map_OneGroupStationPort);
    //睡眠时间
    void WorkSleep(ushort un_Msec);

private:
    //进入调试模式
    void EnterDebugModel();
    //推出调试模式
    void ExitDebugModel();
    //获取固件信息
    void GetFWInfo();
    //复位固件
    void RestartFW();
    //获取固件设备状态
    void InquireMachineState();
    //第一组上电
    void PowerOn_1();
    //第二组上电
    void PowerOn_2();
    //第一组断电
    void PowerOff_1();
    //第二组断电 
    void PowerOff_2();
    //功率自检 
    void TestPowerSelf();
    //第一组枚举测试
    void TestEnum_1();
    //第二组枚举测试
    void TestEnum_2();
    //测试功率测试
    void TestSendPower();
    //整个DUT测试
    void TestWholeDUT();
    //清除信息显示
    void ClearInfoShow();

private:
    //获取枚举完成数据
    void GetEnumUsbComplete();
    //获取功率测试完成数据
    void GetSendPowerTestCompleteData();
    //获取固件发送数据 cmd + 数据
    void GetReceiveCmdData(uchar uc_Command);
    //获取pc发送数据 cmd + 数据
    void GetWriteCmdData(uchar uc_Command);
    //获取固件测试状态
    void GetMachineStateData();
    //获取功率自检测试数据
    void GetPowerSelfTestData();
    //获取开始测试通知数据
    void GetStartTestNoticeData();
    //获取测试超时时间
    void GetTestTimeOutData();
    //获取开始一组功率测试数据
    void GetStartOneGroupPowerTestData(QList<int> &list_OneGroupPowerTestMaskCode);
    //获取开始一组枚举测试数据
    void GetStartOneGroupEnumTestData(QList<int> &list_OneGroupEnumTestMaskCode);
    //获取上传RF功率结果数据
    void GetUploadRFPowerResultData();
    //清除固件测试数据
    void GetCompleteTestData();

public slots:
    //收到 枚举usb完成
    void slot_EnumUsbComplete();
    //收到 载波cmd发送完成
    void slot_SendPowerTestComplete();
    //收到 接收cmd数据
    void slot_ReceiveCmd(ushort us_SequenceNumber,
                         uchar uc_Command);
    //收到 写入cmd数据
    void slot_WriteCmd(ushort us_SequenceNumber,
                       uchar uc_Command);
    //收到 固件工作状态
    void slot_MachineState(ushort us_SequenceNumber);
    //收到 功率自检测试
    void slot_PowerSelfTest(ushort us_SequenceNumber);
    //收到 开始测试通知
    void slot_StartTestNotice(ushort us_SequenceNumber);
    //收到 测试时间
    void slot_TestTimeOut(ushort us_SequenceNumber);
    //收到 开始一组功率测试
    void slot_StartOneGroupPowerTest(ushort us_SequenceNumber);
    //收到 开始一组枚举测试
    void slot_StartOneGroupEnumTest(ushort us_SequenceNumber);
    //收到 上传RF功率结果
    void slot_UploadRFPowerResult(ushort us_SequenceNumber);
    //收到 完成测试
    void slot_CompleteTest(ushort us_SequenceNumber);

private:
    Ui::FirewareDebug *ui;

    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;

    ushort m_usSequenceNumber;                          //工作序号
    STRUCT_USBCONTROLCONFIG m_structUsbControlConfig;   //UsbControl 配置参数
    bool m_bAllDeviceEnum;                              //全部位置枚举

    QList<int> m_listOneGroupPowerTest;                 //无效
    QList<int> m_listOneGroupEnumTest;                  //无效        

    ENUM_POWERONGROUP m_PowerTestGroup;                 //当前测试测试组序

};

#endif // FIREWAREDEBUG_H
