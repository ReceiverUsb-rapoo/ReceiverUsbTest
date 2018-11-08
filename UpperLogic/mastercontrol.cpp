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

void MasterControl::InitTest()
{

}

void MasterControl::StartOneTest()
{

}

void MasterControl::StartAutoTest()
{

}

void MasterControl::StopTest()
{

}

bool MasterControl::InitMasterControl()
{
    m_pDeviceObserver = m_oDeviceObserverInstanceGetter.GetInstance();
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();

    ConnectAllPath();

    SetEquitmentConfig();

    UpdataFWDevice();

    UpdataBox();

    UpdataCatchRobot();

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

    m_pDeviceObserver->SetCatchRobotIP(struct_EquitmentConfig.list_RobotIP.at(0));

    m_pDeviceObserver->SetLocalTcpServer(struct_EquitmentConfig.str_ComputerIP,
                                         struct_EquitmentConfig.us_ComputerPort);

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

bool MasterControl::SetAllFWPCConfig()
{
    QList<Firmware *> list_Firmware;

    m_pDeviceObserver->GetFWObjectPointer(list_Firmware);

    if(list_Firmware.isEmpty()){
        return false;
    }

    ConfigFile o_ConfigFile;
    ushort us_FWSequence = 0;
    STRUCT_PCTESTCONFIG struct_PCTestConfig;

    foreach(Firmware *p_FWPoint, list_Firmware){
        p_FWPoint->GetSequenceNumber(us_FWSequence);
        o_ConfigFile.GetPCTestConfig(us_FWSequence, struct_PCTestConfig);
        m_pDeviceOperator->SetFWPCConfig(us_FWSequence, struct_PCTestConfig);
    }
    return true;
}

bool MasterControl::SetAllUsbControlConfig()
{
    QList<Firmware *> list_Firmware;
    m_pDeviceObserver->GetFWObjectPointer(list_Firmware);

    if(list_Firmware.isEmpty()){
        return false;
    }

    ConfigFile o_ConfigFile;
    ushort us_FWSequence = 0;
    STRUCT_USBCONTROLCONFIG struct_UsbControlConfig;

    foreach(Firmware *p_FWPoint, list_Firmware){
        p_FWPoint->GetSequenceNumber(us_FWSequence);
        o_ConfigFile.GetUsbControlConfig(us_FWSequence, struct_UsbControlConfig);
        m_mapUsbControlConfig.insert(us_FWSequence, struct_UsbControlConfig);
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

    connect(m_pDeviceObserver, SIGNAL(sig_BoxOperatorUpdata(ushort)),
            this, SLOT(slot_BoxOperatorUpdata(ushort)));

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

    return true;
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

bool MasterControl::InitUsbEnumAndSendTest(const ushort &us_SequenceNumber)
{
    Q_UNUSED(us_SequenceNumber);
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

bool MasterControl::ExitUsbEnumAndSendTest(const ushort &us_SequenceNumber)
{
    Q_UNUSED(us_SequenceNumber);
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
    Q_UNUSED(us_SequenceNumber);
    STRUCT_USBCONTROLCONFIG m_structUsbControlConfig;
    QMap<int,int> map_StationPort;
    for(int i = 0; i< list_OneGroupPowerTest.count(); i++){
        if(list_OneGroupPowerTest.at(i) != 0){
            map_StationPort.insert(list_OneGroupPowerTest.at(i),
                                   m_structUsbControlConfig.
                                   map_StationPort.
                                   value(list_OneGroupPowerTest.at(i)));
        }
    }

    m_pDeviceOperator->SetEnumTestUsbConfig(m_structUsbControlConfig.un_Pid,
                                            m_structUsbControlConfig.un_Vid,
                                            map_StationPort.count(),
                                            m_structUsbControlConfig.n_Time,
                                            map_StationPort);

    return m_pDeviceOperator->StartUsbEnumTest();
}

bool MasterControl::StartUsbPowerTestByOneGroup(const ushort &us_SequenceNumber,
                                                const QList<int> &list_OneGroupSendTest)
{
    Q_UNUSED(us_SequenceNumber);
    STRUCT_USBCONTROLCONFIG m_structUsbControlConfig;
    QMap<int,int> map_StationPort;
    for(int i = 0; i< list_OneGroupSendTest.count(); i++){
        if(list_OneGroupSendTest.at(i) != 0){
            map_StationPort.insert(list_OneGroupSendTest.at(i),
                                   m_structUsbControlConfig.
                                   map_StationPort.
                                   value(list_OneGroupSendTest.at(i)));
        }
    }

    m_pDeviceOperator->SetPowerTestConfig(map_StationPort);
    return m_pDeviceOperator->StartSendPowerTest();
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

void MasterControl::slot_FirmwareDiscoverd()
{
    UpdataFWDevice();
}

void MasterControl::slot_FirmwareRemove()
{
    UpdataFWDevice();
}

void MasterControl::slot_BoxDiscoverd()
{
    UpdataBox();
}

void MasterControl::slot_CatchRobotDiscoverd()
{
    UpdataCatchRobot();
}

void MasterControl::slot_BoxRemove()
{
    UpdataBox();
}

void MasterControl::slot_CatchRobotRemove()
{
    UpdataCatchRobot();
}

void MasterControl::slot_EnumUsbComplete()
{

}

void MasterControl::slot_SendPowerTestComplete()
{

}

void MasterControl::slot_StartTestNotice(ushort us_SequenceNumber)
{

}

void MasterControl::slot_StartOneGroupEnumTest(ushort us_SequenceNumber)
{

}

void MasterControl::slot_StartOneGroupPowerTest(ushort us_SequenceNumber)
{

}

void MasterControl::slot_CompleteTest(ushort us_SequenceNumber)
{

}

void MasterControl::slot_BoxOperatorUpdata(ushort us_SequenceNumber)
{

}
