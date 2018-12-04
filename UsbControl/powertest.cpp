#include "powertest.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include "writecmdthread.h"
#include "usboperator.h"
#include <QDebug>

PowerTest::PowerTest(QObject *parent)
    :QObject(parent)
{
    m_pQTimer       = NULL;
    m_pQTimer       = new QTimer;

    m_pQTimer->setSingleShot(true);
    m_oQThreadPool.setMaxThreadCount(30);
    connect(m_pQTimer, SIGNAL(timeout()), this, SLOT(slot_TimeOut()));
}

PowerTest::~PowerTest()
{
    CloseUsbDevice();

    if(m_pQTimer != NULL){
        if(m_pQTimer->isActive()){
            m_pQTimer->stop();
        }
        delete m_pQTimer;
        m_pQTimer = NULL;
    }
}

bool PowerTest::SetPowerTestConfig(const QMap<int, int> &map_StationPort)
{
    qDebug()<<"SetPowerTestConfig m_mapLDHandle.clear();";

    if(!m_mapStationPort.isEmpty()){
        m_mapStationPort.clear();
    }

    if(!m_mapLDHandle.isEmpty()){
        m_mapLDHandle.clear();
    }

    if(!m_mapOpenDeviceResult.isEmpty()){
        m_mapOpenDeviceResult.clear();
    }

    if(!m_mapSendCmdResult.isEmpty()){
        m_mapSendCmdResult.clear();
    }

    m_mapStationPort = map_StationPort;
    return true;
}

bool PowerTest::OpenUsbDevice(const QMap<int, libusb_device *> &map_LDevice)
{
    //m_mapStationPort, <station, port>
    //m_mapOpenDeviceResult, <station, OpenResult>
    //m_mapLDHandle, <station, libusb_device_handle*>
    //map_LDevice, <port, libusb_device*>

    if(map_LDevice.isEmpty() || m_mapStationPort.isEmpty()){
        return false;
    }

    qDebug()<<"map_LDevice"<<map_LDevice;
    qDebug()<<"m_mapStationPort"<<m_mapStationPort;

    QMapIterator<int,int> map_IteratorStationPort(m_mapStationPort);
    while(map_IteratorStationPort.hasNext()){
        map_IteratorStationPort.next();

        m_mapOpenDeviceResult.insert(map_IteratorStationPort.key(), false);

        QMapIterator<int, libusb_device*> map_IterationLDevice(map_LDevice);
        while(map_IterationLDevice.hasNext()){
            map_IterationLDevice.next();

            if(map_IterationLDevice.key() == map_IteratorStationPort.value()){
                libusb_device_handle *p_LDHandle = NULL;
                int n_Ret = libusb_open(map_IterationLDevice.value(), &p_LDHandle);
//                qDebug()<<"n_Ret"<<n_Ret;
//                qDebug()<<"p_LDHandle"<<p_LDHandle;
                if(n_Ret == 0){
                    if(libusb_kernel_driver_active(p_LDHandle, 1) != 0){
                        if(libusb_detach_kernel_driver(p_LDHandle, 1) != 0){
                            libusb_close(p_LDHandle);
                            continue;
                        }
                    }
//                    if(libusb_set_configuration(p_LDHandle, 1) != 0){
//                        libusb_close(p_LDHandle);
//                        continue;
//                    }

                    if(libusb_claim_interface(p_LDHandle, 1) != 0){
                        libusb_close(p_LDHandle);
                        continue;
                    }

                    m_mapLDHandle.insert(map_IteratorStationPort.key(), p_LDHandle);
                    m_mapOpenDeviceResult.insert(map_IteratorStationPort.key(), true);
                }
            }
        }
    }

    qDebug()<<"m_mapOpenDeviceResult"<<m_mapOpenDeviceResult;

    return true;
}

bool PowerTest::CloseUsbDevice()
{
    if(m_mapLDHandle.isEmpty()){
        return false;
    }

    qDebug()<<"CloseUsbDevice m_mapLDHandle.clear();";

    QMapIterator<int, libusb_device_handle *> map_IterationLDHandle(m_mapLDHandle);
    while(map_IterationLDHandle.hasNext()){
        map_IterationLDHandle.next();
        libusb_release_interface(map_IterationLDHandle.value(), 1);
        libusb_close(map_IterationLDHandle.value());
    }

    m_mapLDHandle.clear();
    return true;
}

bool PowerTest::StartPowerTest()
{
    m_bSendComplete = false;

    if(m_mapLDHandle.isEmpty()){
        CompletePowerTest();
        return true;
    }

    qDebug()<<"StartPowerTest  m_mapLDHandle"<<m_mapLDHandle;

    QMapIterator<int, libusb_device_handle *> map_IterationLDHandle(m_mapLDHandle);
    while(map_IterationLDHandle.hasNext()){
        map_IterationLDHandle.next();
            WriteCmdThread *p_WriteCmdThread = new WriteCmdThread();
            p_WriteCmdThread->SetStation(map_IterationLDHandle.key());
            p_WriteCmdThread->SetLDHandle(map_IterationLDHandle.value());
            m_oQThreadPool.start(p_WriteCmdThread);
    }

    m_pQTimer->start(100);
    return true;
}

bool PowerTest::GetPowerTestResult(QMap<int, bool> &map_OpenDeviceResult,
                                   QMap<int, bool> &map_SendCmdResult)
{
//    if(m_mapOpenDeviceResult.isEmpty() ||
//       m_mapSendCmdResult.isEmpty()){
//        return false;
//    }

    QMapIterator<int,int> map_IteratorStationPort(m_mapStationPort);
    while(map_IteratorStationPort.hasNext()){
        map_IteratorStationPort.next();

        map_SendCmdResult.insert(map_IteratorStationPort.key(), false);

        QMapIterator<int, bool> map_IterationSendCmdResult(m_mapSendCmdResult);
        while(map_IterationSendCmdResult.hasNext()){
            map_IterationSendCmdResult.next();

            if(map_IterationSendCmdResult.key() == map_IteratorStationPort.key()){
               map_SendCmdResult.insert(map_IterationSendCmdResult.key(),
                                        map_IterationSendCmdResult.value());
            }
        }


        map_OpenDeviceResult.insert(map_IteratorStationPort.key(), false);
        QMapIterator<int, bool> map_IteratorOpenDeviceResult(m_mapOpenDeviceResult);
        while(map_IteratorOpenDeviceResult.hasNext()){
            map_IteratorOpenDeviceResult.next();

            if(map_IteratorOpenDeviceResult.key() == map_IteratorStationPort.key()){
                map_OpenDeviceResult.insert(map_IteratorOpenDeviceResult.key(),
                                            map_IteratorOpenDeviceResult.value());
            }
        }
    }

    qDebug()<<"GetPowerTestResult m_mapStationPort  "<<m_mapStationPort;
    qDebug()<<"GetPowerTestResult map_OpenDeviceResult  "<<map_OpenDeviceResult;
    qDebug()<<"GetPowerTestResult map_SendCmdResult  "<<map_SendCmdResult;

    return true;
}

bool PowerTest::WriteSendCmdResult(int n_Station,
                                   bool b_Result)
{
    m_oQMutex.lock();

    m_mapSendCmdResult.insert(n_Station, b_Result);
    if(m_mapSendCmdResult.count() == m_mapLDHandle.count()){
        if(!m_bSendComplete){
            CompletePowerTest();
        }
    }
    qDebug()<<"m_mapSendCmdResult"<<m_mapSendCmdResult;

    m_oQMutex.unlock();
    return true;
}

bool PowerTest::StartSinglePowerTest(libusb_device_handle *p_LDHandle)
{
    if(p_LDHandle == NULL){
        return false;
    }

    if(JudgeEnterCommandModel(p_LDHandle)){
        if(EnterTestModel(p_LDHandle)){
            if(TestSignalCarrier(p_LDHandle)){
                return true;
            }
        }
    }

    return false;
}

bool PowerTest::WriteCmdInDongle(libusb_device_handle *p_LDHandle,
                                 uchar *uc_Data,
                                 ushort us_Length)
{
    int n_Ret = libusb_control_transfer(p_LDHandle,
                                        0x21,
                                        0x09,   /* set/get test */
                                        0x02ba, /* test type    */
                                        1,      /* interface id */
                                        uc_Data,
                                        us_Length,
                                        1000);

//    qDebug()<<"WriteCmdInDongle"<<n_Ret;

    if(n_Ret >= 0)
        return true;
    else
        return false;
}

bool PowerTest::ReadDetailsFromDongle(libusb_device_handle *p_LDHandle,
                                      uchar *uc_Data,
                                      ushort us_Length)
{
    int n_Ret = libusb_control_transfer(p_LDHandle,
                                        0xa1,
                                        0x01,   /* set/get test */
                                        0x01ba, /* test type    */
                                        1,      /* interface id */
                                        uc_Data,
                                        us_Length,
                                        1000);

    if(n_Ret == 0)
        return true;
    else
        return false;
}

bool PowerTest::SendCmdToDogle(libusb_device_handle *p_LDHandle, uchar *uc_Data, ushort us_Length)
{
    bool b_Ret = WriteCmdInDongle(p_LDHandle, uc_Data, us_Length);
    if(b_Ret){
        uchar ucData[2] = {0x00, 0x00};
        for(int i = 0; i<3 || ucData[0] == 0x01 ; i++){
            WorkSleep(30);
            ReadDetailsFromDongle(p_LDHandle, ucData, 2);
            if(ucData[0] == 0x01){
                return true;
            }
        }
    }
    return false;
}

bool PowerTest::JudgeEnterCommandModel(libusb_device_handle *p_LDHandle)
{
    uchar ucData[2] = {0xa7, 0x00};
    return SendCmdToDogle(p_LDHandle, ucData, 2);
}

bool PowerTest::EnterTestModel(libusb_device_handle *p_LDHandle)
{
    uchar ucData[2] = {0xa0, 0x82};
    return SendCmdToDogle(p_LDHandle, ucData, 2);
}

bool PowerTest::TestSignalCarrier(libusb_device_handle *p_LDHandle)
{
    uchar ucData[2] = {0xa2, 0x81};
    return SendCmdToDogle(p_LDHandle, ucData, 2);
}

bool PowerTest::CompletePowerTest()
{
    //wait the scheme, nothing to do
    emit sig_SendCmdComplete();
    m_bSendComplete = true;;
    return true;
}

void PowerTest::WorkSleep(uint un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void PowerTest::slot_TimeOut()
{
//    if(m_oQThreadPool.activeThreadCount() == 0){
//        if(!){
//            CompletePowerTest();
//        }

//        if(m_pQTimer->isActive()){

//            m_pQTimer->stop();
//        }
//    }

    m_oQThreadPool.releaseThread();
    m_oQThreadPool.clear();
    if(!m_bSendComplete){
        CompletePowerTest();
    }
}
