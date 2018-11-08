#include "deviceobserver.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QDebug>

DeviceObserver::DeviceObserver(QObject *parent)
    :   QObject(parent)
{
    m_OpenFWModel = BYCOM;
    m_bManuallyCreatUsbControl = false;

    m_pUsbControl = NULL;
    m_pComDiscoverd = NULL;
    m_pQThread = NULL;

    InitComDiscover();
    InitTcpServer();

//    m_pQThread = new QThread;
//    this->moveToThread(m_pQThread);
//    m_pQThread->start();
}

DeviceObserver::~DeviceObserver()
{
    if(m_pComDiscoverd != NULL){
        delete m_pComDiscoverd;
        m_pComDiscoverd = NULL;
    }

    if(m_pQThread != NULL){
        m_pQThread->deleteLater();
        m_pQThread = NULL;
    }

    RemoveAllFWAndFWData();
}

bool DeviceObserver::SetNeedFWConfig(const QList<STRUCT_COMINFO> &list_FWComInfo)
{
    m_listNeedFW = list_FWComInfo;
    return true;
}

bool DeviceObserver::SetOpenFWModelByCom()
{
    m_OpenFWModel = BYCOM;
    return true;
}

bool DeviceObserver::SetOpenFWModelByPidVid()
{
    m_OpenFWModel = BYPIDVID;
    return true;
}

bool DeviceObserver::GetFWObjectPointer(QList<Firmware *> &list_FWObjectPointer)
{
    list_FWObjectPointer = m_listFirmware;
    return true;
}

bool DeviceObserver::GetUsbControlObjectPointer(UsbControl * &p_UsbControl)
{
    p_UsbControl = m_pUsbControl;
    return true;
}

bool DeviceObserver::SetUsbControlObjectPointer(UsbControl * &p_UsbControl)
{
    if(m_pUsbControl != NULL){
        return false;
    }

    m_pUsbControl = p_UsbControl;

    connect(p_UsbControl, SIGNAL(sig_EnumUsbComplete()),
            this, SLOT(slot_EnumUsbComplete()));
    connect(p_UsbControl, SIGNAL(sig_SendPowerTestComplete()),
            this, SLOT(slot_SendPowerTestComplete()));

    return true;
}

bool DeviceObserver::RemoveUsbControlObjectPoint()
{
    if(m_pUsbControl == NULL){
        return false;
    }

    disconnect(m_pUsbControl, SIGNAL(sig_EnumUsbComplete()),
            this, SLOT(slot_EnumUsbComplete()));
    disconnect(m_pUsbControl, SIGNAL(sig_SendPowerTestComplete()),
            this, SLOT(slot_SendPowerTestComplete()));

    m_pUsbControl = NULL;

    return true;
}

bool DeviceObserver::SetLocalTcpServer(const QString &str_IP,
                                       const ushort &us_Port)
{
    if(m_pTcpServer->isListening()){
        m_pTcpServer->close();
    }

    return m_pTcpServer->StartListen(us_Port,
                                     str_IP);
}

bool DeviceObserver::SetBoxIP(const QList<QString> &list_BoxIP)
{
    m_listBoxIP = list_BoxIP;
    return true;
}

bool DeviceObserver::SetCatchRobotIP(const QString &str_CatchRobotIP)
{
    m_strLocalTcpServerIP = str_CatchRobotIP;
    return true;
}

bool DeviceObserver::GetBoxObjectPoint(QList<Box *> &list_Box)
{
    list_Box = m_listBox;
    return true;
}

bool DeviceObserver::GetCatchRobotObjectPoint(CatchRobot *&p_CatchRobot)
{
    p_CatchRobot = m_pCatchRobot;
    return true;
}

bool DeviceObserver::ClearEnumResult()
{
    m_mapEnumResult.clear();
    return true;
}

bool DeviceObserver::ClearPowerSendResult()
{
    m_mapOpenDeviceResult.clear();
    m_mapSendCmdResult.clear();
    return true;
}

void DeviceObserver::GetEnumUsbCompleteData(EMUN_TESTDATAPOINT Point,
                                            QMap<int, bool> &map_EnumResult)
{
    if(Point == LAST){
        map_EnumResult = m_mapLastEnumResult;
    }
    else if(Point == CURRENT){
        map_EnumResult = m_mapEnumResult;
    }
}

void DeviceObserver::GetSendPowerTestCompleteData(EMUN_TESTDATAPOINT Point,
                                                  QMap<int, bool> &map_EnumResult,
                                                  QMap<int, bool> &map_OpenDeviceResult,
                                                  QMap<int, bool> &map_SendCmdResult)
{
    if(Point == LAST){
        map_EnumResult = m_mapLastEnumResult;
        map_OpenDeviceResult = m_mapLastOpenDeviceResult;
        map_SendCmdResult = m_mapLastSendCmdResult;
    }
    else if(Point == CURRENT){
        map_EnumResult = m_mapEnumResult;
        map_OpenDeviceResult = m_mapOpenDeviceResult;
        map_SendCmdResult = m_mapSendCmdResult;
    }
}

void DeviceObserver::GetReceiveCommandAndData(const ushort &us_SequenceNumber,
                                              uchar uc_Command,
                                              STRUCT_COMMANDANDDATA &struct_CommandAndData)
{
    foreach(STRUCT_COMMANDANDDATA struct_Data, m_listReceiveCommandAndData){
        if(struct_Data.us_SequenceNumber == us_SequenceNumber &&
           struct_Data.uc_Command == uc_Command){
            struct_CommandAndData = struct_Data;
        }
    }
}

void DeviceObserver::GetWriteCommandAndData(const ushort &us_SequenceNumber,
                                            uchar uc_Command,
                                            STRUCT_COMMANDANDDATA &struct_CommandAndData)
{
    foreach(STRUCT_COMMANDANDDATA struct_Data, m_listWriteCommandAndData){
        if(struct_Data.us_SequenceNumber == us_SequenceNumber &&
           struct_Data.uc_Command == uc_Command){
            struct_CommandAndData = struct_Data;
        }
    }
}

void DeviceObserver::GetGetFWInfoData(EMUN_TESTDATAPOINT Point,
                                      const ushort &us_SequenceNumber,
                                      STRUCT_FWINFO &struct_FWInfo)
{
    FWUploadData *p_FWData = NULL;

    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    struct_FWInfo = p_FWData->struct_GetFWInfo;
}

void DeviceObserver::GetTestTimeOutData(EMUN_TESTDATAPOINT Point,
                                        const ushort &us_SequenceNumber,
                                        int &n_TestTimeOut)
{
    FWUploadData *p_FWData = NULL;

    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    n_TestTimeOut = p_FWData->n_TestTimeOut;
}

void DeviceObserver::GetMachineStateData(EMUN_TESTDATAPOINT Point,
                                         const ushort &us_SequenceNumber,
                                         ENUM_MACHINEDUTSTATE &DUTState,
                                         ENUM_MACHINETESTSTATE &TestState)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    DUTState = p_FWData->MachineDUTState;
    TestState = p_FWData->MachineTestState;
}

void DeviceObserver::GetAdjustPowerData(EMUN_TESTDATAPOINT Point,
                                        const ushort &us_SequenceNumber,
                                        short &s_Vslop,
                                        short &s_VslopAdc,
                                        short &s_Intercept)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    s_Vslop = p_FWData->s_AdjustPowerVslop;
    s_VslopAdc = p_FWData->s_AdjustPowerVslopAdc;
    s_Intercept = p_FWData->s_AdjustPowerIntercept;
}

void DeviceObserver::GetReadPowerData(EMUN_TESTDATAPOINT Point,
                                      const ushort &us_SequenceNumber,
                                      QList<short> &list_Vslop,
                                      QList<short> &list_VslopAdc,
                                      QList<short> &list_Intercept,
                                      QList<char> &list_db_Compensation,
                                      short &s_ReferenceVoltage_1,
                                      short &s_ReferenceVoltage_2)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    list_Vslop = p_FWData->list_ReadPowerVslop;
    list_VslopAdc = p_FWData->list_ReadPowerVslopAdc;
    list_Intercept = p_FWData->list_ReadPowerIntercept;
    list_db_Compensation = p_FWData->list_ReadPower_db_Compensation;
    s_ReferenceVoltage_1 = p_FWData->s_ReadPowerReferenceVoltage_1;
    s_ReferenceVoltage_2 = p_FWData->s_ReadPowerReferenceVoltage_2;
}

void DeviceObserver::GetPowerSelfTestData(EMUN_TESTDATAPOINT Point,
                                          const ushort &us_SequenceNumber,
                                          QList<short> &list_SelfVoltage)
{
    FWUploadData *p_FWData = NULL;

    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }


    list_SelfVoltage = p_FWData->list_PowerSelfTestSelfVoltage;
}

void DeviceObserver::GetFWBootData(EMUN_TESTDATAPOINT Point,
                                   const ushort &us_SequenceNumber,
                                   STRUCT_FWINFO &struct_FWInfo)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }


    struct_FWInfo = p_FWData->struct_FWBoot;
}

void DeviceObserver::GetFWIntactData(EMUN_TESTDATAPOINT Point,
                                     const ushort &us_SequenceNumber,
                                     ENUM_MSGDEFINE &FWIntact)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    FWIntact = p_FWData->FWIntact;
}

void DeviceObserver::GetHandDataBagData(EMUN_TESTDATAPOINT Point,
                                        const ushort &us_SequenceNumber,
                                        STRUCT_HANDBAG &struct_HandBag)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    struct_HandBag = p_FWData->struct_HandBag;
}

void DeviceObserver::GetStartTestNoticeData(EMUN_TESTDATAPOINT Point,
                                            const ushort &us_SequenceNumber,
                                            char &c_TestType,
                                            char &c_TotalGroup,
                                            char &c_AmoutOfAGroup,
                                            QList<char> &list_UsefulDUT)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    c_TestType = p_FWData->c_StartTestNoticeTestType;
    c_TotalGroup = p_FWData->c_StartTestNoticeTotalGroup;
    c_AmoutOfAGroup = p_FWData->c_StartTestNoticAmoutOfAGroup;
    list_UsefulDUT = p_FWData->list_StartTestNoticUsefulDUT;
}

void DeviceObserver::GetCompleteTestData(EMUN_TESTDATAPOINT Point,
                                         const ushort &us_SequenceNumber,
                                         QList<char> &list_UsefulResult)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    list_UsefulResult = p_FWData->list_CompleteTestUsefulResult;
}

void DeviceObserver::GetStartOneGroupPowerTestData(EMUN_TESTDATAPOINT Point,
                                                   const ushort &us_SequenceNumber,
                                                   char &c_SurplusGroup,
                                                   int &n_TestDUTMask,
                                                   QList<int> &list_MaskCode)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    c_SurplusGroup = p_FWData->c_StartOneGroupPowerTestSurplusGroup;
    n_TestDUTMask = p_FWData->n_StartOneGroupPowerTestDUTMask;

    TransformMaskCode(us_SequenceNumber, n_TestDUTMask, list_MaskCode);
}

void DeviceObserver::GetStartOneGroupEnumTestData(EMUN_TESTDATAPOINT Point,
                                                  const ushort &us_SequenceNumber,
                                                  char &c_SurplusGroup,
                                                  int &n_TestDUTMask,
                                                  QList<int> &list_MaskCode)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    c_SurplusGroup = p_FWData->c_StartOneGroupEnumTestSurplusGroup;
    n_TestDUTMask = p_FWData->n_StartOneGroupEnumTestDUTMask;

    TransformMaskCode(us_SequenceNumber, n_TestDUTMask, list_MaskCode);
}

void DeviceObserver::GetUploadRFPowerResultData(EMUN_TESTDATAPOINT Point,
                                                const ushort &us_SequenceNumber,
                                                QList<short> &list_Power_db)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    list_Power_db = p_FWData->list_UploadRFPowerResult_db;
}

void DeviceObserver::ClearFWTestData(EMUN_TESTDATAPOINT Point,
                                     const ushort &us_SequenceNumber)
{
    FWUploadData *p_FWData = NULL;
    if(Point == LAST){
        if(!GetLastFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }
    else if(Point == CURRENT){
        if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
            return;
        }
    }

    p_FWData->ClearFWData();
}

void DeviceObserver::InitComDiscover()
{
    m_pComDiscoverd = new ComDiscoverd;

    connect(m_pComDiscoverd, SIGNAL(sig_ComDiscoverd(QString,uint,uint)),
            this, SLOT(slot_FirmwareDiscoverd(QString,uint,uint)));
    connect(m_pComDiscoverd, SIGNAL(sig_ComRemove(QString,uint,uint)),
            this, SLOT(slot_FirmwareRemove(QString,uint,uint)));

}

void DeviceObserver::InitTcpServer()
{
    m_pTcpServer = m_oTcpServerInstanceGetter.GetInstance();

    m_pTcpServer->setMaxPendingConnections(30);

    connect(m_pTcpServer, SIGNAL(sig_ConnectClient(int,QString,quint16)),
            this, SLOT(slot_ConnectClient(int,QString,quint16)));
    connect(m_pTcpServer, SIGNAL(sig_SockDisConnect(int,QString,quint16)),
            this, SLOT(slot_DisConnectClient(int,QString,quint16)));
}

void DeviceObserver::WorkSleep(ushort un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

bool DeviceObserver::GetFWDataPointer(const ushort &us_SequenceNumber,
                                      FWUploadData * &p_FWUploadData)
{
    if(m_listFWUploadData.isEmpty()){
        return false;
    }

    foreach(FWUploadData *p_FWData, m_listFWUploadData){
        if(p_FWData->us_SequenceNumber == us_SequenceNumber){
            p_FWUploadData = p_FWData;
            return true;
        }
    }
    return false;
}

bool DeviceObserver::GetLastFWDataPointer(const ushort &us_SequenceNumber,
                                            FWUploadData * &p_FWUploadData)
{
    if(m_listLastFWUploadData.isEmpty()){
        return false;
    }

    foreach(FWUploadData *p_FWData, m_listLastFWUploadData){
        if(p_FWData->us_SequenceNumber == us_SequenceNumber){
            p_FWUploadData = p_FWData;
            return true;
        }
    }
    return false;
}

bool DeviceObserver::FWConfirm(STRUCT_COMINFO &struct_ComInfo)
{
    foreach(STRUCT_COMINFO struct_NeedFW, m_listNeedFW){
        if(struct_ComInfo.str_PortName == struct_NeedFW.str_PortName ||
                (struct_ComInfo.un_Pid == struct_NeedFW.un_Pid &&
                 struct_ComInfo.un_Vid == struct_NeedFW.un_Vid)){
            return true;
        }
    }
    return false;
}

bool DeviceObserver::AddFW(const QString &str_PortName,
                           const uint &un_Pid,
                           const uint &un_Vid)
{
    Firmware *p_Firmware = new Firmware;

    connect(p_Firmware, SIGNAL(sig_ReceiveCommand(ushort,uchar,QByteArray,uint)),
            this, SLOT(slot_ReceiveCommand(ushort,uchar,QByteArray,uint)));
    connect(p_Firmware, SIGNAL(sig_WriteCommand(ushort,uchar,QByteArray,uint)),
            this, SLOT(slot_WriteCommand(ushort,uchar,QByteArray,uint)));
    connect(p_Firmware, SIGNAL(sig_GetFWInfo(ushort,STRUCT_FWINFO)),
            this, SLOT(slot_GetFWInfo(ushort,STRUCT_FWINFO)));
    connect(p_Firmware, SIGNAL(sig_TestTimeOut(ushort,int)),
            this, SLOT(slot_TestTimeOut(ushort,int)));
    connect(p_Firmware, SIGNAL(sig_MachineState(ushort,ENUM_MACHINEDUTSTATE,ENUM_MACHINETESTSTATE)),
            this, SLOT(slot_MachineState(ushort,ENUM_MACHINEDUTSTATE,ENUM_MACHINETESTSTATE)));
    connect(p_Firmware, SIGNAL(sig_AdjustPower(ushort,short,short,short)),
            this, SLOT(slot_AdjustPower(ushort,short,short,short)));
    connect(p_Firmware, SIGNAL(sig_ReadPower(ushort,QList<short>,QList<short>,QList<short>,QList<char>,short,short)),
            this, SLOT(slot_ReadPower(ushort,QList<short>,QList<short>,QList<short>,QList<char>,short,short)));
    connect(p_Firmware, SIGNAL(sig_PowerSelfTest(ushort,QList<short>)),
            this, SLOT(slot_PowerSelfTest(ushort,QList<short>)));
    connect(p_Firmware, SIGNAL(sig_FWBoot(ushort,STRUCT_FWINFO)),
            this, SLOT(slot_FWBoot(ushort,STRUCT_FWINFO)));
    connect(p_Firmware, SIGNAL(sig_FWIntact(ushort,ENUM_MSGDEFINE)),
            this, SLOT(slot_FWIntact(ushort,ENUM_MSGDEFINE)));
    connect(p_Firmware, SIGNAL(sig_HandDataBag(ushort,STRUCT_HANDBAG)),
            this, SLOT(slot_HandDataBag(ushort,STRUCT_HANDBAG)));
    connect(p_Firmware, SIGNAL(sig_StartTestNotice(ushort,char,char,char,QList<char>)),
            this, SLOT(slot_StartTestNotice(ushort,char,char,char,QList<char>)));
    connect(p_Firmware, SIGNAL(sig_CompleteTest(ushort,QList<char>)),
            this, SLOT(slot_CompleteTest(ushort,QList<char>)));
    connect(p_Firmware, SIGNAL(sig_StartOneGroupPowerTest(ushort,char,int)),
            this, SLOT(slot_StartOneGroupPowerTest(ushort,char,int)));
    connect(p_Firmware, SIGNAL(sig_StartOneGroupEnumTest(ushort,char,int)),
            this, SLOT(slot_StartOneGroupEnumTest(ushort,char,int)));
    connect(p_Firmware, SIGNAL(sig_UploadRFPowerResult(ushort,QList<short>)),
            this, SLOT(slot_UploadRFPowerResult(ushort,QList<short>)));

    p_Firmware->SetFirmwareConfig(str_PortName,
                                  un_Pid,
                                  un_Vid);

    if(m_OpenFWModel == BYCOM){
        p_Firmware->SetOpenComModelByCom();
    }
    else if(m_OpenFWModel == BYPIDVID){
        p_Firmware->SetOpenComModelByPidVid();
    }
    else{
        delete p_Firmware;
        p_Firmware = NULL;
    }

    bool b_Ret = p_Firmware->OpenFirmware();

    if(!b_Ret){
        delete p_Firmware;
        p_Firmware = NULL;
        return false;
    }


    //wait fot that get bootInfo(SequenceNumber) from fw
    WorkSleep(800);

    ushort us_SequenceNumber = 0;
    if(!p_Firmware->GetSequenceNumber(us_SequenceNumber)){
        delete p_Firmware;
        p_Firmware = NULL;
        return false;
    }

    if(!AddFWUploadDataObject(us_SequenceNumber, str_PortName)){
        delete p_Firmware;
        p_Firmware = NULL;
        return false;
    }

    m_listFirmware.append(p_Firmware);
    return true;
}

bool DeviceObserver::AddFWUploadDataObject(const ushort &us_SequenceNumber,
                                           const QString &str_PortName)
{
    if(m_listFWUploadData.isEmpty()){
        FWUploadData *p_FWUploadData = new FWUploadData;
        p_FWUploadData->us_SequenceNumber = us_SequenceNumber;
        p_FWUploadData->str_PortName = str_PortName;
        m_listFWUploadData.append(p_FWUploadData);

        FWUploadData *p_LastFWUploadData = new FWUploadData;
        p_LastFWUploadData->us_SequenceNumber = us_SequenceNumber;
        p_LastFWUploadData->str_PortName = str_PortName;
        m_listLastFWUploadData.append(p_LastFWUploadData);
    }
    else{
        foreach(FWUploadData *p_FWData, m_listFWUploadData){
            if(p_FWData->us_SequenceNumber == us_SequenceNumber ||
                    p_FWData->str_PortName == str_PortName){
                return false;
            }
        }

        FWUploadData *p_FWUploadData = new FWUploadData;
        p_FWUploadData->us_SequenceNumber = us_SequenceNumber;
        p_FWUploadData->str_PortName = str_PortName;
        m_listFWUploadData.append(p_FWUploadData);

        FWUploadData *p_LastFWUploadData = new FWUploadData;
        p_LastFWUploadData->us_SequenceNumber = us_SequenceNumber;
        p_LastFWUploadData->str_PortName = str_PortName;
        m_listLastFWUploadData.append(p_LastFWUploadData);
    }
    return true;
}

bool DeviceObserver::RemoveFW(const QString &str_PortName,
                              const uint &un_Pid,
                              const uint &un_Vid)
{
    QString str_FWPortName = "";
    uint un_FWPid = 0;
    uint un_FWVid = 0;
    ushort us_SequenceNumber = 0;

    if(m_listFirmware.isEmpty()){
        return false;
    }

    foreach(Firmware *p_FW, m_listFirmware){
        p_FW->GetFirmwareConfig(str_FWPortName,
                                un_FWPid,
                                un_FWVid);
        if(str_PortName == str_FWPortName &&
                un_Pid == un_FWPid &&
                un_Vid == un_FWVid){
            p_FW->GetSequenceNumber(us_SequenceNumber);
            p_FW->CloseFirmware();
            p_FW->ExitFirmware();
            m_listFirmware.removeAll(p_FW);

/*
            disconnect(p_FW, SIGNAL(sig_GetFWInfo(ushort,STRUCT_FWINFO)),
                    this, SLOT(slot_GetFWInfo(ushort,STRUCT_FWINFO)));
            disconnect(p_FW, SIGNAL(sig_TestTimeOut(ushort,int)),
                    this, SLOT(slot_TestTimeOut(ushort,int)));
            disconnect(p_FW, SIGNAL(sig_MachineState(ushort,ENUM_MACHINEDUTSTATE,ENUM_MACHINETESTSTATE)),
                    this, SLOT(slot_MachineState(ushort,ENUM_MACHINEDUTSTATE,ENUM_MACHINETESTSTATE)));
            disconnect(p_FW, SIGNAL(sig_AdjustPower(ushort,short,short,short)),
                    this, SLOT(slot_AdjustPower(ushort,short,short,short)));
            disconnect(p_FW, SIGNAL(sig_ReadPower(ushort,QList<short>,QList<short>,QList<short>,QList<char>,short,short)),
                    this, SLOT(slot_ReadPower(ushort,QList<short>,QList<short>,QList<short>,QList<char>,short,short)));
            disconnect(p_FW, SIGNAL(sig_PowerSelfTest(ushort,QList<short>)),
                    this, SLOT(slot_PowerSelfTest(ushort,QList<short>)));
            disconnect(p_FW, SIGNAL(sig_FWBoot(ushort,STRUCT_FWINFO)),
                    this, SLOT(slot_FWBoot(ushort,STRUCT_FWINFO)));
            disconnect(p_FW, SIGNAL(sig_FWIntact(ushort,ENUM_MSGDEFINE)),
                    this, SLOT(slot_FWIntact(ushort,ENUM_MSGDEFINE)));
            disconnect(p_FW, SIGNAL(sig_HandDataBag(ushort,STRUCT_HANDBAG)),
                    this, SLOT(slot_HandDataBag(ushort,STRUCT_HANDBAG)));
            disconnect(p_FW, SIGNAL(sig_StartTestNotice(ushort,char,char,char,QList<char>)),
                    this, SLOT(slot_StartTestNotice(ushort,char,char,char,QList<char>)));
            disconnect(p_FW, SIGNAL(sig_CompleteTest(ushort,QList<char>)),
                    this, SLOT(slot_CompleteTest(ushort,QList<char>)));
            disconnect(p_FW, SIGNAL(sig_StartOneGroupPowerTest(ushort,char,int)),
                    this, SLOT(slot_StartOneGroupPowerTest(ushort,char,int)));
            disconnect(p_FW, SIGNAL(sig_StartOneGroupEnumTest(ushort,char,int)),
                    this, SLOT(slot_StartOneGroupEnumTest(ushort,char,int)));
            disconnect(p_FW, SIGNAL(sig_UploadRFPowerResult(ushort,QList<short>)),
                    this, SLOT(slot_UploadRFPowerResult(ushort,QList<short>)));
*/

            delete p_FW;
            p_FW = NULL;
        }
    }

    return RemoveFWUploadDataObject(us_SequenceNumber, str_PortName);
}


bool DeviceObserver::RemoveFWUploadDataObject(const ushort &us_SequenceNumber,
                                              const QString &str_PortName)
{
    if(us_SequenceNumber == 0 &&
            m_listFWUploadData.isEmpty() &&
            m_listLastFWUploadData.isEmpty()){
        return false;
    }

    foreach(FWUploadData *p_FWData, m_listFWUploadData){
        if(p_FWData->str_PortName == str_PortName &&
                p_FWData->us_SequenceNumber == us_SequenceNumber){
            m_listFWUploadData.removeAll(p_FWData);
            delete p_FWData;
            p_FWData = NULL;
        }
    }

    foreach(FWUploadData *p_FWData, m_listLastFWUploadData){
        if(p_FWData->str_PortName == str_PortName &&
                p_FWData->us_SequenceNumber == us_SequenceNumber){
            m_listLastFWUploadData.removeAll(p_FWData);
            delete p_FWData;
            p_FWData = NULL;
        }
    }

    return false;
}

bool DeviceObserver::RemoveAllFWAndFWData()
{
    if(!m_listFirmware.isEmpty()){
        foreach(Firmware *p_FW, m_listFirmware){
            m_listFirmware.removeAll(p_FW);
            p_FW->CloseFirmware();
            p_FW->ExitFirmware();
            delete p_FW;
            p_FW = NULL;
        }
    }

    if(!m_listFWUploadData.isEmpty()){
        foreach(FWUploadData *p_FWData, m_listFWUploadData){
            m_listFWUploadData.removeAll(p_FWData);
            delete p_FWData;
            p_FWData = NULL;
        }
    }

    if(!m_listLastFWUploadData.isEmpty()){
        foreach(FWUploadData *p_FWData, m_listLastFWUploadData){
            m_listLastFWUploadData.removeAll(p_FWData);
            delete p_FWData;
            p_FWData = NULL;
        }
    }

    return true;
}

bool DeviceObserver::UpdataLastFWTestData(const ushort &us_SequenceNumber)
{
    FWUploadData *p_FWCurrentData;
    FWUploadData *p_FWLastData;

    foreach(FWUploadData *p_FWData, m_listLastFWUploadData){
        if(p_FWData->us_SequenceNumber == us_SequenceNumber){
            p_FWLastData = p_FWData;
        }
    }

    foreach(FWUploadData *p_FWData, m_listFWUploadData){
        if(p_FWData->us_SequenceNumber == us_SequenceNumber){
            p_FWCurrentData = p_FWData;
        }
    }

    p_FWLastData == p_FWCurrentData;


    ReplaceUsbControlResult(us_SequenceNumber,
                            m_mapOpenDeviceResult,
                            m_mapLastOpenDeviceResult);

    ReplaceUsbControlResult(us_SequenceNumber,
                            m_mapEnumResult,
                            m_mapLastEnumResult);

    ReplaceUsbControlResult(us_SequenceNumber,
                            m_mapSendCmdResult,
                            m_mapLastSendCmdResult);

//    m_mapLastOpenDeviceResult = m_mapOpenDeviceResult;
//    m_mapLastEnumResult = m_mapEnumResult;
//    m_mapLastSendCmdResult = m_mapSendCmdResult;

    p_FWCurrentData->ClearFWData();

//    m_mapOpenDeviceResult.clear();
//    m_mapEnumResult.clear();
//    m_mapSendCmdResult.clear();

    return false;
}

bool DeviceObserver::TransformMaskCode(const ushort &us_SequenceNumber,
                                       const int &n_MaskData,
                                       QList<int> &list_MaskCode)
{
    int n_UpperSequence = (us_SequenceNumber - 1)*OneGroupUsbNumber;
    int n_CheckBit = 0x01;
    for(int i = 0; i < (int)(sizeof(n_MaskData)*8); i++){
        if(n_MaskData&n_CheckBit){
            list_MaskCode.append(i + n_UpperSequence + 1);
        }
        else{
            list_MaskCode.append(0);
        }
        n_CheckBit<<1;
    }
    return true;
}

bool DeviceObserver::ReplaceUsbControlResult(const ushort &us_SequenceNumber,
                                             QMap<int,bool> &map_FromResultData,
                                             QMap<int,bool> &map_ToResultData)
{
    int n_UpperSequence = (us_SequenceNumber - 1)*OneGroupUsbNumber;

    for(int i = 0; i < OneGroupUsbNumber; i++){
        if(map_FromResultData.contains(i + n_UpperSequence + 1)){
            map_ToResultData.insert(i + n_UpperSequence + 1,
                                       map_FromResultData.value(i + n_UpperSequence + 1));
        }
        else{
            map_ToResultData.insert(i + n_UpperSequence + 1, false);
        }
    }
    return true;
}

bool DeviceObserver::AddBox(const QString &str_IP)
{
    for(int i = 0; i < m_listBoxIP.count(); i++){
        if(m_listBoxIP.at(i) == str_IP){
            Box *p_Box = new Box;
            p_Box->SetIP(str_IP);
            p_Box->SetSequenceNumber(i+1);

            connect(p_Box, SIGNAL(sig_BoxOperator(ushort,BOX_OPERATOR)),
                    this, SLOT(slot_BoxOperator(ushort,BOX_OPERATOR)));

            m_listBox.append(p_Box);
        }
    }
    return true;
}

bool DeviceObserver::RemoveBox(const QString &str_IP)
{
    QString str_ExistIP;
    foreach(Box *p_Box, m_listBox){
        p_Box->GetIP(str_ExistIP);
        if(str_ExistIP == str_IP){
            m_listBox.removeAll(p_Box);

            disconnect(p_Box, SIGNAL(sig_BoxOperator(ushort,BOX_OPERATOR)),
                       this, SLOT(slot_BoxOperator(ushort,BOX_OPERATOR)));

            delete p_Box;
            p_Box = NULL;
        }
    }
    return true;
}

bool DeviceObserver::AddCatchRobot(const QString &str_IP)
{
    m_pCatchRobot = new CatchRobot;
    m_pCatchRobot->SetIP(str_IP);
    return true;
}

bool DeviceObserver::RemoveCatchRobot(const QString &str_IP)
{
    QString str_ExistIP;
    if(m_pCatchRobot != NULL){
        m_pCatchRobot->GetIP(str_ExistIP);

        if(str_ExistIP == str_IP){
            delete m_pCatchRobot;
            m_pCatchRobot = NULL;
        }
    }
    return true;
}

void DeviceObserver::slot_FirmwareDiscoverd(QString str_Port,
                                            uint un_Pid,
                                            uint un_Vid)
{
    STRUCT_COMINFO struct_ComInfo;
    struct_ComInfo.str_PortName = str_Port;
    struct_ComInfo.un_Pid = un_Pid;
    struct_ComInfo.un_Vid = un_Vid;

    qDebug()<<"discoverd port"<<str_Port;
    qDebug()<<"discoverd pid"<<un_Pid;
    qDebug()<<"discoverd vid"<<un_Vid;

    if(FWConfirm(struct_ComInfo)){
        if(!AddFW(str_Port, un_Pid, un_Vid)){
            return;
        }
    }

    emit sig_FirmwareDiscoverd();
}

void DeviceObserver::slot_FirmwareRemove(QString str_Port,
                                         uint un_Pid,
                                         uint un_Vid)
{
    STRUCT_COMINFO struct_ComInfo;
    struct_ComInfo.str_PortName = str_Port;
    struct_ComInfo.un_Pid = un_Pid;
    struct_ComInfo.un_Vid = un_Vid;

    qDebug()<<"remove port"<<str_Port;
    qDebug()<<"remove pid"<<un_Pid;
    qDebug()<<"remove vid"<<un_Vid;

    if(FWConfirm(struct_ComInfo)){
        RemoveFW(str_Port, un_Pid, un_Vid);
    }

    emit sig_FirmwareRemove();
}

void DeviceObserver::slot_ConnectClient(int n_ID,
                                        QString str_IP,
                                        quint16 us_Port)
{
    Q_UNUSED(n_ID);
    Q_UNUSED(us_Port);

    if(m_listBoxIP.contains(str_IP)){
        AddBox(str_IP);
        emit sig_BoxDiscoverd();
    }
    else if(str_IP == m_CatchRobotIP){
        AddCatchRobot(str_IP);
        emit sig_CatchRobotDiscoverd();
    }
}

void DeviceObserver::slot_DisConnectClient(int n_ID,
                                           QString str_IP,
                                           quint16 us_Port)
{
    Q_UNUSED(n_ID);
    Q_UNUSED(us_Port);

    if(m_listBoxIP.contains(str_IP)){
        RemoveBox(str_IP);
        emit sig_BoxRemove();
    }
    else if(str_IP == m_CatchRobotIP){
        RemoveCatchRobot(str_IP);
        sig_CatchRobotRemove();
    }
}

void DeviceObserver::slot_EnumUsbComplete()
{
    m_pUsbControl->GetEnumResult(m_mapEnumResult);

    emit sig_EnumUsbComplete();
}

void DeviceObserver::slot_SendPowerTestComplete()
{
    m_pUsbControl->GetTestResult(m_mapEnumResult,
                                 m_mapOpenDeviceResult,
                                 m_mapSendCmdResult);

    emit sig_SendPowerTestComplete();
}


void DeviceObserver::slot_ReceiveCommand(ushort us_SequenceNumber,
                                         uchar uc_Command,
                                         QByteArray byte_Data,
                                         uint un_DataLength)
{
    STRUCT_COMMANDANDDATA struct_CommandAndData;

    if(m_listReceiveCommandAndData.isEmpty()){
        struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
        struct_CommandAndData.uc_Command = uc_Command;
        memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
        struct_CommandAndData.un_DataLength = un_DataLength;

        m_listReceiveCommandAndData.append(struct_CommandAndData);
    }
    else{
        for(int i = 0; i < m_listReceiveCommandAndData.count(); i++){
            if(m_listReceiveCommandAndData.at(i).us_SequenceNumber == us_SequenceNumber &&
                    m_listReceiveCommandAndData.at(i).uc_Command == uc_Command){
//                m_listReceiveCommandAndData.removeAt(i);

                struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
                struct_CommandAndData.uc_Command = uc_Command;
                memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
                struct_CommandAndData.un_DataLength = un_DataLength;

                m_listReceiveCommandAndData.replace(i, struct_CommandAndData);
                emit sig_ReceiveCmd(us_SequenceNumber, uc_Command);
                return;
            }
        }
/*
        foreach(STRUCT_COMMANDANDDATA struct_Data, m_listReceiveCommandAndData){
            if(struct_Data.us_SequenceNumber == us_SequenceNumber&&
                    struct_Data.c_Command == c_Command){
                m_listReceiveCommandAndData.removeAll(struct_Data);

                struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
                struct_CommandAndData.c_Command = c_Command;
                memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
                struct_CommandAndData.un_DataLength = un_DataLength;

                m_listReceiveCommandAndData.append(struct_CommandAndData);

                return;
            }
        }
*/
        struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
        struct_CommandAndData.uc_Command = uc_Command;
        memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
        struct_CommandAndData.un_DataLength = un_DataLength;

        m_listReceiveCommandAndData.append(struct_CommandAndData);
    }

    emit sig_ReceiveCmd(us_SequenceNumber, uc_Command);
}

void DeviceObserver::slot_WriteCommand(ushort us_SequenceNumber,
                                       uchar uc_Command,
                                       QByteArray byte_Data,
                                       uint un_DataLength)
{
    STRUCT_COMMANDANDDATA struct_CommandAndData;
    if(m_listWriteCommandAndData.isEmpty()){
        struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
        struct_CommandAndData.uc_Command = uc_Command;
        memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
        struct_CommandAndData.un_DataLength = un_DataLength;

        m_listWriteCommandAndData.append(struct_CommandAndData);
    }
    else{
        for(int i = 0; i < m_listWriteCommandAndData.count(); i++){
            if(m_listWriteCommandAndData.at(i).us_SequenceNumber == us_SequenceNumber &&
                    m_listWriteCommandAndData.at(i).uc_Command == uc_Command){
//                m_listWriteCommandAndData.removeAt(i);

                struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
                struct_CommandAndData.uc_Command = uc_Command;
                memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
                struct_CommandAndData.un_DataLength = un_DataLength;

                m_listWriteCommandAndData.replace(i, struct_CommandAndData);
                emit sig_WriteCmd(us_SequenceNumber, uc_Command);
                return;
            }
        }
/*
        foreach(STRUCT_COMMANDANDDATA struct_Data, m_listWriteCommandAndData){
            if(struct_Data.us_SequenceNumber == us_SequenceNumber &&
               struct_Data.uc_Command == uc_Command){
                m_listWriteCommandAndData.removeAll(struct_Data);

                struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
                struct_CommandAndData.uc_Command = uc_Command;
                memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
                struct_CommandAndData.un_DataLength = un_DataLength;

                m_listWriteCommandAndData.append(struct_CommandAndData);

                return;
            }
        }
*/
        struct_CommandAndData.us_SequenceNumber = us_SequenceNumber;
        struct_CommandAndData.uc_Command = uc_Command;
        memcpy(struct_CommandAndData.DATA, byte_Data.data(), un_DataLength);
        struct_CommandAndData.un_DataLength = un_DataLength;

        m_listWriteCommandAndData.append(struct_CommandAndData);
    }
    emit sig_WriteCmd(us_SequenceNumber, uc_Command);
}

void DeviceObserver::slot_GetFWInfo(ushort us_SequenceNumber,
                                    STRUCT_FWINFO struct_FWInfo)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->struct_GetFWInfo = struct_FWInfo;

    emit sig_GetFWInfo(us_SequenceNumber);
}

void DeviceObserver::slot_TestTimeOut(ushort us_SequenceNumber,
                                      int n_TestTimeOut)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->n_TestTimeOut = n_TestTimeOut;

    emit sig_TestTimeOut(us_SequenceNumber);
}

void DeviceObserver::slot_MachineState(ushort us_SequenceNumber,
                                       ENUM_MACHINEDUTSTATE DUTState,
                                       ENUM_MACHINETESTSTATE TestState)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->MachineDUTState = DUTState;
    p_FWData->MachineTestState = TestState;

    emit sig_MachineState(us_SequenceNumber);
}

void DeviceObserver::slot_AdjustPower(ushort us_SequenceNumber,
                                      short s_Vslop,
                                      short s_VslopAdc,
                                      short s_Intercept)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->s_AdjustPowerVslop = s_Vslop;
    p_FWData->s_AdjustPowerVslopAdc = s_VslopAdc;
    p_FWData->s_AdjustPowerIntercept = s_Intercept;

    emit sig_AdjustPower(us_SequenceNumber);
}

void DeviceObserver::slot_ReadPower(ushort us_SequenceNumber,
                                    QList<short> list_Vslop,
                                    QList<short> list_VslopAdc,
                                    QList<short> list_Intercept,
                                    QList<char> list_db_Compensation,
                                    short s_ReferenceVoltage_1,
                                    short s_ReferenceVoltage_2)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->list_ReadPowerVslop = list_Vslop;
    p_FWData->list_ReadPowerVslopAdc = list_VslopAdc;
    p_FWData->list_ReadPowerIntercept = list_Intercept;
    p_FWData->list_ReadPower_db_Compensation = list_db_Compensation;
    p_FWData->s_ReadPowerReferenceVoltage_1 = s_ReferenceVoltage_1;
    p_FWData->s_ReadPowerReferenceVoltage_2 = s_ReferenceVoltage_2;

    emit sig_ReadPower(us_SequenceNumber);
}

void DeviceObserver::slot_PowerSelfTest(ushort us_SequenceNumber,
                                        QList<short> list_SelfVoltage)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->list_PowerSelfTestSelfVoltage = list_SelfVoltage;

    emit sig_PowerSelfTest(us_SequenceNumber);
}

void DeviceObserver::slot_FWBoot(ushort us_SequenceNumber,
                                 STRUCT_FWINFO struct_FWInfo)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->struct_FWBoot = struct_FWInfo;

    emit sig_FWBoot(us_SequenceNumber);
}

void DeviceObserver::slot_FWIntact(ushort us_SequenceNumber,
                                   ENUM_MSGDEFINE FWIntact)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->FWIntact = FWIntact;

    sig_FWIntact(us_SequenceNumber);
}

void DeviceObserver::slot_HandDataBag(ushort us_SequenceNumber,
                                      STRUCT_HANDBAG struct_HandBag)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->struct_HandBag = struct_HandBag;

    emit sig_HandDataBag(us_SequenceNumber);
}

void DeviceObserver::slot_StartTestNotice(ushort us_SequenceNumber,
                                          char c_TestType,
                                          char c_TotalGroup,
                                          char c_AmoutOfAGroup,
                                          QList<char> list_UsefulDUT)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->c_StartTestNoticeTestType = c_TestType;
    p_FWData->c_StartTestNoticeTotalGroup = c_TotalGroup;
    p_FWData->c_StartTestNoticAmoutOfAGroup = c_AmoutOfAGroup;
    p_FWData->list_StartTestNoticUsefulDUT = list_UsefulDUT;

    emit sig_StartTestNotice(us_SequenceNumber);
}

void DeviceObserver::slot_CompleteTest(ushort us_SequenceNumber,
                                       QList<char> list_UsefulResult)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->list_CompleteTestUsefulResult = list_UsefulResult;

    //UpdataTestData
    UpdataLastFWTestData(us_SequenceNumber);

    emit sig_CompleteTest(us_SequenceNumber);
}

void DeviceObserver::slot_StartOneGroupPowerTest(ushort us_SequenceNumber,
                                    char c_SurplusGroup,
                                    int n_TestDUTMask)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->c_StartOneGroupPowerTestSurplusGroup = c_SurplusGroup;
    p_FWData->n_StartOneGroupPowerTestDUTMask = n_TestDUTMask;

    emit sig_StartOneGroupPowerTest(us_SequenceNumber);
}

void DeviceObserver::slot_StartOneGroupEnumTest(ushort us_SequenceNumber,
                                                char c_SurplusGroup,
                                                int n_TestDUTMask)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->c_StartOneGroupEnumTestSurplusGroup = c_SurplusGroup;
    p_FWData->n_StartOneGroupEnumTestDUTMask = n_TestDUTMask;

    emit sig_StartOneGroupEnumTest(us_SequenceNumber);
}

void DeviceObserver::slot_UploadRFPowerResult(ushort us_SequenceNumber,
                                              QList<short> list_Power_db)
{
    FWUploadData *p_FWData = NULL;
    if(!GetFWDataPointer(us_SequenceNumber, p_FWData)){
        return;
    }

    p_FWData->list_UploadRFPowerResult_db = list_Power_db;

    emit sig_UploadRFPowerResult(us_SequenceNumber);
}

void DeviceObserver::slot_BoxOperator(ushort us_SequenceNumber,
                                      BOX_OPERATOR box_Operator)
{
    m_mapBoxOperator.insert(us_SequenceNumber,
                            box_Operator);

    emit sig_BoxOperatorUpdata(us_SequenceNumber);
}
