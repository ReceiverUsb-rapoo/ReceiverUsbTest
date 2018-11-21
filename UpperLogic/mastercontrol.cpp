#include "mastercontrol.h"
#include <QCoreApplication>
#include <QEventLoop>

MasterControl::MasterControl(QObject *parent)
    :QObject(parent)
{
    m_pDeviceObserver = NULL;
    m_pDeviceOperator = NULL;
    m_bEnumTestWorkState = false;
    m_bPowerTestWorkState = false;

    InitMasterControl();
}

MasterControl::~MasterControl()
{


}

bool MasterControl::InitTest()
{     
    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);
    m_bFirstTest = true;

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    ConnectFuntion();

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        m_pDeviceOperator->InitDUTTest(list_SequenceNumber.at(i));
        OpenBox(list_SequenceNumber.at(i));
    }

    InitUsbEnumAndSendTest();
    return true;
}

bool MasterControl::StartOneTest()
{   
    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        if(m_bBoxSwitchClose){
            m_oStartTest_MsgQueue.PushFront(list_SequenceNumber.at(i));

            emit sig_StartTest(i);
//            m_pDeviceOperator->StartOneTest(list_SequenceNumber.at(i));
        }

        if(m_bRobotSwitchClose){
            CloseBox(list_SequenceNumber.at(i));
        }
        else{
            SendCatchRobotAction(list_SequenceNumber.at(i), CatchRobot_Put);
        }
    }

    m_bAutoTest = false;
    m_unStartTimes ++;
    return true;
}

bool MasterControl::StartAutoTest()
{  
    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        if(m_bBoxSwitchClose){
            m_oStartTest_MsgQueue.PushFront(list_SequenceNumber.at(i));

            emit sig_StartTest(i);
//            m_pDeviceOperator->StartOneTest(list_SequenceNumber.at(i));
        }

        if(m_bRobotSwitchClose){
            CloseBox(list_SequenceNumber.at(i));
        }
        else{
            SendCatchRobotAction(list_SequenceNumber.at(i), CatchRobot_Put);
        }
    }

    m_bAutoTest = true;
    m_unStartTimes ++;

    return true;
}

bool MasterControl::StopTest()
{
    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    DisconnctFuntion();

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        m_pDeviceOperator->ExitDUTTest(list_SequenceNumber.at(i));
    }

    ExitUsbEnumAndSendTest();

    return true;
}

void MasterControl::GetAllFWSequenceNumber(QList<ushort> &list_SequenceNumber)
{
    m_pDeviceObserver->GetAllFWSequenceNumber(list_SequenceNumber);
}

void MasterControl::GetAllBoxSequenceNumber(QList<ushort> &list_SequenceNumber)
{
    m_pDeviceObserver->GetAllBoxSequenceNumber(list_SequenceNumber);
}

bool MasterControl::InitMasterControl()
{
    m_pDeviceObserver = m_oDeviceObserverInstanceGetter.GetInstance();
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();
    m_pCountTestData = m_oCountTestResultInstanceGetter.GetInstance();

    ConnectAllPath();

    SetEquitmentConfig();

    UpdataFWDevice();

    UpdataBox();

    UpdataCatchRobot();

    UpdataSupplementRobot();

    return true;
}

bool MasterControl::SetEquitmentConfig()
{
    ConfigFile o_ConfigFile;
    STRUCT_EQUITMENTCONFIG struct_EquitmentConfig;
    o_ConfigFile.GetEquitmentConfig(struct_EquitmentConfig);

    QList<STRUCT_COMINFO> list_FWComInfo;
    STRUCT_COMINFO struct_ComInfo;

    for(int i = 0; i < struct_EquitmentConfig.list_ComName.count(); i++){
        struct_ComInfo.str_PortName = struct_EquitmentConfig.list_ComName.at(i);
        struct_ComInfo.un_Pid = struct_EquitmentConfig.un_ComPid;
        struct_ComInfo.un_Vid = struct_EquitmentConfig.un_ComVid;

        list_FWComInfo.append(struct_ComInfo);
    }

    if(struct_EquitmentConfig.n_OpemWithComName == 1){
        m_pDeviceObserver->SetOpenFWModelByCom();
    }
    else if(struct_EquitmentConfig.n_OpemWithPidVid == 1){
        m_pDeviceObserver->SetOpenFWModelByPidVid();
    }
    else{
        m_pDeviceObserver->SetOpenFWModelByCom();
    }

    m_pDeviceObserver->SetNeedFWConfig(list_FWComInfo);

    m_pDeviceObserver->SetBoxIP(struct_EquitmentConfig.list_BoxIP);

    m_pDeviceObserver->SetCatchRobotIP(struct_EquitmentConfig.list_RobotIP.value(0));

    m_pDeviceObserver->SetSupplementRobotIP(struct_EquitmentConfig.list_RobotIP.value(1));

    m_pDeviceObserver->SetLocalTcpServer(struct_EquitmentConfig.str_ComputerIP,
                                         struct_EquitmentConfig.us_ComputerPort);

    m_bBoxSwitchClose = (bool)struct_EquitmentConfig.n_BoxUnuse;
    m_bRobotSwitchClose = (bool)struct_EquitmentConfig.n_RobotUnuse;

    if(m_bBoxSwitchClose){
        m_bRobotSwitchClose = m_bBoxSwitchClose;
    }

    return true;
}

bool MasterControl::UpdataFWDevice()
{
    QList<Firmware *> list_FWObjectPointer;
    m_pDeviceObserver->GetFWObjectPointer(list_FWObjectPointer);
    m_pDeviceOperator->SetFWObjectPointer(list_FWObjectPointer);

    SetAllFWPCConfig();

    SetAllUsbControlConfig();

    return true;
}

bool MasterControl::UpdataBox()
{
    QList<Box *> list_Box;
    m_pDeviceObserver->GetBoxObjectPoint(list_Box);
    m_pDeviceOperator->SetBoxObjectPoint(list_Box);

    return true;
}

bool MasterControl::UpdataCatchRobot()
{
    CatchRobot *p_CatchRobot;
    m_pDeviceObserver->GetCatchRobotObjectPoint(p_CatchRobot);
    m_pDeviceOperator->SetCatchRobotObjectPoint(p_CatchRobot);

    return true;
}

bool MasterControl::UpdataSupplementRobot()
{
    SupplementRoobot *p_SupplementRoobot;
    m_pDeviceObserver->GetSupplementRobotObjectPoint(p_SupplementRoobot);
    m_pDeviceOperator->SetSupplementRobotObjectPoint(p_SupplementRoobot);

    return true;
}

bool MasterControl::SetAllFWPCConfig()
{
    ConfigFile o_ConfigFile;
    STRUCT_PCTESTCONFIG struct_PCTestConfig;

    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        o_ConfigFile.GetPCTestConfig(list_SequenceNumber.at(i), struct_PCTestConfig);
        m_pDeviceOperator->SetFWPCConfig(list_SequenceNumber.at(i), struct_PCTestConfig);
        m_mapRetestSwitch.insert(list_SequenceNumber.at(i), (bool)struct_PCTestConfig.uc_RetestTimes);
    }
    return true;
}

bool MasterControl::SetAllUsbControlConfig()
{
    ConfigFile o_ConfigFile;
    STRUCT_USBCONTROLCONFIG struct_UsbControlConfig;
    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        o_ConfigFile.GetUsbControlConfig(list_SequenceNumber.at(i), struct_UsbControlConfig);
        m_mapUsbControlConfig.insert(list_SequenceNumber.at(i), struct_UsbControlConfig);
    }
    return true;
}

bool MasterControl::ConnectAllPath()
{
    if(m_pDeviceObserver == NULL){
        return false;
    }

    connect(m_pDeviceObserver, SIGNAL(sig_FirmwareDiscoverd()),
            this, SLOT(slot_FirmwareDiscoverd()));
    connect(m_pDeviceObserver, SIGNAL(sig_FirmwareRemove()),
            this, SLOT(slot_FirmwareRemove()));

    connect(m_pDeviceObserver, SIGNAL(sig_BoxDiscoverd()),
            this, SLOT(slot_BoxDiscoverd()));
    connect(m_pDeviceObserver, SIGNAL(sig_BoxRemove()),
            this, SLOT(slot_BoxRemove()));

    connect(m_pDeviceObserver, SIGNAL(sig_CatchRobotDiscoverd()),
            this, SLOT(slot_CatchRobotDiscoverd()));
    connect(m_pDeviceObserver, SIGNAL(sig_CatchRobotRemove()),
            this, SLOT(slot_CatchRobotRemove()));

    connect(m_pDeviceObserver,SIGNAL(sig_SupplementRobotDiscoverd()),
            this,  SLOT(slot_SupplementRobotDiscoverd()));
    connect(m_pDeviceObserver,  SIGNAL(sig_SupplementRobotRemove()),
            this, SLOT(slot_SupplementRobotRemove()));


    return true;
}

void MasterControl::ConnectFuntion()
{
    connect(m_pDeviceObserver, SIGNAL(sig_BoxOperatorUpdata(ushort)),
            this, SLOT(slot_BoxOperatorUpdata(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_CatchRobotGetActionUpdata(ushort)),
            this, SLOT(slot_CatchRobotGetActionUpdata(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_SupplementRobotGetRequestUpdata(ushort)),
            this, SLOT(slot_SupplementRobotGetRequestUpdata(ushort)));

    connect(m_pDeviceObserver, SIGNAL(sig_EnumUsbComplete()),
            this, SLOT(slot_EnumUsbComplete()));
    connect(m_pDeviceObserver, SIGNAL(sig_SendPowerTestComplete()),
            this, SLOT(slot_SendPowerTestComplete()));
    connect(m_pDeviceObserver, SIGNAL(sig_StartTestNotice(ushort)),
            this, SLOT(slot_StartTestNotice(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupEnumTest(ushort)),
            this, SLOT(slot_StartOneGroupEnumTest(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupPowerTest(ushort)),
            this, SLOT(slot_StartOneGroupPowerTest(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_CompleteTest(ushort)),
            this, SLOT(slot_CompleteTest(ushort)));
}

void MasterControl::DisconnctFuntion()
{
    disconnect(m_pDeviceObserver, SIGNAL(sig_BoxOperatorUpdata(ushort)),
               this, SLOT(slot_BoxOperatorUpdata(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_CatchRobotGetActionUpdata(ushort)),
               this, SLOT(slot_CatchRobotGetActionUpdata(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_SupplementRobotGetRequestUpdata(ushort)),
               this, SLOT(slot_SupplementRobotGetRequestUpdata(ushort)));

    disconnect(m_pDeviceObserver, SIGNAL(sig_EnumUsbComplete()),
               this, SLOT(slot_EnumUsbComplete()));
    disconnect(m_pDeviceObserver, SIGNAL(sig_SendPowerTestComplete()),
               this, SLOT(slot_SendPowerTestComplete()));
    disconnect(m_pDeviceObserver, SIGNAL(sig_StartTestNotice(ushort)),
               this, SLOT(slot_StartTestNotice(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupEnumTest(ushort)),
               this, SLOT(slot_StartOneGroupEnumTest(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupPowerTest(ushort)),
               this, SLOT(slot_StartOneGroupPowerTest(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_CompleteTest(ushort)),
               this, SLOT(slot_CompleteTest(ushort)));
}

bool MasterControl::DisConnectAllPath()
{  
//    disconnect(m_pDeviceObserver, SIGNAL(sig_FirmwareDiscoverd()),
//               this, SLOT(slot_FirmwareDiscoverd()));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_FirmwareRemove()),
//               this, SLOT(slot_FirmwareRemove()));

//    disconnect(m_pDeviceObserver, SIGNAL(sig_BoxDiscoverd()),
//               this, SLOT(slot_BoxDiscoverd()));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_BoxRemove()),
//               this, SLOT(slot_BoxRemove()));

//    disconnect(m_pDeviceObserver, SIGNAL(sig_CatchRobotDiscoverd()),
//               this, SLOT(slot_CatchRobotDiscoverd()));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_CatchRobotRemove()),
//               this, SLOT(slot_CatchRobotRemove()));

//    disconnect(m_pDeviceObserver, SIGNAL(sig_BoxOperatorUpdata(ushort)),
//               this, SLOT(slot_BoxOperatorUpdata(ushort)));

//    disconnect(m_pDeviceObserver, SIGNAL(sig_EnumUsbComplete()),
//               this, SLOT(slot_EnumUsbComplete()));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_SendPowerTestComplete()),
//               this, SLOT(slot_SendPowerTestComplete()));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_StartTestNotice(ushort)),
//               this, SLOT(slot_StartTestNotice(ushort)));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupEnumTest(ushort)),
//               this, SLOT(slot_StartOneGroupEnumTest(ushort)));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupPowerTest(ushort)),
//               this, SLOT(slot_StartOneGroupPowerTest(ushort)));
//    disconnect(m_pDeviceObserver, SIGNAL(sig_CompleteTest(ushort)),
//               this, SLOT(slot_CompleteTest(ushort)));

    return true;
}

bool MasterControl::InitUsbEnumAndSendTest()
{
    UsbControl *p_UsbControl = NULL;
    if(!m_pDeviceOperator->CreatUsbControlObject()){
        m_pDeviceObserver->RemoveUsbControlObjectPoint();
        m_pDeviceOperator->ExitUsbTest();
        m_pDeviceOperator->DeleteUsbControlObject();
        m_pDeviceOperator->CreatUsbControlObject();
    }
    m_pDeviceOperator->GetUsbControlObjectPointer(p_UsbControl);
    m_pDeviceObserver->SetUsbControlObjectPointer(p_UsbControl);

    return true;
}

bool MasterControl::ExitUsbEnumAndSendTest()
{
    bool b_Ret1 = m_pDeviceObserver->RemoveUsbControlObjectPoint();
    bool b_Ret2 = m_pDeviceOperator->ExitUsbTest();
    bool b_Ret3 = m_pDeviceOperator->DeleteUsbControlObject();

    if(b_Ret1 &&
            b_Ret2&&
            b_Ret3){
        return true;
    }

    return false;
}

bool MasterControl::StartUsbEnumTestByOneGroup(const ushort &us_SequenceNumber,
                                               const QList<int> &list_OneGroupPowerTest)
{
    QMap<int,int> map_StationPort;

    for(int i = 0; i< list_OneGroupPowerTest.count(); i++){
        if(list_OneGroupPowerTest.at(i) != 0){
            map_StationPort.insert(list_OneGroupPowerTest.at(i),
                                   m_mapUsbControlConfig.
                                   value(us_SequenceNumber).
                                   map_StationPort.
                                   value(list_OneGroupPowerTest.at(i)));
        }
    }

    m_pDeviceOperator->SetEnumTestUsbConfig(m_mapUsbControlConfig.value(us_SequenceNumber).un_Pid,
                                            m_mapUsbControlConfig.value(us_SequenceNumber).un_Vid,
                                            map_StationPort.count(),
                                            m_mapUsbControlConfig.value(us_SequenceNumber).n_Time,
                                            map_StationPort);

    return m_pDeviceOperator->StartUsbEnumTest();
}

bool MasterControl::StartUsbPowerTestByOneGroup(const ushort &us_SequenceNumber,
                                                const QList<int> &list_OneGroupSendTest)
{
    QMap<int,int> map_StationPort;

    for(int i = 0; i < list_OneGroupSendTest.count(); i++){
        if(list_OneGroupSendTest.at(i) != 0){
            map_StationPort.insert(list_OneGroupSendTest.at(i),
                                   m_mapUsbControlConfig.
                                   value(us_SequenceNumber).
                                   map_StationPort.
                                   value(list_OneGroupSendTest.at(i)));
        }
    }

    m_pDeviceOperator->SetPowerTestConfig(map_StationPort);
    return m_pDeviceOperator->StartSendPowerTest();
}

bool MasterControl::OpenBox(const ushort &us_SequenceNumber)
{
    if(m_bBoxSwitchClose)
        return false;
    return m_pDeviceOperator->OpenBox(us_SequenceNumber);
}

bool MasterControl::CloseBox(const ushort &us_SequenceNumber)
{
    if(m_bBoxSwitchClose)
        return false;
    return m_pDeviceOperator->CloseBox(us_SequenceNumber);
}

bool MasterControl::SendCatchRobotAction(const ushort &us_FWStation,
                                         const QString &str_RobotAction)
{
    if(m_bRobotSwitchClose)
        return false;
    return m_pDeviceOperator->SendCatchRobotAction(us_FWStation,str_RobotAction);
}

bool MasterControl::SendSupplementRobotData(const ushort &us_FWStation,
                                            const QString &str_Data)
{
    if(m_bRobotSwitchClose)
        return false;
    return m_pDeviceOperator->SendSupplementRobotData(us_FWStation, str_Data);
}

void MasterControl::WaitUsbEnumTestFinish()
{
    QTime o_DieTime = QTime::currentTime().addMSecs(7000);
    while(QTime::currentTime() < o_DieTime  || !m_bEnumTestWorkState){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void MasterControl::WaitUsbPowerTestFinish()
{
    QTime o_DieTime = QTime::currentTime().addMSecs(2000);
    while(QTime::currentTime() < o_DieTime || !m_bPowerTestWorkState){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void MasterControl::WorkSleep(uint un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void MasterControl::slot_FirmwareDiscoverd()
{
    UpdataFWDevice();

    emit sig_FWDiscoverd();
}

void MasterControl::slot_FirmwareRemove()
{
    UpdataFWDevice();

    emit sig_FWRemove();
}

void MasterControl::slot_BoxDiscoverd()
{
    UpdataBox();

    emit sig_BoxDiscoverd();
}

void MasterControl::slot_BoxRemove()
{
    UpdataBox();

    emit sig_BoxRemove();
}

void MasterControl::slot_CatchRobotDiscoverd()
{
    UpdataCatchRobot();

    emit sig_CRobotDiscoverd();
}

void MasterControl::slot_CatchRobotRemove()
{
    UpdataCatchRobot();

    emit sig_CRobotRemove();
}

void MasterControl::slot_SupplementRobotDiscoverd()
{
    UpdataSupplementRobot();

    emit sig_SRobotDiscoverd();
}

void MasterControl::slot_SupplementRobotRemove()
{
    UpdataSupplementRobot();

    emit sig_SRobotRemove();
}

void MasterControl::slot_BoxOperatorUpdata(ushort us_SequenceNumber)
{
    if(m_bBoxSwitchClose){
        return;
    }

    BOX_OPERATOR box_Operator;

    m_pDeviceObserver->GetBoxOperator(us_SequenceNumber, box_Operator);

    if(box_Operator == OPENBOX_OK){
        if(m_bAutoTest){
            if(!m_bFirstTest){
                SendCatchRobotAction(us_SequenceNumber, CatchRobot_Get);
            }
        }
    }
    else if(box_Operator == CLOSEBOX_OK){
        m_oStartTest_MsgQueue.PushBack(us_SequenceNumber);

        emit sig_StartTest(us_SequenceNumber);
//        m_pDeviceOperator->StartOneTest(us_SequenceNumber);
    }
}

void MasterControl::slot_CatchRobotGetActionUpdata(ushort us_SequenceNumber)
{
    if(m_bRobotSwitchClose){
        return;
    }
    QString str_Action;

    m_pDeviceObserver->GetCatchRobotGetAction(us_SequenceNumber, str_Action);

    if(str_Action == CatchRobot_Put_Ok){
        CloseBox(us_SequenceNumber);
    }
    else if(str_Action == CatchRobot_Get_OK){
        SendCatchRobotAction(us_SequenceNumber, CatchRobot_Put);
    }
}

void MasterControl::slot_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber)
{
    if(m_bRobotSwitchClose){
        return;
    }
    QString str_RequestCmd;
    QString str_RequestData;

    m_pDeviceObserver->GetSupplementRobotGetRequest(us_SequenceNumber, str_RequestCmd);

    m_pCountTestData->GetResultData(us_SequenceNumber, str_RequestData);

    SendSupplementRobotData(us_SequenceNumber, str_RequestData);

    //result
}

void MasterControl::slot_EnumUsbComplete()
{
    m_pDeviceOperator->CompleteEnumTest(m_usWorkingSequenceNumber);
}

void MasterControl::slot_SendPowerTestComplete()
{
    m_pDeviceOperator->ExitSendPowerTest();
    m_pDeviceOperator->PCACK_StartOneGroupPowerTest(m_usWorkingSequenceNumber);
}

void MasterControl::slot_StartTestNotice(ushort us_SequenceNumber)
{
    Q_UNUSED(us_SequenceNumber);
 //   m_usWorkingSequenceNumber = us_SequenceNumber;
}

void MasterControl::slot_StartOneGroupEnumTest(ushort us_SequenceNumber)
{
    QList<int> list_OneGroupEnumTestMaskCode;
    char c_SurplusGroup;
    int n_TestDUTMask;

    m_pDeviceObserver->GetStartOneGroupEnumTestData(CURRENT,
                                                    us_SequenceNumber,
                                                    c_SurplusGroup,
                                                    n_TestDUTMask,
                                                    list_OneGroupEnumTestMaskCode);

    StartUsbEnumTestByOneGroup(us_SequenceNumber,list_OneGroupEnumTestMaskCode);
}

void MasterControl::slot_StartOneGroupPowerTest(ushort us_SequenceNumber)
{
    QList<int> list_OneGroupPowerTestMaskCode;
    char c_SurplusGroup;
    int n_TestDUTMask;

    m_pDeviceObserver->GetStartOneGroupPowerTestData(CURRENT,
                                                     us_SequenceNumber,
                                                     c_SurplusGroup,
                                                     n_TestDUTMask,
                                                     list_OneGroupPowerTestMaskCode);

    StartUsbPowerTestByOneGroup(us_SequenceNumber, list_OneGroupPowerTestMaskCode);
}

void MasterControl::slot_CompleteTest(ushort us_SequenceNumber)
{
    QMap<int,bool> map_EnumResult;
    QMap<int,bool> map_OpenDeviceResult;
    QMap<int,bool> map_SendCmdResult;
    QList<short> list_Power_db;
    bool b_Result;
    bool b_RetestSwitch = m_mapRetestSwitch.value(us_SequenceNumber);

    m_pDeviceObserver->GetSendPowerTestCompleteData(CURRENT,
                                                    map_EnumResult,
                                                    map_OpenDeviceResult,
                                                    map_SendCmdResult);

    m_pDeviceObserver->GetUploadRFPowerResultData(CURRENT,
                                                  us_SequenceNumber,
                                                  list_Power_db);


    if(b_RetestSwitch){
        if(!m_bRetested){
            m_pCountTestData->CountOneTestData(us_SequenceNumber,
                                               m_unStartTimes,
                                               map_EnumResult,
                                               map_OpenDeviceResult,
                                               map_SendCmdResult,
                                               list_Power_db);

            emit sig_CompleteTest(us_SequenceNumber);

            m_pCountTestData->GetResult(us_SequenceNumber,
                                        b_Result);

            if(!b_Result){
                m_oStartTest_MsgQueue.PushFront(us_SequenceNumber);
                m_oStartTest_MsgQueue.CompleteOneTest();
                emit sig_StartTest(us_SequenceNumber);
//                m_pDeviceOperator->StartOneTest(us_SequenceNumber);
                m_bRetested = true;
            }
            else{
                if(m_bBoxSwitchClose){
                    if(m_bAutoTest){
                        m_oStartTest_MsgQueue.PushFront(us_SequenceNumber);
                        m_oStartTest_MsgQueue.CompleteOneTest();

                        WorkSleep(1000);
                        emit sig_StartTest(us_SequenceNumber);
//                        m_pDeviceOperator->StartOneTest(us_SequenceNumber);
                    }
                }
                else{
                    m_oStartTest_MsgQueue.CompleteOneTest();
                    OpenBox(us_SequenceNumber);
                }
            }
        }
        else{
            QMap<int,bool> map_LastEnumResult;
            QMap<int,bool> map_LastOpenDeviceResult;
            QMap<int,bool> map_LastSendCmdResult;
            QList<short> list_LastPower_db;

            m_pDeviceObserver->GetSendPowerTestCompleteData(LAST,
                                                            map_LastEnumResult,
                                                            map_LastOpenDeviceResult,
                                                            map_LastSendCmdResult);

            m_pDeviceObserver->GetUploadRFPowerResultData(LAST,
                                                          us_SequenceNumber,
                                                          list_LastPower_db);

            m_pCountTestData->CountRetestData(us_SequenceNumber,
                                              m_unStartTimes,
                                              map_EnumResult,
                                              map_OpenDeviceResult,
                                              map_SendCmdResult,
                                              list_Power_db,
                                              map_LastEnumResult,
                                              map_LastOpenDeviceResult,
                                              map_LastSendCmdResult,
                                              list_LastPower_db);

            emit sig_CompleteTest(us_SequenceNumber);

            if(m_bBoxSwitchClose){
                if(m_bAutoTest){
                    m_oStartTest_MsgQueue.PushBack(us_SequenceNumber);
                    m_oStartTest_MsgQueue.CompleteOneTest();

                    WorkSleep(1000);
                    emit sig_StartTest(us_SequenceNumber);
//                    m_pDeviceOperator->StartOneTest(us_SequenceNumber);
                }
            }
            else{
                m_oStartTest_MsgQueue.CompleteOneTest();
                OpenBox(us_SequenceNumber);
            }
            m_bRetested = false;
        }
    }
    else{
        m_pCountTestData->CountOneTestData(us_SequenceNumber,
                                           m_unStartTimes,
                                           map_EnumResult,
                                           map_OpenDeviceResult,
                                           map_SendCmdResult,
                                           list_Power_db);

        emit sig_CompleteTest(us_SequenceNumber);

        if(m_bBoxSwitchClose){
            if(m_bAutoTest){
                m_oStartTest_MsgQueue.PushBack(us_SequenceNumber);
                m_oStartTest_MsgQueue.CompleteOneTest();

                WorkSleep(1000);
                emit sig_StartTest(us_SequenceNumber);
//                m_pDeviceOperator->StartOneTest(us_SequenceNumber);
            }
        }
        else{
            m_oStartTest_MsgQueue.CompleteOneTest();
            OpenBox(us_SequenceNumber);
        }
    }
}

