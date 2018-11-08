#ifndef FIREWAREDEBUG_H
#define FIREWAREDEBUG_H
#include <QDialog>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"
#include "DataFile/configfile.h"

namespace Ui {
class FirewareDebug;
}

class FirewareDebug : public QDialog
{
    Q_OBJECT
public:
    explicit FirewareDebug(ushort us_SequenceNumber, QWidget *parent = 0);
    ~FirewareDebug();

    void SetSequenceNumber(ushort us_SequenceNumber);

private:
    void InitFirewareDebug();

    void ExitFirewareDebug();

    void ShowDataByMap(QString str_DataName,
                       QMap<int, bool> &map_Data,
                       QString str_Unit);

    QString AutoCoverDouble(int n_Data);

    QString AutoCoverDouble(QString str_Info);

    void ShowInfo(QString str_Info);

    bool InitUsbEnumAndSendTest();

    bool StartUsbEnumTest(const uint &un_Pid,
                          const uint &un_Vid,
                          const int &n_DeviceNumber,
                          const int &n_Time,
                          const QMap<int, int> &map_StationPort);

    bool StartUsbSendTest(const QMap<int, int> &map_StationPort);

    bool ExitUsbEnumAndSendTest();

    bool StartUsbEnumTestByOneGroup(const QList<int> &list_OneGroupPowerTest);

    bool StartUsbPowerTestByOneGroup(const QList<int> &list_OneGroupSendTest);

    bool GetOneGroupStationPort(const ENUM_POWERONGROUP &GroupStation,
                                QMap<int, int> &map_OneGroupStationPort);

    void WorkSleep(ushort un_Msec);

private:
    void EnterDebugModel();

    void ExitDebugModel();

    void GetFWInfo();

    void RestartFW();

    void InquireMachineState();

    void PowerOn_1();

    void PowerOn_2();

    void PowerOff_1();

    void PowerOff_2();

    void TestPowerSelf();

    void TestEnum_1();

    void TestEnum_2();

    void TestSendPower();

    void TestWholeDUT();

    void ClearInfoShow();

private:
    void GetEnumUsbComplete();

    void GetSendPowerTestCompleteData();

    void GetReceiveCmdData(uchar uc_Command);

    void GetWriteCmdData(uchar uc_Command);

    void GetMachineStateData();

    void GetPowerSelfTestData();

    void GetStartTestNoticeData();

    void GetTestTimeOutData();

    void GetStartOneGroupPowerTestData(QList<int> &list_OneGroupPowerTestMaskCode);

    void GetStartOneGroupEnumTestData(QList<int> &list_OneGroupEnumTestMaskCode);

    void GetUploadRFPowerResultData();

    void GetCompleteTestData();

public slots:
    void slot_EnumUsbComplete();

    void slot_SendPowerTestComplete();

    void slot_ReceiveCmd(ushort us_SequenceNumber,
                         uchar uc_Command);

    void slot_WriteCmd(ushort us_SequenceNumber,
                       uchar uc_Command);

    void slot_MachineState(ushort us_SequenceNumber);

    void slot_PowerSelfTest(ushort us_SequenceNumber);

    void slot_StartTestNotice(ushort us_SequenceNumber);

    void slot_TestTimeOut(ushort us_SequenceNumber);

    void slot_StartOneGroupPowerTest(ushort us_SequenceNumber);

    void slot_StartOneGroupEnumTest(ushort us_SequenceNumber);

    void slot_UploadRFPowerResult(ushort us_SequenceNumber);

    void slot_CompleteTest(ushort us_SequenceNumber);

private:
    Ui::FirewareDebug *ui;

    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;

    ushort m_usSequenceNumber;
    STRUCT_USBCONTROLCONFIG m_structUsbControlConfig;
    bool m_bAllDeviceEnum;

    QList<int> m_listOneGroupPowerTest;
    QList<int> m_listOneGroupEnumTest;

    ENUM_POWERONGROUP m_PowerTestGroup;

};

#endif // FIREWAREDEBUG_H
