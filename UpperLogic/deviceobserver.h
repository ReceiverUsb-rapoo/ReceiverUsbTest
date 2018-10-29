#ifndef DEVICEOBSERVER_H
#define DEVICEOBSERVER_H
#include "Firmware/firmware.h"
#include "Firmware/comdiscoverd.h"
#include "UsbControl/usbcontrol.h"
#include "fwuploaddata.h"
#include "upperdefine.h"
#include "Robot/robot.h"
#include "Box/box.h"

class DeviceObserver : public QObject
{
    Q_OBJECT
    friend class DeviceObserverInstanceGetter;

private:
    explicit DeviceObserver(QObject *parent = NULL);

public:
    ~DeviceObserver();

    bool SetNeedFWConfig(const QList<STRUCT_COMINFO> &list_FWComInfo);

    bool SetOpenFWModelByCom();

    bool SetOpenFWModelByPidVid();

//    bool SetManuallyCreatUsbControl();

    bool GetFWObjectPointer(QList<Firmware *> &list_FWObjectPointer);

    bool GetUsbControlObjectPointer(UsbControl * &p_UsbControl);

    bool SetUsbControlObjectPointer(UsbControl * &p_UsbControl);

    bool RemoveUsbControlObjectPoint();

//    bool UploadRobotObjectPointer(Robot * p_Robot);

//    bool UploadBoxObjectPointer(Box * p_Box);


public:
    void GetEnumUsbCompleteData(EMUN_TESTDATAPOINT Point,
                                QMap<int,bool> &map_EnumResult);

    void GetSendPowerTestCompleteData(EMUN_TESTDATAPOINT Point,
                                      QMap<int,bool> &map_EnumResult,
                                      QMap<int,bool> &map_OpenDeviceResult,
                                      QMap<int, bool> &map_SendCmdResult);

    void GetReceiveCommandAndData(const ushort &us_SequenceNumber,
                                  uchar uc_Command,
                                  STRUCT_COMMANDANDDATA &struct_CommandAndData);

    void GetWriteCommandAndData(const ushort &us_SequenceNumber,
                                uchar c_Command,
                                STRUCT_COMMANDANDDATA &struct_CommandAndData);

    void GetGetFWInfoData(EMUN_TESTDATAPOINT Point,
                          const ushort &us_SequenceNumber,
                          STRUCT_FWINFO &struct_FWInfo);

    void GetTestTimeOutData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            int &n_TestTimeOut);

    void GetMachineStateData(EMUN_TESTDATAPOINT Point,
                             const ushort &us_SequenceNumber,
                             ENUM_MACHINEDUTSTATE &DUTState,
                             ENUM_MACHINETESTSTATE &TestState);

    void GetAdjustPowerData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            short &s_Vslop,
                            short &s_VslopAdc,
                            short &s_Intercept);

    void GetReadPowerData(EMUN_TESTDATAPOINT Point,
                          const ushort &us_SequenceNumber,
                          QList<short> &list_Vslop,
                          QList<short> &list_VslopAdc,
                          QList<short> &list_Intercept,
                          QList<char> &list_db_Compensation,
                          short &s_ReferenceVoltage_1,
                          short &s_ReferenceVoltage_2);

    void GetPowerSelfTestData(EMUN_TESTDATAPOINT Point,
                              const ushort &us_SequenceNumber,
                              QList<short> &list_SelfVoltage);

    void GetFWBootData(EMUN_TESTDATAPOINT Point,
                       const ushort &us_SequenceNumber,
                       STRUCT_FWINFO &struct_FWInfo);

    void GetFWIntactData(EMUN_TESTDATAPOINT Point,
                         const ushort &us_SequenceNumber,
                         ENUM_MSGDEFINE &FWIntact);

    void GetHandDataBagData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            STRUCT_HANDBAG &struct_HandBag);

    void GetStartTestNoticeData(EMUN_TESTDATAPOINT Point,
                                const ushort &us_SequenceNumber,
                                char &c_TestType,
                                char &c_TotalGroup,
                                char &c_AmoutOfAGroup,
                                QList<char> &list_UsefulDUT);

    void GetCompleteTestData(EMUN_TESTDATAPOINT Point,
                             const ushort &us_SequenceNumber,
                             QList<char> &list_UsefulResult);

    void GetStartOneGroupPowerTestData(EMUN_TESTDATAPOINT Point,
                                       const ushort &us_SequenceNumber,
                                       char &c_SurplusGroup,
                                       int &n_TestDUTMask,
                                       QList<int> &list_MaskCode);

    void GetStartOneGroupEnumTestData(EMUN_TESTDATAPOINT Point,
                                      const ushort &us_SequenceNumber,
                                      char &c_SurplusGroup,
                                      int &n_TestDUTMask,
                                      QList<int> &list_MaskCode);

    void GetUploadRFPowerResultData(EMUN_TESTDATAPOINT Point,
                                    const ushort &us_SequenceNumber,
                                    QList<short> &list_Power_db);

    void ClearFWTestData(EMUN_TESTDATAPOINT Point,
                         const ushort &us_SequenceNumber);

private:
    void WorkSleep(ushort un_Msec);

    bool GetFWDataPointer(const ushort &us_SequenceNumber,
                          FWUploadData * &p_FWUploadData);

    bool GetLastFWDataPointer(const ushort &us_SequenceNumber,
                                FWUploadData * &p_FWUploadData);

    bool FWConfirm(STRUCT_COMINFO &struct_ComInfo);

    bool AddFW(const QString &str_PortName,
               const uint &un_Pid,
               const uint &un_Vid);

    bool AddFWUploadDataObject(const ushort &us_SequenceNumber,
                               const QString &str_PortName);

    bool RemoveFW(const QString &str_PortName,
                  const uint &un_Pid,
                  const uint &un_Vid);

    bool RemoveFWUploadDataObject(const ushort &us_SequenceNumber,
                                  const QString &str_PortName);
    bool RemoveAllFWAndFWData();

    bool UpdataLastFWTestData(const ushort &us_SequenceNumber);

    bool TransformMaskCode(const ushort &us_SequenceNumber,
                           const int &n_MaskData,
                           QList<int> &list_MaskCode);

    bool ReplaceUsbControlResult(const ushort &us_SequenceNumber,
                                 QMap<int,bool> &map_FromResultData,
                                 QMap<int,bool> &map_ToResultData);

signals:
    void sig_EnumUsbComplete();

    void sig_SendPowerTestComplete();

    void sig_FirmwareDiscoverd();

    void sig_FirmwareRemove();

    void sig_ReceiveCmd(ushort us_SequenceNumber,
                        uchar uc_Command);

    void sig_WriteCmd(ushort us_SequenceNumber,
                      uchar uc_Command);

    void sig_GetFWInfo(ushort us_SequenceNumber);

    void sig_TestTimeOut(ushort us_SequenceNumber);

    void sig_MachineState(ushort us_SequenceNumber);

    void sig_AdjustPower(ushort us_SequenceNumber);

    void sig_ReadPower(ushort us_SequenceNumber);

    void sig_PowerSelfTest(ushort us_SequenceNumber);

    void sig_FWBoot(ushort us_SequenceNumber);

    void sig_FWIntact(ushort us_SequenceNumber);

    void sig_HandDataBag(ushort us_SequenceNumber);

    void sig_StartTestNotice(ushort us_SequenceNumber);

    void sig_CompleteTest(ushort us_SequenceNumber);

    void sig_StartOneGroupPowerTest(ushort us_SequenceNumber);

    void sig_StartOneGroupEnumTest(ushort us_SequenceNumber);

    void sig_UploadRFPowerResult(ushort us_SequenceNumber);

public slots:
    void slot_EnumUsbComplete();

    void slot_SendPowerTestComplete();

    void slot_FirmwareDiscoverd(QString str_Port,
                                uint un_Pid,
                                uint un_Vid);

    void slot_FirmwareRemove(QString str_Port,
                             uint un_Pid,
                             uint un_Vid);

    void slot_ReceiveCommand(ushort us_SequenceNumber,
                             uchar uc_Command,
                             QByteArray byte_Data,
                             uint un_DataLength);

    void slot_WriteCommand(ushort us_SequenceNumber,
                           uchar uc_Command,
                           QByteArray byte_Data,
                           uint un_DataLength);

    void slot_GetFWInfo(ushort us_SequenceNumber,
                        STRUCT_FWINFO struct_FWInfo);

    void slot_TestTimeOut(ushort us_SequenceNumber,
                          int n_TestTimeOut);

    void slot_MachineState(ushort us_SequenceNumber,
                           ENUM_MACHINEDUTSTATE DUTState,
                           ENUM_MACHINETESTSTATE TestState);

    void slot_AdjustPower(ushort us_SequenceNumber,
                          short s_Vslop,
                          short s_VslopAdc,
                          short s_Intercept);

    void slot_ReadPower(ushort us_SequenceNumber,
                        QList<short> list_Vslop,
                        QList<short> list_VslopAdc,
                        QList<short> list_Intercept,
                        QList<char> list_db_Compensation,
                        short s_ReferenceVoltage_1,
                        short s_ReferenceVoltage_2);

    void slot_PowerSelfTest(ushort us_SequenceNumber,
                            QList<short> list_SelfVoltage);

    void slot_FWBoot(ushort us_SequenceNumber,
                     STRUCT_FWINFO struct_FWInfo);

    void slot_FWIntact(ushort us_SequenceNumber,
                       ENUM_MSGDEFINE FWIntact);

    void slot_HandDataBag(ushort us_SequenceNumber,
                          STRUCT_HANDBAG struct_HandBag);

    void slot_StartTestNotice(ushort us_SequenceNumber,
                              char c_TestType,
                              char c_TotalGroup,
                              char c_AmoutOfAGroup,
                              QList<char> list_UsefulDUT);

    void slot_CompleteTest(ushort us_SequenceNumber,
                           QList<char> list_UsefulResult);

    void slot_StartOneGroupPowerTest(ushort us_SequenceNumber,
                        char c_SurplusGroup,
                        int n_TestDUTMask);

    void slot_StartOneGroupEnumTest(ushort us_SequenceNumber,
                                    char c_SurplusGroup,
                                    int n_TestDUTMask);

    void slot_UploadRFPowerResult(ushort us_SequenceNumber,
                                  QList<short> list_Power_db);

private slots:

private:
    ComDiscoverd *m_pComDiscoverd;
    QList<Firmware *> m_listFirmware;
    UsbControl *m_pUsbControl;
    QThread *m_pQThread;

    QList<STRUCT_COMINFO> m_listNeedFW;
    QList<STRUCT_COMINFO> m_listFindCom;

    QList<FWUploadData *> m_listFWUploadData;
    QMap<int,bool> m_mapEnumResult;
    QMap<int,bool> m_mapOpenDeviceResult;
    QMap<int, bool> m_mapSendCmdResult;

    QList<FWUploadData *> m_listLastFWUploadData;
    QMap<int,bool> m_mapLastEnumResult;
    QMap<int,bool> m_mapLastOpenDeviceResult;
    QMap<int, bool> m_mapLastSendCmdResult;

    ENUM_OPENMODEL m_OpenFWModel;

    bool m_bManuallyCreatUsbControl;

    QList<STRUCT_COMMANDANDDATA> m_listReceiveCommandAndData;
    QList<STRUCT_COMMANDANDDATA> m_listWriteCommandAndData;
};

#endif // DEVICEOBSERVER_H
