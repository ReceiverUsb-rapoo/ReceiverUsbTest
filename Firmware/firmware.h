#ifndef FIRMWARE_H
#define FIRMWARE_H
#include <QTimer>
#include <QMap>
#include <QTime>
#include "firmwarecom.h"
#include "firmware_define.h"

class Firmware :public QObject
{
    Q_OBJECT
public:
    explicit Firmware(QObject *parent = 0);
    ~Firmware();

    bool InitFirmware();

    bool ExitFirmware();

    bool GetSequenceNumber(ushort &us_SequenceNumber);

    bool SetFirmwareConfig(const QString &str_Com,
                           const uint &un_Pid,
                           const uint &un_Vid);

    bool GetFirmwareConfig(QString &str_Com,
                           uint &un_Pid,
                           uint &un_Vid);

    bool SetOpenComModelByCom();

    bool SetOpenComModelByPidVid();

    bool SetPCConfig(STRUCT_PCTESTCONFIG &struct_PCConfig);

    bool OpenFirmware();

    bool CloseFirmware();

public:
    //pc send
    bool PC_GetFWInfo();

    bool PC_RestartFW();

    bool PC_InitDUTTest();

    bool PC_ExitDUTTest();

    bool PC_StartOneTest();

    bool PC_InquireMachineState();

    bool PC_CompleteEnumTest();

    bool PC_EnterDebugModel();

    bool PC_ExitDebugModel();

    bool PC_StartWholeDUTTest();

    bool PC_AdjustPower(char c_Funtion = 0x00,
                        short s_ReferenceVoltage_1 = 0x09c4,
                        short s_ReferenceVoltage_2 = 0x09c4,
                        uchar uc_Station = 0x01,
                        char c_AdjustPoint = 0x32,
                        char c_dp_Compensation = 0x00);

    bool PC_ReadPowerData();

    bool PC_ClearPowerConfig(const int &n_Station);

    bool PC_TestPowerSelfTest();

    bool PC_TestEnum(const ENUM_POWERONGROUP &Group,
                     const short &s_Time);

    bool PC_PowerOnSwitch(const ENUM_POWERONSWITCH &Switch,
                          const ENUM_POWERONGROUP &Group);

private:
    void WorkSleep(ushort un_Msec);

    bool SendCommandData_ShortTimeOut(ENUM_PCCOMMAND PCCommand,
                                      char *p_cData,
                                      uint un_DataLength);

    bool SendCommandData_LongtTimeOut(ENUM_PCCOMMAND PCCommand,
                                      char *p_cData,
                                      uint un_DataLength);


    bool SendCommandData(ENUM_PCCOMMAND PCCommand,
                         char *p_cData,
                         uint un_DataLength);

    bool FWACKRemoveTimeOut(ENUM_FIRMWARECOMMAND FWCommand);

//    bool AnalyzeDataArray_1_Byte(ENUM_FIRMWARECOMMAND FWCommand,
//                            char *p_cData,
//                            uint &un_DataLength);

//    bool AnalyzeDataArray_2_Byte(ENUM_FIRMWARECOMMAND FWCommand,
//                            char *p_cData,
//                            uint &un_DataLength);

    bool FWCMDControl(uchar &uc_Command,
                      QByteArray &byte_Data,
                      uint &un_DataLength);

private:
    //pc send to fw
    bool PCACK_boot();

    bool PCACK_FirmwareIntact(ENUM_MSGDEFINE FWState);

    bool PCACK_SelectFirmware();

    bool PCACK_StartTest();

    bool PCACK_CompleteTest();

public:
    bool PCACK_StartOneGroupPowerTest();

private:
    bool PCACK_StartOneGroupEnumTesT();

    bool PCACK_UploadRFPowerResult();

    //pc receive from fw cmd
    bool FW_boot(char *p_cData,
                 const uint &un_DataLength);

    bool FW_FirmwareIntact(char *p_cData,
                           const uint &un_DataLength);

    bool FW_SelectFirmware();

    bool FW_HandShake(char *p_cData,
                      const uint &un_DataLength);

    bool FW_StartTest(char *p_cData,
                      const uint &un_DataLength);

    bool FW_CompleteTest(char *p_cData,
                         const uint &un_DataLength);

    bool FW_StartOneGroupPowerTest(char *p_cData,
                                   const uint &un_DataLength);

    bool FW_StartOneGroupEnumTesT(char *p_cData,
                                  const uint &un_DataLength);

    bool FW_UploadRFPowerResult(char *p_cData,
                                const uint &un_DataLength);

    //pc receive from fw ack
    bool FWACK_GetFWInfo(char *p_cData,
                         const uint &un_DataLength);

    bool FWACK_RestartFW();

    bool FWACK_InitDUTTest();

    bool FWACK_ExitDUTTest();

    bool FWACK_StartOneTest(char *p_cData,
                            const uint &un_DataLength);

    bool FWACK_InquireMachineState(char *p_cData,
                                   const uint &un_DataLength);

    bool FWACK_CompleteEnumTest();

    bool FWACK_EnterDebugModel();

    bool FWACK_ExitDebugModel();

    bool FWACK_StartWholeDUTTest();

    bool FWACK_AdjustPower(char *p_cData,
                           const uint &un_DataLength);

    bool FWACK_ReadPowerData(char *p_cData,
                             const uint &un_DataLength);

    bool FWACK_ClearPowerConfig();

    bool FWACK_TestPowerSelfTest(char *p_cData,
                                 const uint &un_DataLength);

    bool FWACK_TestEnum();

    bool FWACK_PowerOnSwitch();

signals:
    void sig_ReceiveCommand(ushort us_SequenceNumber,
                            uchar uc_Command,
                            QByteArray byte_Data,
                            uint un_DataLength);

    void sig_WriteCommand(ushort us_SequenceNumber,
                          uchar uc_Command,
                          QByteArray byte_Data,
                          uint un_DataLength);

    void sig_GetFWInfo(ushort us_SequenceNumber,
                       STRUCT_FWINFO struct_FWInfo);

    void sig_TestTimeOut(ushort us_SequenceNumber,
                         int n_TestTimeOut);

    void sig_MachineState(ushort us_SequenceNumber,
                          ENUM_MACHINEDUTSTATE,
                          ENUM_MACHINETESTSTATE);

    void sig_AdjustPower(ushort us_SequenceNumber,
                         short s_Vslop,
                         short s_VslopAdc,
                         short s_Intercept);

    void sig_ReadPower(ushort us_SequenceNumber,
                       QList<short> list_Vslop,
                       QList<short> list_VslopAdc,
                       QList<short> list_Intercept,
                       QList<char> list_db_Compensation,
                       short s_ReferenceVoltage_1,
                       short s_ReferenceVoltage_2);

    void sig_PowerSelfTest(ushort us_SequenceNumber,
                           QList<short> list_SelfVoltage);

    void sig_FWBoot(ushort us_SequenceNumber,
                    STRUCT_FWINFO struct_FWInfo);

    void sig_FWIntact(ushort us_SequenceNumber,
                      ENUM_MSGDEFINE FWIntact);

    void sig_HandDataBag(ushort us_SequenceNumber,
                         STRUCT_HANDBAG struct_HandBag);

    void sig_StartTestNotice(ushort us_SequenceNumber,
                             char c_TestType,
                             char c_TotalGroup,
                             char c_AmoutOfAGroup,
                             QList<char> list_UsefulDUT);

    void sig_CompleteTest(ushort us_SequenceNumber,
                          QList<char> list_UsefulResult);

    void sig_StartOneGroupPowerTest(ushort us_SequenceNumber,
                                    char c_SurplusGroup,
                                    int n_TestDUTMask);

    void sig_StartOneGroupEnumTest(ushort us_SequenceNumber,
                                   char c_SurplusGroup,
                                   int n_TestDUTMask);

    void sig_UploadRFPowerResult(ushort us_SequenceNumber,
                                 QList<short> list_Power_db);

public slots:
    void slot_ReceiveCommand(char c_Command,
                             QByteArray byte_Data,
                             uint un_DataLength);

    void slot_WriteCommand(char c_Command,
                          QByteArray byte_Data,
                          uint un_DataLength);

private slots:
    void slot_ShortTimeOut();
    void slot_LongTimeOut();

private:
    FirmwareCom *m_pFirmwareCom;
    STRUCT_PCTESTCONFIG m_structPCConfig;
    ushort m_usSequenceNumber;

    QTimer *m_pLongQTimer;
    QTimer *m_pShortQTimer;

    QThread *m_pQThread;

    QMap<ENUM_PCCOMMAND, QTime> m_mapPCCommand_ShortTimeOut;
    QMap<ENUM_PCCOMMAND, QTime> m_mapPCCommand_LongTimeOut;

    QMap<ENUM_PCCOMMAND, STRUCT_PCCOMMANDDATA> m_mapPCCommandData;
};

#endif // FIRMWARE_H
