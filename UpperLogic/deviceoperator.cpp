#include "deviceoperator.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QDebug>

DeviceOperator::DeviceOperator(QObject *parent)
    :QObject(parent)
{
    m_pUsbControl = NULL;
}

DeviceOperator::~DeviceOperator()
{
    if(m_pUsbControl != NULL){
        delete m_pUsbControl;
        m_pUsbControl = NULL;
    }
}

bool DeviceOperator::SetFWObjectPointer(QList<Firmware *> &list_FWObjectPointer)
{
    m_listFirmware = list_FWObjectPointer;
    return true;
}

bool DeviceOperator::GetUsbControlObjectPointer(UsbControl * &p_UsbControl)
{
    p_UsbControl = m_pUsbControl;
    return true;
}

bool DeviceOperator::SetFWPCConfig(const ushort &us_SequenceNumber,
                                   STRUCT_PCTESTCONFIG &struct_PCConfig)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->SetPCConfig(struct_PCConfig);
}

bool DeviceOperator::SetBoxObjectPoint(QList<Box *> &list_Box)
{
    m_listBox = list_Box;
    return true;
}

bool DeviceOperator::SetCatchRobotObjectPoint(CatchRobot *&p_CatchRobot)
{
    m_pCatchRobot = p_CatchRobot;
    return true;
}

//bool DeviceOperator::SetUsbControlObjectPointer(UsbControl *p_UsbControl)
//{
//    m_pUsbControl = p_UsbControl;
//    return true;
//}

//bool DeviceOperator::RemoveUsbControlObjectPoint()
//{
//    m_pUsbControl = NULL;
//    return true;
//}

bool DeviceOperator::CreatUsbControlObject()
{
    if(m_pUsbControl != NULL){
        return false;
    }

    m_pUsbControl = new UsbControl;
    m_pUsbControl->InitUsbControl();
    return true;
}

bool DeviceOperator::DeleteUsbControlObject()
{
    if(m_pUsbControl != NULL){
        delete m_pUsbControl;
        m_pUsbControl = NULL;
    }
    else{
        return false;
    }

    return true;
}

bool DeviceOperator::SetEnumTestUsbConfig(const uint &un_Pid,
                                          const uint &un_Vid,
                                          const int &n_DeviceNumber,
                                          const int &n_Time,
                                          const QMap<int, int> &map_StationPort)
{
    return m_pUsbControl->SetEnumTestUsbConfig(un_Pid,
                                               un_Vid,
                                               n_DeviceNumber,
                                               n_Time,
                                               map_StationPort);
}

bool DeviceOperator::SetPowerTestConfig(const QMap<int, int> &map_StationPort)
{
    return m_pUsbControl->SetPowerTestConfig(map_StationPort);
}

bool DeviceOperator::StartUsbEnumTest()
{
    return m_pUsbControl->StartUsbEnumTest();
}

bool DeviceOperator::StartSendPowerTest()
{
    return m_pUsbControl->StartPowerTest();
}

bool DeviceOperator::ExitUsbTest()
{
    return m_pUsbControl->ExitUsbControl();
}

bool DeviceOperator::OpenBox(const ushort &us_SequenceNumber)
{
    Box *p_Box = NULL;
    if(!GetBoxPointer(us_SequenceNumber, p_Box)){
        return false;
    }

    p_Box->OpenBox();
    return true;
}

bool DeviceOperator::CloseBox(const ushort &us_SequenceNumber)
{
    Box *p_Box = NULL;
    if(!GetBoxPointer(us_SequenceNumber, p_Box)){
        return false;
    }

    p_Box->CloseBox();
    return true;
}

bool DeviceOperator::GetFWInfo(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_GetFWInfo();
}

bool DeviceOperator::RestartFW(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_RestartFW();
}

bool DeviceOperator::InitDUTTest(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_InitDUTTest();
}

bool DeviceOperator::ExitDUTTest(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_ExitDUTTest();
}

bool DeviceOperator::StartOneTest(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_StartOneTest();
}

bool DeviceOperator::InquireMachineState(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_InquireMachineState();
}

bool DeviceOperator::CompleteEnumTest(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_CompleteEnumTest();
}

bool DeviceOperator::EnterDebugModel(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_EnterDebugModel();
}

bool DeviceOperator::ExitDebugModel(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_ExitDebugModel();
}

bool DeviceOperator::StartWholeDUTTest(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_StartWholeDUTTest();
}

bool DeviceOperator::AdjustPower(const ushort &us_SequenceNumber,
                                 char c_Funtion,
                                 short s_ReferenceVoltage_1,
                                 short s_ReferenceVoltage_2,
                                 uchar uc_Station,
                                 char c_AdjustPoint,
                                 char c_dp_Compensation)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_AdjustPower(c_Funtion,
                                s_ReferenceVoltage_1,
                                s_ReferenceVoltage_2,
                                uc_Station,
                                c_AdjustPoint,
                                c_dp_Compensation);
}

bool DeviceOperator::ReadPowerData(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_ReadPowerData();
}

bool DeviceOperator::ClearPowerConfig(const ushort &us_SequenceNumber,
                                      const int &n_Station)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_ClearPowerConfig(n_Station);
}

bool DeviceOperator::TestPowerSelfTest(const ushort &us_SequenceNumber)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_TestPowerSelfTest();
}

bool DeviceOperator::TestEnum(const ushort &us_SequenceNumber,
                              const ENUM_POWERONGROUP &Group,
                              const short &s_Time)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_TestEnum(Group,s_Time);
}

bool DeviceOperator::PowerOnSwitch(const ushort &us_SequenceNumber,
                                   const ENUM_POWERONSWITCH &Switch,
                                   const ENUM_POWERONGROUP &Group)
{
    Firmware *p_FW = NULL;
    if(!GetFWPointer(us_SequenceNumber, p_FW)){
        return false;
    }

    return p_FW->PC_PowerOnSwitch(Switch, Group);
}

void DeviceOperator::WorkSlepp(ushort un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

bool DeviceOperator::GetFWPointer(const ushort &us_SequenceNumber,
                                  Firmware * &p_FW)
{
    if(m_listFirmware.isEmpty()){
        return false;
    }

    ushort us_FWSequence = 0;

    foreach(Firmware *p_FWPoint, m_listFirmware){
        p_FWPoint->GetSequenceNumber(us_FWSequence);
        if(us_FWSequence == us_SequenceNumber){
            p_FW = p_FWPoint;
            return true;
        }
    }
    return false;
}

bool DeviceOperator::GetBoxPointer(const ushort &us_SequenceNumber,
                                   Box * &p_Box)
{
    if(m_listBox.isEmpty()){
        return false;
    }

    ushort us_BoxSequence = 0;

    foreach(Box *p_BoxPoint, m_listBox){
        p_BoxPoint->GetSequenceNumber(us_BoxSequence);
        if(us_BoxSequence == us_SequenceNumber){
            p_Box = p_BoxPoint;
            return true;
        }
    }
    return false;
}
