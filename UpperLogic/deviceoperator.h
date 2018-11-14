﻿#ifndef DEVICEOPERATOR_H
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

    bool SetFWObjectPointer(QList<Firmware *> &list_FWObjectPointer);

    bool GetUsbControlObjectPointer(UsbControl * &p_UsbControl);

    bool SetFWPCConfig(const ushort &us_SequenceNumber,
                       STRUCT_PCTESTCONFIG &struct_PCConfig);

    bool SetBoxObjectPoint(QList<Box *> &list_Box);

    bool SetCatchRobotObjectPoint(CatchRobot * &p_CatchRobot);

    bool SetSupplementRobotObjectPoint(SupplementRoobot * &p_SupplementRobot);

//    bool SetFWPCConfig();

//    bool SetUsbControlObjectPointer(UsbControl *p_UsbControl);

//    bool RemoveUsbControlObjectPoint();

public:
    //USB
    bool CreatUsbControlObject();

    bool DeleteUsbControlObject();

    bool SetEnumTestUsbConfig(const uint &un_Pid,
                              const uint &un_Vid,
                              const int &n_DeviceNumber,
                              const int &n_Time,
                              const QMap<int,int> &map_StationPort);

    bool SetPowerTestConfig(const QMap<int,int> &map_StationPort);

    bool StartUsbEnumTest();

    bool StartSendPowerTest();

    bool ExitUsbTest();

public:
    //us_SequenceNumber == us_FWStation
    bool OpenBox(const ushort &us_SequenceNumber);

    bool CloseBox(const ushort &us_SequenceNumber);

    bool SendCatchRobotAction(const ushort &us_FWStation,
                              const QString &str_RobotAction);

    bool SendSupplementRobotData(const ushort &us_FWStation,
                                 const QString &str_Data);

public:
    //FW
    bool GetFWInfo(const ushort &us_SequenceNumber);

    bool RestartFW(const ushort &us_SequenceNumber);

    bool InitDUTTest(const ushort &us_SequenceNumber);

    bool ExitDUTTest(const ushort &us_SequenceNumber);

    bool StartOneTest(const ushort &us_SequenceNumber);

    bool InquireMachineState(const ushort &us_SequenceNumber);

    bool CompleteEnumTest(const ushort &us_SequenceNumber);

    bool EnterDebugModel(const ushort &us_SequenceNumber);

    bool ExitDebugModel(const ushort &us_SequenceNumber);

    bool StartWholeDUTTest(const ushort &us_SequenceNumber);

    bool AdjustPower(const ushort &us_SequenceNumber,
                     char c_Funtion,
                     short s_ReferenceVoltage_1,
                     short s_ReferenceVoltage_2,
                     uchar uc_Station,
                     char c_AdjustPoint,
                     char c_dp_Compensation);

    bool ReadPowerData(const ushort &us_SequenceNumber);

    bool ClearPowerConfig(const ushort &us_SequenceNumber,
                          const int &n_Station);

    bool TestPowerSelfTest(const ushort &us_SequenceNumber);

    bool TestEnum(const ushort &us_SequenceNumber,
                  const ENUM_POWERONGROUP &Group,
                  const short &s_Time);

    bool PowerOnSwitch(const ushort &us_SequenceNumber,
                       const ENUM_POWERONSWITCH &Switch,
                       const ENUM_POWERONGROUP &Group);

    bool PCACK_StartOneGroupPowerTest(const ushort &us_SequenceNumber);

private:
    void WorkSlepp(ushort un_Msec);

    bool GetFWPointer(const ushort &us_SequenceNumber,
                      Firmware * &p_FW);

    bool GetBoxPointer(const ushort &us_SequenceNumber,
                       Box * &p_Box);

signals:

public slots:

private slots:

private:
    QList<Firmware *> m_listFirmware;
    UsbControl *m_pUsbControl;

    QList<Box *> m_listBox;
    CatchRobot *m_pCatchRobot;
    SupplementRoobot *m_pSupplementRoobot;
};

#endif // DEVICEOPERATOR_H
