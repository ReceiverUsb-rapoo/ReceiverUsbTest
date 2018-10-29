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

private:
    void InitTest();

    void StartOneTest();

    void StartAutoTest();

    void StopTest();

private:
    bool InitMasterControl();

    bool SetConnectFWConfig();

    bool UpdataFWDevice();

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

signals:

public slots:
    void slot_FirmwareDiscoverd();

    void slot_FirmwareRemove();

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
};

#endif // MASTERCONTROL_H
