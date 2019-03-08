#include "usbcontrol.h"
#include "QDebug"

UsbControl::UsbControl(QObject *parent)
    :   QObject(parent)
{
    m_pEnumUsb = NULL;
    m_pPowerTest = NULL;

    m_pEnumUsb = new EnumUsb;
    m_pPowerTest = m_oPowerTestInstanceGetter.GetInstance();

    connect(m_pEnumUsb, SIGNAL(sig_EnumComplete()),
            this, SLOT(slot_EnumUsbTestComplete()));
    connect(m_pPowerTest, SIGNAL(sig_SendCmdComplete()),
            this, SLOT(slot_SendPowerTestomplete()));
}

UsbControl::~UsbControl()
{
    ExitUsbControl();
    if(m_pEnumUsb != NULL){
        delete m_pEnumUsb;
        m_pEnumUsb = NULL;
    }
}

bool UsbControl::InitUsbControl()
{
    m_pEnumUsb->InitEnumUsb();
    return true;
}

bool UsbControl::SetEnumTestUsbConfig(const uint &un_Pid,
                                      const uint &un_Vid,
                                      const int &n_DeviceNumber,
                                      const int &n_Time,
                                      const QMap<int, int> &map_StationPort)
{
    return m_pEnumUsb->SetEnumUsbConfig(un_Pid,
                                        un_Vid,
                                        n_DeviceNumber,
                                        n_Time,
                                        map_StationPort);


/*
    bool b_Ret1 = m_pEnumUsb->SetEnumUsbConfig(un_Pid,
                                               un_Vid,
                                               n_DeviceNumber,
                                               n_Time,
                                               map_StationPort);

    bool b_Ret2 = m_pPowerTest->SetPowerTestConfig(map_StationPort);

    if(b_Ret1 && b_Ret2){
        return true;
    }

    return false;
*/
}

bool UsbControl::SetPowerTestConfig(const QMap<int, int> &map_StationPort)
{
    return m_pPowerTest->SetPowerTestConfig(map_StationPort);
}

bool UsbControl::StartUsbEnumTest()
{
    return m_pEnumUsb->StartNewEnumUsb();
}

bool UsbControl::StartPowerTest()
{
    QMap<int, libusb_device *> map_LDevice;

    bool b_Ret1 = m_pEnumUsb->GetDevice(map_LDevice);

    bool b_Ret2 = m_pPowerTest->OpenUsbDevice(map_LDevice);

    qDebug()<<"b_Ret1"<<b_Ret1;
    qDebug()<<"b_Ret2"<<b_Ret2;

    if(!b_Ret1 || !b_Ret2){
        emit sig_SendPowerTestComplete();
        return false;
    }
    return m_pPowerTest->StartPowerTest();
}

bool UsbControl::GetEnumResult(QMap<int, bool> &map_EnumResult)
{
    return m_pEnumUsb->GetEnumResult(map_EnumResult);
}

bool UsbControl::GetTestResult(QMap<int, bool> &map_EnumResult,
                               QMap<int, bool> &map_OpenDeviceResult,
                               QMap<int, bool> &map_SendCmdResult)
{
    bool b_Ret1 = m_pEnumUsb->GetEnumResult(map_EnumResult);
    bool b_Ret2 = m_pPowerTest->GetPowerTestResult(map_OpenDeviceResult,
                                                   map_SendCmdResult);

    if(b_Ret1 && b_Ret2){
        return true;
    }

    return false;
}

bool UsbControl::ExitUsbControl()
{
    bool b_Ret1 = ExitPowerTest();
    bool b_Ret2 = ExitEnumUsbTest();
    if(b_Ret1 && b_Ret2){
        return true;
    }

    return false;
}

bool UsbControl::ExitEnumUsbTest()
{
    return m_pEnumUsb->CloseEmumUsb();
}

bool UsbControl::ExitPowerTest()
{
    return m_pPowerTest->CloseUsbDevice();
}

void UsbControl::slot_EnumUsbTestComplete()
{
    emit sig_EnumUsbComplete();
}

void UsbControl::slot_SendPowerTestomplete()
{
    emit sig_SendPowerTestComplete();
}
