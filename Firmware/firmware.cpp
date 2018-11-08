#include "firmware.h"
#include "outputfirmwareconfig.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QThread>
#include <QDebug>

Firmware::Firmware(QObject *parent)
    : QObject(parent)
{
    m_pFirmwareCom = NULL;
    m_pLongQTimer = NULL;
    m_pShortQTimer = NULL;
    m_pQThread = NULL;

    m_pLongQTimer = new QTimer;
    m_pShortQTimer = new QTimer;

    connect(m_pLongQTimer, SIGNAL(timeout()), this, SLOT(slot_LongTimeOut()));
    connect(m_pShortQTimer, SIGNAL(timeout()), this, SLOT(slot_ShortTimeOut()));

//    m_pQThread = new QThread;
//    this->moveToThread(m_pQThread);
//    m_pQThread->start();

    InitFirmware();
}

Firmware::~Firmware()
{
    if(m_pQThread != NULL){
        m_pQThread->deleteLater();
        m_pQThread = NULL;
    }

    ExitFirmware();
}

bool Firmware::InitFirmware()
{
    m_pFirmwareCom = new FirmwareCom;

    connect(m_pFirmwareCom, SIGNAL(sig_ReceiveCommand(char,QByteArray,uint)),
            this, SLOT(slot_ReceiveCommand(char,QByteArray,uint)));

    connect(m_pFirmwareCom, SIGNAL(sig_WriteCommand(char,QByteArray,uint)),
            this, SLOT(slot_WriteCommand(char,QByteArray,uint)));

    return m_pFirmwareCom->InitFirmwareCom();
}

bool Firmware::ExitFirmware()
{
    if(m_pFirmwareCom != NULL){
        m_pFirmwareCom->ExitFirmwareCom();
        delete m_pFirmwareCom;
        m_pFirmwareCom = NULL;
    }

    if(m_pLongQTimer != NULL){
        if(m_pLongQTimer->isActive()){
            m_pLongQTimer->stop();
        }

        delete m_pLongQTimer;
        m_pLongQTimer = NULL;
    }

    if(m_pShortQTimer != NULL){
        if(m_pShortQTimer->isActive()){
            m_pShortQTimer->stop();
        }

        delete m_pShortQTimer;
        m_pShortQTimer = NULL;
    }

    return true;
}

bool Firmware::GetSequenceNumber(ushort &us_SequenceNumber)
{
    if(m_usSequenceNumber == 0){
        return false;
    }
    us_SequenceNumber = m_usSequenceNumber;
    return true;
}

bool Firmware::SetFirmwareConfig(const QString &str_Com,
                                 const uint &un_Pid,
                                 const uint &un_Vid)
{
    return m_pFirmwareCom->SetFirmwareComConfig(str_Com,
                                                un_Pid,
                                                un_Vid);
}

bool Firmware::GetFirmwareConfig(QString &str_Com,
                                 uint &un_Pid,
                                 uint &un_Vid)
{
    return m_pFirmwareCom->GetFirmwareComComfig(str_Com,
                                                un_Pid,
                                                un_Vid);
}

bool Firmware::SetOpenComModelByCom()
{
    return m_pFirmwareCom->SetOpenComModelByCom();
}

bool Firmware::SetOpenComModelByPidVid()
{
    return m_pFirmwareCom->SetOpenComModelByPidVid();
}

bool Firmware::SetPCConfig(STRUCT_PCTESTCONFIG &struct_PCConfig)
{
    m_structPCConfig = struct_PCConfig;
    return true;
}

bool Firmware::OpenFirmware()
{
    if(!m_pFirmwareCom->OpenFirmwareCom()){
        return false;
    }

//   return true;
    return PC_GetFWInfo();
}

bool Firmware::CloseFirmware()
{
    return m_pFirmwareCom->CloseFirmwareCom();
}

bool Firmware::PC_GetFWInfo()
{
    char cDATA[1] = {0x00};
    return SendCommandData_ShortTimeOut(PC_GETFWINFO,
                                        cDATA,
                                        1);
}

bool Firmware::PC_RestartFW()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_RESTARTFW,
                                        cDATA,
                                        1);
}

bool Firmware::PC_InitDUTTest()
{
    OutPutFirmwareConfig o_OutPutFirmwareConfig;
    char DATA[254];
    uint un_Datalength = 0;

    o_OutPutFirmwareConfig.InPutPCConfig(m_structPCConfig);
    o_OutPutFirmwareConfig.OutPutFWConfig(DATA, un_Datalength);

    char cDATA[254] = {(char)m_structPCConfig.TestType};
    memcpy(cDATA + 1, DATA, un_Datalength);

    return SendCommandData_ShortTimeOut(PC_INITRDUTTEST,
                                        cDATA,
                                        un_Datalength + 1);
}

bool Firmware::PC_ExitDUTTest()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_EXITDUTTEST,
                                        cDATA,
                                        1);
}

bool Firmware::PC_StartOneTest()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_STARTONETEST,
                                        cDATA,
                                        1);
}

bool Firmware::PC_InquireMachineState()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_INQUIREMACHINESTATE,
                                        cDATA,
                                        1);
}

bool Firmware::PC_CompleteEnumTest()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_COMPLETEENUM,
                                        cDATA,
                                        1);
}

bool Firmware::PC_EnterDebugModel()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_ENTERDEBUGMODEL,
                                        cDATA,
                                        1);
}

bool Firmware::PC_ExitDebugModel()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_EXITDEBUGMODLE,
                                        cDATA,
                                        1);
}

bool Firmware::PC_StartWholeDUTTest()
{
    OutPutFirmwareConfig o_OutPutFirmwareConfig;
    char DATA[254];
    uint un_Datalength = 0;


    o_OutPutFirmwareConfig.InPutPCConfig(m_structPCConfig);
    o_OutPutFirmwareConfig.OutPutFWConfig(DATA, un_Datalength);

    char cDATA[254] = {(char)m_structPCConfig.TestType};
    memcpy(cDATA + 1, DATA, un_Datalength);

    return SendCommandData_ShortTimeOut(PC_STARTWHOLEDUTTEST,
                                        cDATA,
                                        un_Datalength + 1);
}

bool Firmware::PC_AdjustPower(char c_Funtion,
                              short s_ReferenceVoltage_1,
                              short s_ReferenceVoltage_2,
                              uchar uc_Station,
                              char c_AdjustPoint,
                              char c_dp_Compensation)
{
    char cDATA[8] = {c_Funtion,
                     (char)0x00,
                     (char)0x00,
                     (char)0x00,
                     (char)0x00,
                     (char)uc_Station,
                     c_AdjustPoint,
                     c_dp_Compensation};

    memcpy(cDATA + 1, &s_ReferenceVoltage_1, 2);
    memcpy(cDATA + 3, &s_ReferenceVoltage_2, 2);

    return SendCommandData_ShortTimeOut(PC_ADJUSTPOWER,
                                        cDATA,
                                        8);
}

bool Firmware::PC_ReadPowerData()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_READPOWERDATA,
                                        cDATA,
                                        1);
}

bool Firmware::PC_ClearPowerConfig(const int &n_Station)
{
    char cDATA[5] = {NORMAL};

    memcpy(cDATA + 1, &n_Station, 4);

    return SendCommandData_ShortTimeOut(PC_CLEARPOWERCONFIG,
                                        cDATA,
                                        5);
}

bool Firmware::PC_TestPowerSelfTest()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData_ShortTimeOut(PC_TESTPOWERSELFTEST,
                                        cDATA,
                                        1);
}

bool Firmware::PC_TestEnum(const ENUM_POWERONGROUP &Group,
                           const short &s_Time)
{
    char cDATA[4] = {NORMAL, (char)Group};

    memcpy(cDATA + 2, &s_Time, 2);

    return SendCommandData_ShortTimeOut(PC_TESTENUM,
                                        cDATA,
                                        4);
}

bool Firmware::PC_PowerOnSwitch(const ENUM_POWERONSWITCH &Switch,
                                const ENUM_POWERONGROUP &Group)
{
    char cDATA[2] = {(char)Switch, (char)Group};

    return SendCommandData_ShortTimeOut(PC_POWERONSWITCH,
                                        cDATA,
                                        2);
}

void Firmware::WorkSleep(ushort un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

bool Firmware::SendCommandData_ShortTimeOut(ENUM_PCCOMMAND PCCommand,
                                            char *p_cData,
                                            uint un_DataLength)
{
    QTime o_QTime = QTime::currentTime().addMSecs(ShortTimeOut);
    m_mapPCCommand_ShortTimeOut.insert(PCCommand, o_QTime);

    STRUCT_PCCOMMANDDATA struct_PCCommandData;
    struct_PCCommandData.PCCommand = PCCommand;
    memcpy(struct_PCCommandData.DATA, p_cData, un_DataLength);
    struct_PCCommandData.un_DataLength = un_DataLength;
    struct_PCCommandData.un_SendTime = 0;


    m_mapPCCommandData.insert(PCCommand, struct_PCCommandData);

    m_pShortQTimer->start(500);

    return SendCommandData(PCCommand,
                           p_cData,
                           un_DataLength);

}

bool Firmware::SendCommandData_LongtTimeOut(ENUM_PCCOMMAND PCCommand,
                                            char *p_cData,
                                            uint un_DataLength)
{
    QTime o_QTime = QTime::currentTime().addMSecs(LongTimeOut);
    m_mapPCCommand_ShortTimeOut.insert(PCCommand, o_QTime);

    STRUCT_PCCOMMANDDATA struct_PCCommandData;
    struct_PCCommandData.PCCommand = PCCommand;
    memcpy(struct_PCCommandData.DATA, p_cData, un_DataLength);
    struct_PCCommandData.un_DataLength = un_DataLength;
    struct_PCCommandData.un_SendTime = 0;

    m_mapPCCommandData.insert(PCCommand, struct_PCCommandData);

    m_pLongQTimer->start(2000);

    return SendCommandData(PCCommand,
                           p_cData,
                           un_DataLength);
}

bool Firmware::SendCommandData(ENUM_PCCOMMAND PCCommand,
                               char *p_cData,
                               uint un_DataLength)
{
    char cCommand[1] = {(char)PCCommand};
    return m_pFirmwareCom->WriteCommandData(cCommand,
                                            p_cData,
                                            un_DataLength);
}

bool Firmware::FWACKRemoveTimeOut(ENUM_FIRMWARECOMMAND FWCommand)
{
    //need add cmd what is long-timg or short-time to divide

    if(FWCommand == FW_BOOT ||
            FWCommand == FW_FIRMWAREINTACT ||
            FWCommand == FW_SELECTFIRMWARE ||
            FWCommand == FW_HANDSHAKE ||
            FWCommand == FW_STARTTEST ||
            FWCommand == FW_COMPLETETEST ||
            FWCommand == FW_STARTONEGROUPPOWTEST ||
            FWCommand == FW_STARTONEGROUPENUMTEST ||
            FWCommand == FW_UPLOADRFPOWERRESYLT ||
            FWCommand == FWACK_GETFWINFO ||
            FWCommand == FWACK_RESTARTFW ||
            FWCommand == FWACK_INITDUTTEST ||
            FWCommand == FWACK_EXITDUTTEST ||
            FWCommand == FWACK_STARTONETEST ||
            FWCommand == FWACK_INQUIREMACHINESTATE ||
            FWCommand == FWACK_COMPLETEENUM ||
            FWCommand == FWACK_ENTERDEBUGMODEL ||
            FWCommand == FWACK_EXITDEBUGMODLE ||
            FWCommand == FWACK_STARTWHOLEDUTTEST ||
            FWCommand == FWACK_ADJUSTPOWER ||
            FWCommand == FWACK_READPOWERDATA ||
            FWCommand == FWACK_CLEARPOWERCONFIG ||
            FWCommand == FWACK_TESTPOWERSELFTEST ||
            FWCommand == FWACK_TESTENUM ||
            FWCommand == FWACK_POWERONSWITCH
            ){
        if(m_mapPCCommand_ShortTimeOut.isEmpty()){
            return false;
        }
    }
    else{
        if(m_mapPCCommand_LongTimeOut.isEmpty()){
            return false;
        }
    }

    QMapIterator<ENUM_PCCOMMAND, QTime>
            map_Iterator_PCCommand_ShortTimeOut(m_mapPCCommand_ShortTimeOut);
    while(map_Iterator_PCCommand_ShortTimeOut.hasNext()){
        map_Iterator_PCCommand_ShortTimeOut.next();

        if(map_Iterator_PCCommand_ShortTimeOut.key() == FWCommand - 0x80){
            m_mapPCCommand_ShortTimeOut.remove(map_Iterator_PCCommand_ShortTimeOut.key());
        }
    }


    QMapIterator<ENUM_PCCOMMAND, QTime>
            map_Iterator_PCCommand_LongTimeOut(m_mapPCCommand_LongTimeOut);
    while(map_Iterator_PCCommand_LongTimeOut.hasNext()){
        map_Iterator_PCCommand_LongTimeOut.next();

        if(map_Iterator_PCCommand_LongTimeOut.key() == FWCommand - 0x80){
            m_mapPCCommand_LongTimeOut.remove(map_Iterator_PCCommand_ShortTimeOut.key());
        }
    }

    return true;
}

bool Firmware::FWCMDControl(uchar &uc_Command,
                            QByteArray &byte_Data,
                            uint &un_DataLength)
{
    switch(uc_Command){
    case FW_BOOT:{
        FW_boot(byte_Data.data(), un_DataLength);
        break;
    }
    case FW_FIRMWAREINTACT:{
        FW_FirmwareIntact(byte_Data.data(), un_DataLength);
        break;
    }
    case FW_SELECTFIRMWARE:{
        FW_SelectFirmware();
        break;
    }
    case FW_HANDSHAKE:{
        FW_HandShake(byte_Data.data(), un_DataLength);
        break;
    }
    case FW_STARTTEST:{
        FW_StartTest(byte_Data.data(), un_DataLength);
        break;
    }
    case FW_COMPLETETEST:{
        FW_CompleteTest(byte_Data.data(), un_DataLength);
        break;
    }
    case FW_STARTONEGROUPPOWTEST:{
        FW_StartOneGroupPowerTest(byte_Data.data(), un_DataLength);
        break;
    }
    case FW_STARTONEGROUPENUMTEST:{
        FW_StartOneGroupEnumTesT(byte_Data.data(), un_DataLength);
        break;
    }
    case FW_UPLOADRFPOWERRESYLT:{
        FW_UploadRFPowerResult(byte_Data.data(), un_DataLength);
        break;
    }
    case FWACK_GETFWINFO:{
        FWACK_GetFWInfo(byte_Data.data(), un_DataLength);
        break;
    }
    case FWACK_RESTARTFW:{
        FWACK_RestartFW();
        break;
    }
    case FWACK_INITDUTTEST:{
        FWACK_InitDUTTest();
        break;
    }
    case FWACK_EXITDUTTEST:{
        FWACK_ExitDUTTest();
        break;
    }
    case FWACK_STARTONETEST:{
        FWACK_StartOneTest(byte_Data.data(), un_DataLength);
        break;
    }
    case FWACK_INQUIREMACHINESTATE:{
        FWACK_InquireMachineState(byte_Data.data(), un_DataLength);
        break;
    }
    case FWACK_COMPLETEENUM:{
        FWACK_CompleteEnumTest();
        break;
    }
    case FWACK_ENTERDEBUGMODEL:{
        FWACK_EnterDebugModel();
        break;
    }
    case FWACK_EXITDEBUGMODLE:{
        FWACK_ExitDebugModel();
        break;
    }
    case FWACK_STARTWHOLEDUTTEST:{
        FWACK_StartWholeDUTTest();
        break;
    }
    case FWACK_ADJUSTPOWER:{
        FWACK_AdjustPower(byte_Data.data(), un_DataLength);
        break;
    }
    case FWACK_READPOWERDATA:{
        FWACK_ReadPowerData(byte_Data.data(), un_DataLength);
        break;
    }
    case FWACK_CLEARPOWERCONFIG:{
        FWACK_ClearPowerConfig();
        break;
    }
    case FWACK_TESTPOWERSELFTEST:{
        FWACK_TestPowerSelfTest(byte_Data.data(), un_DataLength);
        break;
    }
    case FWACK_TESTENUM:{
        FWACK_TestEnum();
        break;
    }
    case FWACK_POWERONSWITCH:{
        FWACK_PowerOnSwitch();
        break;
    }
    default:
        break;
    }

    return true;
}

bool Firmware::PCACK_boot()
{
    char cDATA[1] = {NORMAL};
    return SendCommandData(PCACK_BOOT,
                           cDATA,
                           1);
}

bool Firmware::PCACK_FirmwareIntact(ENUM_MSGDEFINE FWState)
{
    char cDATA[2] = {0x00, 0x00};

    if(FWState == NOFW){
        cDATA[0] = NORMAL;
    }
    else if(FWState == FWDAMAGED){
        cDATA[0] = NORMAL;
    }
    else{
        cDATA[0] = OTHERERROR;
        return SendCommandData(PCACK_FIRMWAREINTACT,
                               cDATA,
                               2);
    }

    return SendCommandData(PCACK_FIRMWAREINTACT,
                           cDATA,
                           2);
}

bool Firmware::PCACK_SelectFirmware()
{
    char cDATA[3] = {NORMAL, 0x00, 0x00};

    return SendCommandData(PCACK_SELECTFIRMWARE,
                           cDATA,
                           3);
}

bool Firmware::PCACK_StartTest()
{
    char cDATA[1] = {NORMAL};

    return SendCommandData(PCACK_STARTTEST,
                           cDATA,
                           1);
}

bool Firmware::PCACK_CompleteTest()
{
    char cDATA[1] = {NORMAL};

    return SendCommandData(PCACK_COMPLETETEST,
                           cDATA,
                           1);
}

bool Firmware::PCACK_StartOneGroupPowerTest()
{
    char cDATA[1] = {NORMAL};

    return SendCommandData(PCACK_POWTEST,
                           cDATA,
                           1);
}

bool Firmware::PCACK_StartOneGroupEnumTesT()
{
    char cDATA[1] = {NORMAL};

    return SendCommandData(PCACK_STARTONEGROUNDTEST,
                           cDATA,
                           1);
}

bool Firmware::PCACK_UploadRFPowerResult()
{
    char cDATA[1] = {NORMAL};

    return SendCommandData(PCACK_UPLOADRFPOWERRESYLT,
                           cDATA,
                           1);
}

bool Firmware::FW_boot(char *p_cData,
                       const uint &un_DataLength)
{
    if(un_DataLength != 23){
        return false;
    }

    STRUCT_FWINFO struct_FWInfo;
    memcpy(&struct_FWInfo, p_cData + 1, 22);

    m_usSequenceNumber = (ushort)struct_FWInfo.uc_FixtureNumber;
    emit sig_FWBoot(m_usSequenceNumber, struct_FWInfo);

    return PCACK_boot();
}

bool Firmware::FW_FirmwareIntact(char *p_cData,
                                 const uint &un_DataLength)
{
    if(un_DataLength != 2){
        return false;
    }

    char cDATA[1] = {0x00};
    memcpy(cDATA, p_cData, 1);

    emit sig_FWIntact(m_usSequenceNumber, (ENUM_MSGDEFINE)cDATA[0]);

    return PCACK_FirmwareIntact((ENUM_MSGDEFINE)cDATA[0]);
}

bool Firmware::FW_SelectFirmware()
{
    return PCACK_SelectFirmware();
}

bool Firmware::FW_HandShake(char *p_cData,
                            const uint &un_DataLength)
{   
    if(un_DataLength != 6){
        return false;
    }

    STRUCT_HANDBAG struct_HandBag;
    memcpy(&struct_HandBag, p_cData, 5);

    emit sig_HandDataBag(m_usSequenceNumber, struct_HandBag);
    return true;
}

bool Firmware::FW_StartTest(char *p_cData,
                            const uint &un_DataLength)
{
//    char cDATA[3] = {};
//    memcpy(cDATA, p_cData + 1, 3);

    if(un_DataLength != 36){
        return false;
    }

    char c_TestType;
    char c_TotalGroup;
    char c_AmoutOfAGroup;
    QList<char> list_UsefulDUT;
    char c_UsefulDUT;

    memcpy(&c_TestType, p_cData + 1, 1);
    memcpy(&c_TotalGroup, p_cData + 2, 1);
    memcpy(&c_AmoutOfAGroup, p_cData + 3, 1);

    for(int i = 0; i < 32; i++){
        memcpy(&c_UsefulDUT, p_cData + i + 4, 1);
        list_UsefulDUT.append(c_UsefulDUT);
    }

    emit sig_StartTestNotice(m_usSequenceNumber,
                             c_TestType,
                             c_TotalGroup,
                             c_AmoutOfAGroup,
                             list_UsefulDUT);
    return PCACK_StartTest();
}

bool Firmware::FW_CompleteTest(char *p_cData,
                               const uint &un_DataLength)
{
    if(un_DataLength != 33){
        return false;
    }

    QList<char> list_UsefulResult;
    char c_UsefulResult;

    for(int i = 0; i < 32; i++){
        memcpy(&c_UsefulResult, p_cData + i + 1, 1);
        list_UsefulResult.append(c_UsefulResult);
    }

    emit sig_CompleteTest(m_usSequenceNumber, list_UsefulResult);

    return PCACK_CompleteTest();
}

bool Firmware::FW_StartOneGroupPowerTest(char *p_cData,
                            const uint &un_DataLength)
{
    if(un_DataLength != 6){
        return false;
    }

    char c_SurplusGroup;
    int n_TestDUTMask;

    memcpy(&c_SurplusGroup, p_cData + 1, 1);
    memcpy(&n_TestDUTMask, p_cData + 2 , 4);

    emit sig_StartOneGroupPowerTest(m_usSequenceNumber,
                       c_SurplusGroup,
                       n_TestDUTMask);

    return PCACK_StartOneGroupPowerTest();
}

bool Firmware::FW_StartOneGroupEnumTesT(char *p_cData,
                                        const uint &un_DataLength)
{
    if(un_DataLength != 6){
        return false;
    }

    char c_SurplusGroup;
    int n_TestDUTMask;

    memcpy(&c_SurplusGroup, p_cData + 1, 1);
    memcpy(&n_TestDUTMask, p_cData + 2 , 4);

    emit sig_StartOneGroupEnumTest(m_usSequenceNumber,
                                   c_SurplusGroup,
                                   n_TestDUTMask);

    return PCACK_StartOneGroupEnumTesT();
}

bool Firmware::FW_UploadRFPowerResult(char *p_cData,
                                      const uint &un_DataLength)
{
    if(un_DataLength != 73){
        return false;
    }

    QList<short> list_Power_db;
    short s_Power_db;

    for(int i = 0; i < 32; i++){
        memcpy(&s_Power_db, p_cData + 2*i + 1, 2);
        list_Power_db.append(s_Power_db);
    }

    emit sig_UploadRFPowerResult(m_usSequenceNumber, list_Power_db);

    return PCACK_UploadRFPowerResult();
}

bool Firmware::FWACK_GetFWInfo(char *p_cData,
                               const uint &un_DataLength)
{
    if(un_DataLength != 24){
        return false;
    }

    if(!FWACKRemoveTimeOut(FWACK_GETFWINFO)){
        return false;
    }

    STRUCT_FWINFO struct_FWInfo;
    memcpy(&struct_FWInfo, p_cData + 2, 22);

    m_usSequenceNumber = (ushort)struct_FWInfo.uc_FixtureNumber;

    emit sig_GetFWInfo(m_usSequenceNumber, struct_FWInfo);

    return true;
}

bool Firmware::FWACK_RestartFW()
{
    return FWACKRemoveTimeOut(FWACK_RESTARTFW);
}

bool Firmware::FWACK_InitDUTTest()
{
    return FWACKRemoveTimeOut(FWACK_INITDUTTEST);
}

bool Firmware::FWACK_ExitDUTTest()
{
    return FWACKRemoveTimeOut(FWACK_EXITDUTTEST);
}

bool Firmware::FWACK_StartOneTest(char *p_cData,
                                  const uint &un_DataLength)
{
    if(un_DataLength != 5){
        return false;
    }

    if(!FWACKRemoveTimeOut(FWACK_STARTONETEST)){
        return false;
    }

    uint un_TestTimeOut = 0;
    memcpy(&un_TestTimeOut, p_cData + 1, 4);

    emit sig_TestTimeOut(m_usSequenceNumber, un_TestTimeOut);

    return true;
}

bool Firmware::FWACK_InquireMachineState(char *p_cData,
                                         const uint &un_DataLength)
{
    if(un_DataLength != 3){
        return false;
    }

    if(!FWACKRemoveTimeOut(FWACK_INQUIREMACHINESTATE)){
        return false;
    }

    uchar uc_DUTState = 0;
    uchar uc_TestState = 0;

    memcpy(&uc_DUTState, p_cData + 1, 1);
    memcpy(&uc_TestState, p_cData + 2, 1);

    emit sig_MachineState(m_usSequenceNumber,
                          (ENUM_MACHINEDUTSTATE)uc_DUTState,
                          (ENUM_MACHINETESTSTATE)uc_TestState);
    return true;
}

bool Firmware::FWACK_CompleteEnumTest()
{
    return FWACKRemoveTimeOut(FWACK_COMPLETEENUM);
}

bool Firmware::FWACK_EnterDebugModel()
{
    return FWACKRemoveTimeOut(FWACK_ENTERDEBUGMODEL);
}

bool Firmware::FWACK_ExitDebugModel()
{
    return FWACKRemoveTimeOut(FWACK_EXITDEBUGMODLE);
}

bool Firmware::FWACK_StartWholeDUTTest()
{
    return FWACKRemoveTimeOut(FWACK_STARTWHOLEDUTTEST);
}

bool Firmware::FWACK_AdjustPower(char *p_cData,
                                 const uint &un_DataLength)
{
    short s_Vslop = 0;
    short s_VslopAdc = 0;
    short s_Intercept = 0;

    if(!FWACKRemoveTimeOut(FWACK_ADJUSTPOWER)){
        return false;
    }

    if(un_DataLength == 2){

        emit sig_AdjustPower(m_usSequenceNumber,
                             s_Vslop,
                             s_VslopAdc,
                             s_Intercept);
        return true;
    }

    if(un_DataLength != 8){
        return false;
    }

    s_Vslop = 1;
    s_VslopAdc = 1;
    s_Intercept = 1;

    memcpy(&s_Vslop, p_cData + 2, 2);
    memcpy(&s_VslopAdc, p_cData + 4, 2);
    memcpy(&s_Intercept, p_cData + 6, 2);

    emit sig_AdjustPower(m_usSequenceNumber,
                         s_Vslop,
                         s_VslopAdc,
                         s_Intercept);
    return true;
}

bool Firmware::FWACK_ReadPowerData(char *p_cData,
                                   const uint &un_DataLength)
{
    if(un_DataLength != 229){
        return false;
    }

    if(!FWACKRemoveTimeOut(FWACK_READPOWERDATA)){
        return false;
    }

    QList<short> list_Vslop;
    QList<short> list_VslopAdc;
    QList<short> list_Intercept;
    QList<char> list_db_Compensation;
    short s_ReferenceVoltage_1;
    short s_ReferenceVoltage_2;

    short s_Vslop = 0;
    short s_VslopAdc = 0;
    short s_Intercept = 0;
    char c_db_Compensation = 0;

    for(int i = 0; i < 32; i++){
        memcpy(&s_Vslop, p_cData + 2*i + 1, 2);
        memcpy(&s_VslopAdc, p_cData + 2*i + 65, 2);
        memcpy(&s_Intercept, p_cData + 2*i + 129, 2);
        memcpy(&c_db_Compensation, p_cData + i + 193, 1);

        list_Vslop.append(s_Vslop);
        list_VslopAdc.append(s_VslopAdc);
        list_Intercept.append(s_Intercept);
        list_db_Compensation.append(c_db_Compensation);
    }

    memcpy(&s_ReferenceVoltage_1, p_cData + 225, 2);
    memcpy(&s_ReferenceVoltage_2 ,p_cData + 227, 2);

    emit sig_ReadPower(m_usSequenceNumber,
                       list_Vslop,
                       list_VslopAdc,
                       list_Intercept,
                       list_db_Compensation,
                       s_ReferenceVoltage_1,
                       s_ReferenceVoltage_2);
    return true;
}

bool Firmware::FWACK_ClearPowerConfig()
{
    return FWACKRemoveTimeOut(FWACK_CLEARPOWERCONFIG);
}

bool Firmware::FWACK_TestPowerSelfTest(char *p_cData,
                                       const uint &un_DataLength)
{
    if(un_DataLength != 65){
        return false;
    }

    if(!FWACKRemoveTimeOut(FWACK_TESTPOWERSELFTEST)){
        return false;
    }

    QList<short> list_SelfVoltage;
    short s_PowerSelfTest = 0;

    for(int i = 0; i < 32; i++){
        memcpy(&s_PowerSelfTest, p_cData + 2*i + 1, 2);
        list_SelfVoltage.append(s_PowerSelfTest);
    }

    emit sig_PowerSelfTest(m_usSequenceNumber, list_SelfVoltage);
    return true;
}

bool Firmware::FWACK_TestEnum()
{
    return FWACKRemoveTimeOut(FWACK_TESTENUM);
}

bool Firmware::FWACK_PowerOnSwitch()
{
    return FWACKRemoveTimeOut(FWACK_POWERONSWITCH);
}

void Firmware::slot_ReceiveCommand(char c_Command,
                                   QByteArray byte_Data,
                                   uint un_DataLength)
{  
    uchar uc_Command = (uchar)c_Command;

    FWCMDControl(uc_Command, byte_Data, un_DataLength);

    emit sig_ReceiveCommand(m_usSequenceNumber,
                            uc_Command,
                            byte_Data,
                            un_DataLength);
}

void Firmware::slot_WriteCommand(char c_Command,
                                 QByteArray byte_Data,
                                 uint un_DataLength)
{
    uchar uc_Command = (uchar)c_Command;
    emit sig_WriteCommand(m_usSequenceNumber,
                          uc_Command,
                          byte_Data,
                          un_DataLength);
}

void Firmware::slot_ShortTimeOut()
{
    if(m_mapPCCommand_ShortTimeOut.isEmpty()){
        m_pShortQTimer->stop();
        return;
    }

    QMapIterator<ENUM_PCCOMMAND, QTime>
            map_Iterator_PCCommand_ShortTimeOut(m_mapPCCommand_ShortTimeOut);

    while(map_Iterator_PCCommand_ShortTimeOut.hasNext()){
        map_Iterator_PCCommand_ShortTimeOut.next();

        if(map_Iterator_PCCommand_ShortTimeOut.value() > QTime::currentTime()){
                if(m_mapPCCommandData.value(
                            map_Iterator_PCCommand_ShortTimeOut.key()).un_SendTime < 3){

                    SendCommandData(m_mapPCCommandData.value(
                                        map_Iterator_PCCommand_ShortTimeOut.key()).PCCommand,
                                    const_cast<char*>(m_mapPCCommandData.value(
                                        map_Iterator_PCCommand_ShortTimeOut.key()).DATA),
                                    m_mapPCCommandData.value(
                                        map_Iterator_PCCommand_ShortTimeOut.key()).un_DataLength);
                    // un_SendTime ++
                    STRUCT_PCCOMMANDDATA STRUCT_PCCommandData;
                    STRUCT_PCCommandData = m_mapPCCommandData.value(
                                map_Iterator_PCCommand_ShortTimeOut.key());

                    STRUCT_PCCommandData.un_SendTime = STRUCT_PCCommandData.un_SendTime + 1;

                    m_mapPCCommandData.insert(map_Iterator_PCCommand_ShortTimeOut.key(),
                                              STRUCT_PCCommandData);

                    m_mapPCCommand_ShortTimeOut.insert(map_Iterator_PCCommand_ShortTimeOut.key(),
                                                       QTime::currentTime().addMSecs(ShortTimeOut));

                }
                else{
                    //error to cmd respond
                }
        }
    }
}

void Firmware::slot_LongTimeOut()
{
    if(m_mapPCCommand_LongTimeOut.isEmpty()){
        m_pLongQTimer->stop();
        return;
    }

    QMapIterator<ENUM_PCCOMMAND, QTime>
            map_Iterator_PCCommand_LongTimeOut(m_mapPCCommand_LongTimeOut);

    while(map_Iterator_PCCommand_LongTimeOut.hasNext()){
        map_Iterator_PCCommand_LongTimeOut.next();

        if(map_Iterator_PCCommand_LongTimeOut.value() > QTime::currentTime()){
                if(m_mapPCCommandData.value(
                            map_Iterator_PCCommand_LongTimeOut.key()).un_SendTime < 3){


                    SendCommandData(m_mapPCCommandData.value(
                                        map_Iterator_PCCommand_LongTimeOut.key()).PCCommand,
                                    const_cast<char*>(m_mapPCCommandData.value(
                                        map_Iterator_PCCommand_LongTimeOut.key()).DATA),
                                    m_mapPCCommandData.value(
                                        map_Iterator_PCCommand_LongTimeOut.key()).un_DataLength);
                    // un_SendTime ++
                    STRUCT_PCCOMMANDDATA STRUCT_PCCommandData;
                    STRUCT_PCCommandData = m_mapPCCommandData.value(
                                map_Iterator_PCCommand_LongTimeOut.key());

                    STRUCT_PCCommandData.un_SendTime = STRUCT_PCCommandData.un_SendTime + 1;

                    m_mapPCCommandData.insert(map_Iterator_PCCommand_LongTimeOut.key(),
                                              STRUCT_PCCommandData);

                    m_mapPCCommand_LongTimeOut.insert(map_Iterator_PCCommand_LongTimeOut.key(),
                                                      QTime::currentTime().addMSecs(ShortTimeOut));
                }
                else{
                    //error to cmd respond
                }
        }
    }
}
