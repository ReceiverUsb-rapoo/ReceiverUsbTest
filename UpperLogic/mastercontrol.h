#ifndef MASTERCONTROL_H
#define MASTERCONTROL_H
#include <QObject>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"
#include "DataFile/configfile.h"

class MasterControl : public QObject
{
    Q_OBJECT
public:
    explicit MasterControl(QObject *parent = 0);
    ~MasterControl();

public:
    bool InitTest();

    bool StartOneTest();

    bool StartAutoTest();

    bool StopTest();

public:
    void GetAllSequenceNumber(QList<ushort> &list_SequenceNumber);

private:
    bool InitMasterControl();

    bool SetEquitmentConfig();

    bool UpdataFWDevice();

    bool UpdataBox();

    bool UpdataCatchRobot();

    bool SetAllFWPCConfig();

    bool SetAllUsbControlConfig();

    bool ConnectAllPath();

    bool DisConnectAllPath();

    bool InitUsbEnumAndSendTest(const ushort &us_SequenceNumber);

    bool ExitUsbEnumAndSendTest(const ushort &us_SequenceNumber);

    bool StartUsbEnumTestByOneGroup(const ushort &us_SequenceNumber,
                                    const QList<int> &list_OneGroupPowerTest);

    bool StartUsbPowerTestByOneGroup(const ushort &us_SequenceNumber,
                                     const QList<int> &list_OneGroupSendTest);

//    void ShowResultStatisticalData();

private:
    bool OpenBox(const ushort &us_SequenceNumber);

    bool CloseBox(const ushort &us_SequenceNumber);

    bool SendCatchRobotAction(const ushort &us_FWStation,
                              const QString &str_RobotAction);

    bool SendSupplementRobotData(const ushort &us_FWStation,
                                 const QString &str_Data);

private:
    void WaitUsbEnumTestFinish();

    void WaitUsbPowerTestFinish();

signals:
    void sig_FWDiscoverd();

    void sig_FWRemove();

public slots:
    void slot_FirmwareDiscoverd();

    void slot_FirmwareRemove();

    void slot_BoxDiscoverd();

    void slot_CatchRobotDiscoverd();

    void slot_BoxRemove();

    void slot_CatchRobotRemove();

    void slot_CatchRobotRemove();

    void slot_SupplementRobotRemove();

public slots:
    void slot_BoxOperatorUpdata(ushort us_SequenceNumber);

    void slot_CatchRobotGetActionUpdata(ushort us_SequenceNumber);

    void slot_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber);

public slots:
    void slot_EnumUsbComplete();

    void slot_SendPowerTestComplete();

    void slot_StartTestNotice(ushort us_SequenceNumber);

    void slot_StartOneGroupEnumTest(ushort us_SequenceNumber);

    void slot_StartOneGroupPowerTest(ushort us_SequenceNumber);

    void slot_CompleteTest(ushort us_SequenceNumber);

private slots:


private:
    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;

    QMap<ushort, STRUCT_USBCONTROLCONFIG> m_mapUsbControlConfig;

    bool m_bEnumTestWorkState;
    bool m_bPowerTestWorkState;
};

#endif // MASTERCONTROL_H
