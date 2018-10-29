#include "mastercontrol.h"

MasterControl::MasterControl(QObject *parent)
    :QObject(parent)
{
    m_pDeviceObserver = NULL;
    m_pDeviceOperator = NULL;

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

    connect(m_pDeviceObserver, SIGNAL(sig_FirmwareDiscoverd()),
            this, SLOT(slot_FirmwareDiscoverd()));
    connect(m_pDeviceObserver, SIGNAL(sig_FirmwareRemove()),
            this, SLOT(slot_FirmwareRemove()));

    SetConnectFWConfig();

    UpdataFWDevice();

    return true;
}

bool MasterControl::SetConnectFWConfig()
{
    ConfigFile o_ConfigFile;
    STRUCT_EQUITMENTCONFIG struct_EquitmentConfig;
    o_ConfigFile.GetEquitmentConfig(struct_EquitmentConfig);

    QList<STRUCT_COMINFO> list_FWComInfo;
    STRUCT_COMINFO struct_ComInfo;

    for(int i = 0; i < struct_EquitmentConfig.list_ConName.count(); i++){
        struct_ComInfo.str_PortName = struct_EquitmentConfig.list_ConName.at(i);
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

    return true;
}

bool MasterControl::UpdataFWDevice()
{
    QList<Firmware *> list_FWObjectPointer;
    m_pDeviceObserver->GetFWObjectPointer(list_FWObjectPointer);
    m_pDeviceOperator->SetFWObjectPointer(list_FWObjectPointer);

    SetAllFWPCConfig();

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

    return true;
}

bool MasterControl::InitUsbEnumAndSendTest(const ushort &us_SequenceNumber)
{
    UsbControl *p_UsbControl = NULL;
    if(!m_pDeviceOperator->CreatUsbControlObject()){
        m_pDeviceOperator->ExitUsbTest();
        m_pDeviceOperator->DeleteUsbControlObject();
        m_pDeviceObserver->RemoveUsbControlObjectPoint();
        m_pDeviceOperator->CreatUsbControlObject();
    }
    m_pDeviceOperator->GetUsbControlObjectPointer(p_UsbControl);
    m_pDeviceObserver->SetUsbControlObjectPointer(p_UsbControl);

    return true;
}

bool MasterControl::ExitUsbEnumAndSendTest(const ushort &us_SequenceNumber)
{
    bool b_Ret1 = m_pDeviceOperator->ExitUsbTest();
    bool b_Ret2 = m_pDeviceOperator->DeleteUsbControlObject();
    bool b_Ret3 = m_pDeviceObserver->RemoveUsbControlObjectPoint();

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

void MasterControl::slot_FirmwareDiscoverd()
{
    UpdataFWDevice();
}

void MasterControl::slot_FirmwareRemove()
{
    UpdataFWDevice();
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
