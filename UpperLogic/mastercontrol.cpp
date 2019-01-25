#include "mastercontrol.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QDebug>
#include <DataFile/logfile.h>

MasterControl::MasterControl(QObject *parent)
    :QObject(parent)
{
    m_pDeviceObserver = NULL;
    m_pDeviceOperator = NULL;

    m_bEnumTestNoWorkState = true;
    m_bPowerTestNoWorkState = true;
    m_bTest = false;
    m_unStartTimes = 0;
    m_bCatchWorking = false;
    m_bRequestRobotCmd = true;
    m_bRetested = false;

    m_OpenFWModel = BYCOM;

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

//    InitUsbEnumAndSendTest();
    return true;
}

bool MasterControl::StartOneTest()
{   
    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }
    AppendCRobotCmd(1, CatchRobot_Start);

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        if(m_bBoxSwitchClose){
            emit sig_ReadyTest(list_SequenceNumber.at(i));
            m_oStartTest_MsgQueue.PushFront(list_SequenceNumber.at(i));

//           m_pDeviceOperator->StartOneTest(list_SequenceNumber.at(i));
        }

        if(m_bRobotSwitchClose){
            CloseBox(list_SequenceNumber.at(i));
        }
        else{
            AppendCRobotCmd(1, CatchRobot_Put);
//            SendCatchRobotAction(list_SequenceNumber.at(i), CatchRobot_Put);
        }
    }

    m_bAutoTest = false;
    m_unStartTimes ++;

    m_bTest = true;

    return true;
}

bool MasterControl::StartAutoTest()
{  
    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    AppendCRobotCmd(1, CatchRobot_Start);
    for(int i = 0; i < list_SequenceNumber.count(); i++){
        if(m_bBoxSwitchClose){
            emit sig_ReadyTest(list_SequenceNumber.at(i));
            m_oStartTest_MsgQueue.PushFront(list_SequenceNumber.at(i));

//            m_pDeviceOperator->StartOneTest(list_SequenceNumber.at(i));
        }

        if(m_bRobotSwitchClose){
            CloseBox(list_SequenceNumber.at(i));
        }
        else{
            AppendCRobotCmd(list_SequenceNumber.at(i), CatchRobot_Put);
//            SendCatchRobotAction(list_SequenceNumber.at(i), CatchRobot_Put);
        }
    }

    m_bAutoTest = true;
    m_unStartTimes ++;

    m_bTest = true;

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

    WaitUsbEnumTestFinish();
    WaitUsbPowerTestFinish();

    ExitUsbEnumAndSendTest();

    m_bTest = false;
    m_bAutoTest = false;

    return true;
}

bool MasterControl::Resetting()
{
    if(m_OpenFWModel == BYCOM){
        m_pDeviceObserver->UpdateFW_OpenByCom();
    }

    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        m_pDeviceOperator->RestartFW(list_SequenceNumber.at(i));
    }

    m_pCountTestData->ClearRobotResultData();

    SetEquitmentConfig();

    UpdataFWDevice();

    m_listCRobotCmd.clear();

    m_bCatchWorking = false;

    m_bRequestRobotCmd = true;

    m_mapBoxState.clear();

    return m_oStartTest_MsgQueue.ClearAndResetting();
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

    m_pQTimer = new QTimer(this);
    connect(m_pQTimer, SIGNAL(timeout()),
            this, SLOT(slot_TimeOut_SendCRobotCmd()));
    m_pQTimer->start(500);

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
        m_OpenFWModel = BYCOM;
    }
    else if(struct_EquitmentConfig.n_OpemWithPidVid == 1){
        m_pDeviceObserver->SetOpenFWModelByPidVid();
        m_OpenFWModel = BYPIDVID;
    }
    else{
        m_pDeviceObserver->SetOpenFWModelByCom();
        m_OpenFWModel = BYCOM;
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
    QList<int> list_RFPowerDBUpperLimit;
    QList<int> list_RFPowerDBLowerLimit;
    QList<int> list_DUTFWPosition;

    QList<ushort> list_SequenceNumber;
    GetAllFWSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return false;
    }

    for(int i = 0; i < list_SequenceNumber.count(); i++){
        o_ConfigFile.GetPCTestConfig(list_SequenceNumber.at(i), struct_PCTestConfig);
        m_pDeviceOperator->SetFWPCConfig(list_SequenceNumber.at(i), struct_PCTestConfig);
        m_mapRetestSwitch.insert(list_SequenceNumber.at(i), (bool)struct_PCTestConfig.uc_RetestTimes);

        o_ConfigFile.TransformToList(struct_PCTestConfig.str_RFPowerDBUponLimit, list_RFPowerDBUpperLimit);
        o_ConfigFile.TransformToList(struct_PCTestConfig.str_RFPowerDBLowerLimit, list_RFPowerDBLowerLimit);
        o_ConfigFile.TransformToList(struct_PCTestConfig.str_DUTFWPositions, list_DUTFWPosition);

        m_pCountTestData->SetRetest(list_SequenceNumber.at(i), (bool)struct_PCTestConfig.uc_RetestTimes);
        m_pCountTestData->SetEnumSwitch(list_SequenceNumber.at(i), (bool)struct_PCTestConfig.uc_EnumTestSwitch);
        m_pCountTestData->SetPowerTestSwitch(list_SequenceNumber.at(i), (bool)struct_PCTestConfig.uc_PowerTestSwitch);

        m_pCountTestData->SetRFPowerDBLimit(list_SequenceNumber.at(i),
                                            list_RFPowerDBUpperLimit,
                                            list_RFPowerDBLowerLimit);

        m_pCountTestData->SetDUTFWPositions(list_SequenceNumber.at(i), list_DUTFWPosition);
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
    return true;
}

bool MasterControl::InitUsbEnumAndSendTest()
{
    qDebug()<<"InitUsbEnumAndSendTest";

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
    //注意 此处容易出现崩溃（待解决）
    qDebug()<<"ExitUsbEnumAndSendTest";
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

    //
    WaitCatchRobotLeave();

    LogFile::Addlog("Box-" + QString::number(us_SequenceNumber) + (" 打开箱子"));

    m_mapBoxState.insert(us_SequenceNumber, OPENBOX);

    return m_pDeviceOperator->OpenBox(us_SequenceNumber);
}

bool MasterControl::CloseBox(const ushort &us_SequenceNumber)
{
    if(m_bBoxSwitchClose)
        return false;

    //
    WaitCatchRobotLeave();

    LogFile::Addlog("Box-" + QString::number(us_SequenceNumber) + (" 关闭箱子"));

    m_mapBoxState.insert(us_SequenceNumber, CLOSEBOX);

    return m_pDeviceOperator->CloseBox(us_SequenceNumber);
}

bool MasterControl::SendCatchRobotAction(const ushort &us_FWStation,
                                         const QString &str_RobotAction)
{
    if(m_bRobotSwitchClose)
        return false;

    LogFile::Addlog("CatchRobot-" + QString::number(us_FWStation) + (" 发送抓取机器人动作 ") + str_RobotAction);

    return m_pDeviceOperator->SendCatchRobotAction(us_FWStation,str_RobotAction);
}

bool MasterControl::SendSupplementRobotData(const ushort &us_FWStation,
                                            const QString &str_Data)
{
    if(m_bRobotSwitchClose)
        return false;

    LogFile::Addlog("SupplementRobot-" + QString::number(us_FWStation) + (" 发送分料机器人数据 ") + str_Data);
        
    return m_pDeviceOperator->SendSupplementRobotData(us_FWStation, str_Data);
}

void MasterControl::WaitUsbEnumTestFinish()
{
    QTime o_DieTime = QTime::currentTime().addMSecs(10000);
    while(QTime::currentTime() < o_DieTime){
        if(m_bEnumTestNoWorkState)
            break;

        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }

    m_bEnumTestNoWorkState = true;
}

void MasterControl::WaitUsbPowerTestFinish()
{
    QTime o_DieTime = QTime::currentTime().addMSecs(3000);
    while(QTime::currentTime() < o_DieTime){
        if(m_bPowerTestNoWorkState)
            break;

        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }

    m_bEnumTestNoWorkState = true;
}

void MasterControl::WaitCatchRobotLeave()
{
//    QTime o_DieTime = QTime::currentTime().addMSecs(10000);
//    while(QTime::currentTime() < o_DieTime){
//        if(!m_bCatchWorking)
//            break;
    while(m_bCatchWorking){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(15);
    }

//    m_bCatchWorking = false;
}

void MasterControl::WorkSleep(uint un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void MasterControl::AppendCRobotCmd(const ushort &us_FWStation,
                                    const QString &str_RobotAction)
{
    QMutexLocker o_Locker(&m_oQMutexM);

    STRUCT_ROBOTCATCHCMD struct_RobotCatchCmd;
    struct_RobotCatchCmd.us_SequenceNumber = us_FWStation;
    struct_RobotCatchCmd.str_Cmd = str_RobotAction;

    m_listCRobotCmd.append(struct_RobotCatchCmd);
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
    if(m_bBoxSwitchClose && !m_bTest){
        return;
    }

    BOX_OPERATOR box_Operator;

    m_pDeviceObserver->GetBoxOperator(us_SequenceNumber, box_Operator);

    LogFile::Addlog("Box-" + QString::number(us_SequenceNumber) + (" 收到箱子命令 ") + QString::number(box_Operator));

    if(box_Operator == OPENBOX_OK){
        m_mapBoxState.insert(us_SequenceNumber, OPENBOX_OK);

        if(!m_bRobotSwitchClose){
            if(!m_bFirstTest){
                AppendCRobotCmd(us_SequenceNumber, CatchRobot_Get);
//                SendCatchRobotAction(us_SequenceNumber, CatchRobot_Get);
            }
        }
    }
    else if(box_Operator == CLOSEBOX_OK){
        m_mapBoxState.insert(us_SequenceNumber, CLOSEBOX_OK);

        emit sig_ReadyTest(us_SequenceNumber);
        m_oStartTest_MsgQueue.PushBack(us_SequenceNumber);
//        m_pDeviceOperator->StartOneTest(us_SequenceNumber);
    }
}

void MasterControl::slot_CatchRobotGetActionUpdata(ushort us_SequenceNumber)
{
//    qDebug()<<"slot_CatchRobotGetActionUpdata";

    if(m_bRobotSwitchClose && !m_bTest){
        return;
    }
    QString str_Action;

    m_pDeviceObserver->GetCatchRobotGetAction(us_SequenceNumber, str_Action);

    LogFile::Addlog("CatchRobot-" + QString::number(us_SequenceNumber) + (" 收到抓取机器人命令 ") + str_Action);

    if(str_Action == CatchRobot_Put_Ok){
        CloseBox(us_SequenceNumber);
    }
    else if(str_Action == CatchRobot_Get_OK){
        AppendCRobotCmd(us_SequenceNumber, CatchRobot_Put);
//        SendCatchRobotAction(us_SequenceNumber, CatchRobot_Put);
    }
    else if(str_Action == CatchRobot_Working){
        m_bCatchWorking = true;
    }
    else if(str_Action == CatchRobot_Leaving){
        m_bCatchWorking = false;
    }
    else if(str_Action == CatchRobot_Request){
        m_bRequestRobotCmd = true;
    }
}

void MasterControl::slot_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber)
{
    if(m_bRobotSwitchClose){
        return;
    }

    QString str_RequestCmd;
    QString str_RequestData;

    bool b_Ret1 = m_pDeviceObserver->GetSupplementRobotGetRequest(us_SequenceNumber, str_RequestCmd);

    LogFile::Addlog("SupplementRobot-" + QString::number(us_SequenceNumber) + (" 收到补料机器人命令 ") + str_RequestCmd);

    bool b_Ret2 = m_pCountTestData->GetResultData(us_SequenceNumber, str_RequestData);

    if(b_Ret1 && b_Ret2){
        SendSupplementRobotData(us_SequenceNumber, str_RequestData);
        LogFile::Addlog("SupplementRobot-" + QString::number(us_SequenceNumber) + (" 读取补料数据错误 ") + str_RequestCmd);
    }
//    else
//        SendSupplementRobotData(us_SequenceNumber, "00000000000000000000");

    //result
}

void MasterControl::slot_EnumUsbComplete()
{
    qDebug()<<"slot_EnumUsbComplete";
    m_pDeviceOperator->CompleteEnumTest(m_usWorkingSequenceNumber);
    m_bEnumTestNoWorkState = true;
}

void MasterControl::slot_SendPowerTestComplete()
{
    //注意 此处容易出现崩溃（待解决）
    qDebug()<<"slot_SendPowerTestComplete";
    m_pDeviceOperator->ExitSendPowerTest();
    m_pDeviceOperator->PCACK_StartOneGroupPowerTest(m_usWorkingSequenceNumber);
    m_bPowerTestNoWorkState = true;
}

void MasterControl::slot_StartTestNotice(ushort us_SequenceNumber)
{
    Q_UNUSED(us_SequenceNumber);
    m_bEnumTestNoWorkState = false;
    m_bPowerTestNoWorkState = false;
    m_usWorkingSequenceNumber = us_SequenceNumber;

    InitUsbEnumAndSendTest();

    emit sig_StartTest(us_SequenceNumber);
}

void MasterControl::slot_StartOneGroupEnumTest(ushort us_SequenceNumber)
{
    QList<int> list_OneGroupEnumTestMaskCode;
    char c_SurplusGroup = 0x00;
    int n_TestDUTMask = 0;

    m_pDeviceObserver->GetStartOneGroupEnumTestData(CURRENT,
                                                    us_SequenceNumber,
                                                    c_SurplusGroup,
                                                    n_TestDUTMask,
                                                    list_OneGroupEnumTestMaskCode);

    qDebug()<<"list_OneGroupEnumTestMaskCode"<<list_OneGroupEnumTestMaskCode;

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

    qDebug()<<"list_OneGroupPowerTestMaskCode"<<list_OneGroupPowerTestMaskCode;

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

    qDebug()<<"slot_CompleteTest\n\n\n\n\n";

    ExitUsbEnumAndSendTest();

    m_bFirstTest = false;

    m_pDeviceObserver->GetSendPowerTestCompleteData(CURRENT,
                                                    map_EnumResult,
                                                    map_OpenDeviceResult,
                                                    map_SendCmdResult);

    m_pDeviceObserver->GetUploadRFPowerResultData(CURRENT,
                                                  us_SequenceNumber,
                                                  list_Power_db);

    qDebug()<<"us_SequenceNumber"<<us_SequenceNumber;
    qDebug()<<"map_EnumResult"<<map_EnumResult;
    qDebug()<<"map_OpenDeviceResult"<<map_OpenDeviceResult;
    qDebug()<<"map_SendCmdResult"<<map_SendCmdResult;
    qDebug()<<"list_Power_db"<<list_Power_db;
    qDebug()<<"m_unStartTimes"<<m_unStartTimes;

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
                emit sig_ReadyTest(us_SequenceNumber);
                m_oStartTest_MsgQueue.PushFront(us_SequenceNumber);
//                m_oStartTest_MsgQueue.CompleteOneTest();

//                m_pDeviceOperator->StartOneTest(us_SequenceNumber);
                m_bRetested = true;
            }
            else{
                if(m_bBoxSwitchClose){
                    if(m_bAutoTest){
                        WorkSleep(AutoTime_NoEquitment);

                        if(m_bTest){
                            emit sig_ReadyTest(us_SequenceNumber);
                            m_oStartTest_MsgQueue.PushFront(us_SequenceNumber);

    //                        m_pDeviceOperator->StartOneTest(us_SequenceNumber);
                        }
                    }
                }
                else{
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
                    WorkSleep(AutoTime_NoEquitment);

                    if(m_bTest){
                        emit sig_ReadyTest(us_SequenceNumber);
                        m_oStartTest_MsgQueue.PushBack(us_SequenceNumber);

    //                    m_pDeviceOperator->StartOneTest(us_SequenceNumber);
                    }
                }
            }
            else{
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
                WorkSleep(AutoTime_NoEquitment);

                if(m_bTest){
                    emit sig_ReadyTest(us_SequenceNumber);
                    m_oStartTest_MsgQueue.PushBack(us_SequenceNumber);

    //                m_pDeviceOperator->StartOneTest(us_SequenceNumber);
                }
            }
        }
        else{
            OpenBox(us_SequenceNumber);
        }
    }

    m_oStartTest_MsgQueue.CompleteOneTest();
}

void MasterControl::slot_TimeOut_SendCRobotCmd()
{
    if(m_bBoxSwitchClose && m_bRobotSwitchClose){
        return;
    }

    if(!m_bRequestRobotCmd){
        return;
    }

    if(m_listCRobotCmd.isEmpty()){
        return;
    }

    QMutexLocker o_Locker(&m_oQMutexM);

    QMapIterator<ushort, BOX_OPERATOR> map_IterationBoxState(m_mapBoxState);
    while(map_IterationBoxState.hasNext()){
        map_IterationBoxState.next();
        if(map_IterationBoxState.value() == OPENBOX ||
                map_IterationBoxState.value() == CLOSEBOX){
            return;
        }
    }

    STRUCT_ROBOTCATCHCMD struct_RobotCatchCmd = m_listCRobotCmd.first();

    if(SendCatchRobotAction(struct_RobotCatchCmd.us_SequenceNumber,
                            struct_RobotCatchCmd.str_Cmd)){
        m_listCRobotCmd.takeFirst();
        m_bRequestRobotCmd = false;
    }
}

