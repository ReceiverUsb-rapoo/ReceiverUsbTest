#include "firewaredebug.h"
#include "ui_firewaredebug.h"

FirewareDebug::FirewareDebug(ushort us_SequenceNumber, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirewareDebug)
{
    ui->setupUi(this);
    m_pDeviceObserver = NULL;
    m_pDeviceOperator = NULL;
    m_usSequenceNumber = us_SequenceNumber;
    m_bAllDeviceEnum = false;

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("FirewareDebug-夹具-" + QString::number(m_usSequenceNumber));
    this->setFixedSize(this->size());

    InitFirewareDebug();
}

FirewareDebug::~FirewareDebug()
{
    ExitDebugModel();
    delete ui;
}

void FirewareDebug::SetSequenceNumber(ushort us_SequenceNumber)
{
    m_usSequenceNumber = us_SequenceNumber;
    this->setWindowTitle("FirewareDebug-夹具-" + QString::number(m_usSequenceNumber));
}

void FirewareDebug::InitFirewareDebug()
{
    m_pDeviceObserver = m_oDeviceObserverInstanceGetter.GetInstance();
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();

    QList<Firmware *> list_FWObjectPointer;
    m_pDeviceObserver->GetFWObjectPointer(list_FWObjectPointer);
    m_pDeviceOperator->SetFWObjectPointer(list_FWObjectPointer);

    connect(m_pDeviceObserver, SIGNAL(sig_EnumUsbComplete()),
            this, SLOT(slot_EnumUsbComplete()));
    connect(m_pDeviceObserver, SIGNAL(sig_SendPowerTestComplete()),
            this, SLOT(slot_SendPowerTestComplete()));
    connect(m_pDeviceObserver, SIGNAL(sig_ReceiveCmd(ushort,uchar)),
            this, SLOT(slot_ReceiveCmd(ushort,uchar)));
    connect(m_pDeviceObserver, SIGNAL(sig_WriteCmd(ushort,uchar)),
            this, SLOT(slot_WriteCmd(ushort,uchar)));
    connect(m_pDeviceObserver, SIGNAL(sig_MachineState(ushort)),
            this, SLOT(slot_MachineState(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_PowerSelfTest(ushort)),
            this, SLOT(slot_PowerSelfTest(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_StartTestNotice(ushort)),
            this, SLOT(slot_StartTestNotice(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_TestTimeOut(ushort)),
            this, SLOT(slot_TestTimeOut(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupPowerTest(ushort)),
            this, SLOT(slot_StartOneGroupPowerTest(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupEnumTest(ushort)),
            this, SLOT(slot_StartOneGroupEnumTest(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_UploadRFPowerResult(ushort)),
            this, SLOT(slot_UploadRFPowerResult(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_CompleteTest(ushort)),
            this, SLOT(slot_CompleteTest(ushort)));

    connect(ui->pb_EnterDebugModel, &QPushButton::pressed,
            this, &FirewareDebug::EnterDebugModel);
    connect(ui->pb_ExitDebugModel, &QPushButton::pressed,
            this, &FirewareDebug::ExitDebugModel);
    connect(ui->pb_GetFWInfo, &QPushButton::pressed,
            this, &FirewareDebug::GetFWInfo);
    connect(ui->pb_RestartFW, &QPushButton::pressed,
            this, &FirewareDebug::RestartFW);
    connect(ui->pb_InquireMachineState, &QPushButton::pressed,
            this, &FirewareDebug::InquireMachineState);
    connect(ui->pb_TestPowerSelf, &QPushButton::pressed,
            this, &FirewareDebug::TestPowerSelf);
    connect(ui->pb_TestEnum_1, &QPushButton::pressed,
            this, &FirewareDebug::TestEnum_1);
    connect(ui->pb_TestEnum_2, &QPushButton::pressed,
            this, &FirewareDebug::TestEnum_2);
    connect(ui->pb_TestWholeDUT, &QPushButton::pressed,
            this, &FirewareDebug::TestWholeDUT);
    connect(ui->pb_ClearInfoShow, &QPushButton::pressed,
            this, &FirewareDebug::ClearInfoShow);

    connect(ui->pb_PowerOn_1, &QPushButton::pressed,
            this, &FirewareDebug::PowerOn_1);
    connect(ui->pb_PowerOn_2, &QPushButton::pressed,
            this, &FirewareDebug::PowerOn_2);
    connect(ui->pb_PowerOff_1, &QPushButton::pressed,
            this, &FirewareDebug::PowerOff_1);
    connect(ui->pb_PowerOff_2, &QPushButton::pressed,
            this, &FirewareDebug::PowerOff_2);

    ConfigFile o_ConfigFile;
    o_ConfigFile.GetUsbControlConfig(m_usSequenceNumber,
                                     m_structUsbControlConfig);
}

void FirewareDebug::ExitFirewareDebug()
{
    disconnect(m_pDeviceObserver, SIGNAL(sig_EnumUsbComplete()),
            this, SLOT(slot_EnumUsbComplete()));
    disconnect(m_pDeviceObserver, SIGNAL(sig_SendPowerTestComplete()),
            this, SLOT(slot_SendPowerTestComplete()));
    disconnect(m_pDeviceObserver, SIGNAL(sig_ReceiveCmd(ushort,uchar)),
            this, SLOT(slot_ReceiveCmd(ushort,uchar)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_WriteCmd(ushort,uchar)),
            this, SLOT(slot_WriteCmd(ushort,uchar)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_MachineState(ushort)),
            this, SLOT(slot_MachineState(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_PowerSelfTest(ushort)),
            this, SLOT(slot_PowerSelfTest(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_StartTestNotice(ushort)),
            this, SLOT(slot_StartTestNotice(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_TestTimeOut(ushort)),
            this, SLOT(slot_TestTimeOut(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupPowerTest(ushort)),
            this, SLOT(slot_StartOneGroupPowerTest(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_StartOneGroupEnumTest(ushort)),
            this, SLOT(slot_StartOneGroupEnumTest(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_UploadRFPowerResult(ushort)),
            this, SLOT(slot_UploadRFPowerResult(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_CompleteTest(ushort)),
               this, SLOT(slot_CompleteTest(ushort)));
}

void FirewareDebug::ShowDataByMap(QString str_DataName,
                                  QMap<int, bool> &map_Data,
                                  QString str_Unit)
{
//   int n_Number = map_Data.count();
    QString str_Info = "";
    int n_UpperSequence = (m_usSequenceNumber - 1)*OneGroupUsbNumber;

    str_Info += str_DataName + "\n";

    for(int i = 1; i <= OneGroupUsbNumber; i++){
        str_Info += "编号" + AutoCoverDouble(i + n_UpperSequence) + " : " +
                QString::number(map_Data.value(i + n_UpperSequence)) +
                str_Unit +
                "\n";
    }

    ShowInfo(str_Info);
}

QString FirewareDebug::AutoCoverDouble(int n_Data)
{
    QString str_Data = QString::number(n_Data);
    if(str_Data.count() == 1){
        str_Data += "  ";
    }

    return str_Data;
}

QString FirewareDebug::AutoCoverDouble(QString str_Info)
{
    if(str_Info.count() == 1){
        str_Info = "0" + str_Info;
    }

    return str_Info;
}

void FirewareDebug::ShowInfo(QString str_Info)
{
    QDateTime o_QDateTime = QDateTime::currentDateTime();
    QString str_InfoAddTime = QString("%1:%2:%3 %4\n").
                              arg(o_QDateTime.time().hour()).
                              arg(o_QDateTime.time().minute()).
                              arg(o_QDateTime.time().second()).
                              arg(str_Info);

    ui->tb_InfoShow->append(str_InfoAddTime);
}

bool FirewareDebug::InitUsbEnumAndSendTest()
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

bool FirewareDebug::StartUsbEnumTest(const uint &un_Pid,
                                     const uint &un_Vid,
                                     const int &n_DeviceNumber,
                                     const int &n_Time,
                                     const QMap<int, int> &map_StationPort)
{
    m_pDeviceOperator->SetEnumTestUsbConfig(un_Pid,
                                            un_Vid,
                                            n_DeviceNumber,
                                            n_Time,
                                            map_StationPort);

    qDebug()<<"n_Time FWD"<<n_Time;

    return m_pDeviceOperator->StartUsbEnumTest();
}

bool FirewareDebug::StartUsbSendTest(const QMap<int, int> &map_StationPort)
{
    m_pDeviceOperator->SetPowerTestConfig(map_StationPort);
    return m_pDeviceOperator->StartSendPowerTest();
}

bool FirewareDebug::ExitUsbEnumAndSendTest()
{
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

bool FirewareDebug::StartUsbEnumTestByOneGroup(const QList<int> &list_OneGroupPowerTest)
{
    QMap<int,int> map_StationPort;
    for(int i = 0; i< list_OneGroupPowerTest.count(); i++){
        if(list_OneGroupPowerTest.at(i) != 0){
            map_StationPort.insert(list_OneGroupPowerTest.at(i),
                                   m_structUsbControlConfig.
                                   map_StationPort.
                                   value(list_OneGroupPowerTest.at(i)));
        }
    }

    return StartUsbEnumTest(m_structUsbControlConfig.un_Pid,
                            m_structUsbControlConfig.un_Vid,
                            map_StationPort.count(),
                            m_structUsbControlConfig.n_Time,
                            map_StationPort);
}

bool FirewareDebug::StartUsbPowerTestByOneGroup(const QList<int> &list_OneGroupSendTest)
{
    QMap<int,int> map_StationPort;
    for(int i = 0; i< list_OneGroupSendTest.count(); i++){
        if(list_OneGroupSendTest.at(i) != 0){
            map_StationPort.insert(list_OneGroupSendTest.at(i),
                                   m_structUsbControlConfig.
                                   map_StationPort.
                                   value(list_OneGroupSendTest.at(i)));
        }
    }

    return StartUsbSendTest(map_StationPort);
}

bool FirewareDebug::GetOneGroupStationPort(const ENUM_POWERONGROUP &GroupStation,
                                           QMap<int, int> &map_OneGroupStationPort)
{
    int n_Total = 0;
    if(GroupStation == FIRSTGROUP){
        n_Total = 0 + (m_usSequenceNumber-1)*20;
    }
    else{
        n_Total = 10 + (m_usSequenceNumber-1)*20;
    }


    for(int i = 1 + n_Total; i <= 10 + n_Total; i++){
        if(m_structUsbControlConfig.map_StationPort.contains(i)){
            map_OneGroupStationPort.insert(i, m_structUsbControlConfig.map_StationPort.value(i));
        }
    }

    return true;
}

void FirewareDebug::WorkSleep(ushort un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void FirewareDebug::EnterDebugModel()
{
    m_pDeviceOperator->EnterDebugModel(m_usSequenceNumber);
}

void FirewareDebug::ExitDebugModel()
{
    m_pDeviceOperator->ExitDebugModel(m_usSequenceNumber);
}

void FirewareDebug::GetFWInfo()
{
    m_pDeviceOperator->GetFWInfo(m_usSequenceNumber);
}

void FirewareDebug::RestartFW()
{
    m_pDeviceOperator->RestartFW(m_usSequenceNumber);
}

void FirewareDebug::InquireMachineState()
{
    m_pDeviceOperator->InquireMachineState(m_usSequenceNumber);
}

void FirewareDebug::PowerOn_1()
{
    m_pDeviceOperator->PowerOnSwitch(m_usSequenceNumber,
                                     POWERON,
                                     FIRSTGROUP);
}

void FirewareDebug::PowerOn_2()
{
    m_pDeviceOperator->PowerOnSwitch(m_usSequenceNumber,
                                     POWERON,
                                     SECONDGROUP);
}

void FirewareDebug::PowerOff_1()
{
    m_pDeviceOperator->PowerOnSwitch(m_usSequenceNumber,
                                     POWEROFF,
                                     FIRSTGROUP);
}

void FirewareDebug::PowerOff_2()
{
    m_pDeviceOperator->PowerOnSwitch(m_usSequenceNumber,
                                     POWEROFF,
                                     SECONDGROUP);
}

void FirewareDebug::TestPowerSelf()
{
    m_pDeviceOperator->TestPowerSelfTest(m_usSequenceNumber);
}

void FirewareDebug::TestEnum_1()
{
    m_pDeviceObserver->ClearEnumResult();
    m_pDeviceOperator->TestEnum(m_usSequenceNumber,
                                FIRSTGROUP,
                                m_structUsbControlConfig.n_Time + 2000);
    m_bAllDeviceEnum = true;
    QMap<int,int> map_StationPort;
    GetOneGroupStationPort(FIRSTGROUP, map_StationPort);
    m_PowerTestGroup = FIRSTGROUP;

    InitUsbEnumAndSendTest();
    StartUsbEnumTest(m_structUsbControlConfig.un_Pid,
                     m_structUsbControlConfig.un_Vid,
                     10,
                     m_structUsbControlConfig.n_Time,
                     map_StationPort);

    ui->pb_TestEnum_1->setEnabled(false);
    ui->pb_TestEnum_2->setEnabled(false);
    ui->pb_TestWholeDUT->setEnabled(false);
}

void FirewareDebug::TestEnum_2()
{
    m_pDeviceObserver->ClearEnumResult();
    m_pDeviceOperator->TestEnum(m_usSequenceNumber,
                                SECONDGROUP,
                                m_structUsbControlConfig.n_Time + 2000);
    m_bAllDeviceEnum = true;

    QMap<int,int> map_StationPort;
    GetOneGroupStationPort(SECONDGROUP, map_StationPort);
    m_PowerTestGroup = SECONDGROUP;

    InitUsbEnumAndSendTest();
    StartUsbEnumTest(m_structUsbControlConfig.un_Pid,
                     m_structUsbControlConfig.un_Vid,
                     10,
                     m_structUsbControlConfig.n_Time,
                     map_StationPort);

    ui->pb_TestEnum_1->setEnabled(false);
    ui->pb_TestEnum_2->setEnabled(false);
    ui->pb_TestWholeDUT->setEnabled(false);
}

void FirewareDebug::TestSendPower()
{
    QMap<int,int> map_StationPort;
    GetOneGroupStationPort(m_PowerTestGroup, map_StationPort);

    StartUsbSendTest(map_StationPort);

    m_pDeviceObserver->ClearPowerSendResult();
}

void FirewareDebug::TestWholeDUT()
{
    m_pDeviceOperator->StartWholeDUTTest(m_usSequenceNumber);
    m_bAllDeviceEnum = false;

    ui->pb_TestEnum_1->setEnabled(false);
    ui->pb_TestEnum_2->setEnabled(false);
    ui->pb_TestWholeDUT->setEnabled(false);
}

void FirewareDebug::ClearInfoShow()
{
    ui->tb_InfoShow->clear();
}

void FirewareDebug::GetEnumUsbComplete()
{
    QMap<int, bool> map_EnumResult;
    m_pDeviceObserver->GetEnumUsbCompleteData(CURRENT, map_EnumResult);

    ShowDataByMap("枚举结果", map_EnumResult, "");
}

void FirewareDebug::GetSendPowerTestCompleteData()
{
    QMap<int, bool> map_EnumResult;
    QMap<int, bool> map_OpenDeviceResult;
    QMap<int, bool> map_SendCmdResult;
    m_pDeviceObserver->GetSendPowerTestCompleteData(CURRENT,
                                                    map_EnumResult,
                                                    map_OpenDeviceResult,
                                                    map_SendCmdResult);

    ShowDataByMap("枚举结果", map_EnumResult, "");
    ShowDataByMap("打开设备结果", map_OpenDeviceResult, "");
    ShowDataByMap("发送命令结果", map_SendCmdResult, "");
}

void FirewareDebug::GetReceiveCmdData(uchar uc_Command)
{
    STRUCT_COMMANDANDDATA struct_CommandAndData;
    QString str_Info;

    m_pDeviceObserver->GetReceiveCommandAndData(m_usSequenceNumber,
                                                uc_Command,
                                                struct_CommandAndData);

    str_Info += "receive cmd  \n" +
            QString::number(struct_CommandAndData.uc_Command, 16).toUpper() + "    ";

    for(int i = 0; i < (int)struct_CommandAndData.un_DataLength; i++){
        str_Info += AutoCoverDouble(QString::number((uchar)struct_CommandAndData.DATA[i] ,16)) + " ";
    }


    ShowInfo(str_Info);
}

void FirewareDebug::GetWriteCmdData(uchar uc_Command)
{
    STRUCT_COMMANDANDDATA struct_CommandAndData;
    QString str_Info;

    m_pDeviceObserver->GetWriteCommandAndData(m_usSequenceNumber,
                                              uc_Command,
                                              struct_CommandAndData);

    str_Info += "write cmd  \n" +
            QString::number(struct_CommandAndData.uc_Command, 16).toUpper() + "   ";

    for(int i = 0; i < (int)struct_CommandAndData.un_DataLength; i++){
        str_Info += AutoCoverDouble(QString::number((uchar)struct_CommandAndData.DATA[i], 16)) + " ";
    }

    ShowInfo(str_Info);
}

void FirewareDebug::GetMachineStateData()
{
    QString str_Info = "机器状态 \n";
    ENUM_MACHINEDUTSTATE DUTState;
    ENUM_MACHINETESTSTATE TestState;

    m_pDeviceObserver->GetMachineStateData(CURRENT,
                                           m_usSequenceNumber,
                                           DUTState,
                                           TestState);
    switch(DUTState){
    case DUTOUTFIXTURE:
        str_Info += "DUT状态离线\n";
        break;
    case DUTINFIXTURE:
        str_Info += "DUT状态在线\n";
         break;
    default:
        break;
    }

    switch(TestState){
    case UNSTART:
        str_Info += "测试状态 未开始";
        break;
    case WAIT:
        str_Info += "测试状态 等待";
         break;
    case TESTING:
        str_Info += "测试状态 测试中";
         break;
    case COMPLETE:
        str_Info += "测试状态 完成";
         break;
    default:
        break;
    }

    ShowInfo(str_Info);
}

void FirewareDebug::GetPowerSelfTestData()
{
    QString str_Info = "自检数据\n";
    QList<short> list_SelfVoltage;

    m_pDeviceObserver->GetPowerSelfTestData(CURRENT,
                                            m_usSequenceNumber,
                                            list_SelfVoltage);

    for(int i = 0; i < list_SelfVoltage.count(); i++){
        str_Info += "自检电压" + AutoCoverDouble(i) + "  " +
                QString::number(list_SelfVoltage.at(i)) + " \n";
    }

    ShowInfo(str_Info);
}

void FirewareDebug::GetStartTestNoticeData()
{
    QString str_Info = "测试开始通知\n";
    char c_TestType;
    char c_TotalGroup;
    char c_AmoutOfAGroup;
    QList<char> list_UsefulDUT;

    m_pDeviceObserver->GetStartTestNoticeData(CURRENT,
                                              m_usSequenceNumber,
                                              c_TestType,
                                              c_TotalGroup,
                                              c_AmoutOfAGroup,
                                              list_UsefulDUT);

    str_Info += "测试类型 " + QString::number((uchar)c_TestType) + "\n";
    str_Info += "测试总分组 " + QString::number((uchar)c_TotalGroup) + "\n";
    str_Info += "每组数量 " + QString::number((uchar)c_AmoutOfAGroup) + "\n";

    for(int i = 0; i < list_UsefulDUT.count(); i++){
        str_Info += "有效DUT" + AutoCoverDouble(i+1) + "  " +
                QString::number((uchar)list_UsefulDUT.at(i)) + "\n";
    }

    ShowInfo(str_Info);
}

void FirewareDebug::GetTestTimeOutData()
{
    QString str_Info = "测试超时时间\n";
    int n_TestTimeOut = 0;
    m_pDeviceObserver->GetTestTimeOutData(CURRENT,
                                          m_usSequenceNumber,
                                          n_TestTimeOut);

    str_Info += "时间: " + QString::number(n_TestTimeOut) + "ms";

    ShowInfo(str_Info);
}

void FirewareDebug::GetStartOneGroupPowerTestData(QList<int> &list_OneGroupPowerTestMaskCode)
{
    QString str_Info = "开始一组功率测试\n";
    char c_SurplusGroup;
    int n_TestDUTMask;

    m_pDeviceObserver->GetStartOneGroupPowerTestData(CURRENT,
                                                     m_usSequenceNumber,
                                                     c_SurplusGroup,
                                                     n_TestDUTMask,
                                                     list_OneGroupPowerTestMaskCode);

    str_Info += "剩余分组  " + QString::number((uchar)c_SurplusGroup, 16) + "\n";
    str_Info += "DUT掩码  " + QString::number(n_TestDUTMask, 16) + "\n";

    for(int i = 0; i < list_OneGroupPowerTestMaskCode.count(); i++){
        if(list_OneGroupPowerTestMaskCode.at(i) == 0){
            continue;
        }

        str_Info += "测试编号" + AutoCoverDouble(i + 1) + "  " +
                QString::number(list_OneGroupPowerTestMaskCode.at(i)) + "\n";
    }

    ShowInfo(str_Info);
}

void FirewareDebug::GetStartOneGroupEnumTestData(QList<int> &list_OneGroupEnumTestMaskCode)
{
    QString str_Info = "开始一组枚举测试\n";
    char c_SurplusGroup;
    int n_TestDUTMask;

    m_pDeviceObserver->GetStartOneGroupEnumTestData(CURRENT,
                                                    m_usSequenceNumber,
                                                    c_SurplusGroup,
                                                    n_TestDUTMask,
                                                    list_OneGroupEnumTestMaskCode);

    str_Info += "剩余分组  " + QString::number((uchar)c_SurplusGroup, 16) + "\n";
    str_Info += "DUT掩码   " + QString::number(n_TestDUTMask, 16) + "\n";

    for(int i = 0; i < list_OneGroupEnumTestMaskCode.count(); i++){
        if(list_OneGroupEnumTestMaskCode.at(i) == 0){
            continue;
        }

        str_Info += "测试编号" + AutoCoverDouble(i + 1) + "   " +
                QString::number(list_OneGroupEnumTestMaskCode.at(i)) + "\n";
    }

    ShowInfo(str_Info);
}

void FirewareDebug::GetUploadRFPowerResultData()
{
    QString str_Info = "RF功率检测\n";
    QList<short> list_Power_db;

    m_pDeviceObserver->GetUploadRFPowerResultData(CURRENT,
                                                  m_usSequenceNumber,
                                                  list_Power_db);

    QString str_Power_db = "";
    for(int i = 0; i < list_Power_db.count(); i++){
        str_Power_db = QString::number(list_Power_db.at(i));

        if(str_Power_db.length() > 1){
            str_Power_db = str_Power_db.insert(str_Power_db.length() - 1, QString("."));
        }
        else if(str_Power_db.length() == 1){
            str_Power_db += ".0";
        }
        else{
            str_Power_db = "0.0";
        }

        str_Info += "已校准检测功率db" +
                    AutoCoverDouble(i+1) +
                    "  " +
                    str_Power_db +
                    " db\n";
    }

    ShowInfo(str_Info);
}

void FirewareDebug::GetCompleteTestData()
{

    QString str_Info = "测试完成\n";
    QList<char> list_UsefulResult;
    m_pDeviceObserver->GetCompleteTestData(CURRENT,
                                           m_usSequenceNumber,
                                           list_UsefulResult);

    for(int i = 0; i < list_UsefulResult.count(); i++){
        str_Info += "有效结果DUT" + AutoCoverDouble(i + 1) + "  " +
                QString::number((uchar)list_UsefulResult.at(i)) + "\n";
    }

    ShowInfo(str_Info);
}

void FirewareDebug::slot_EnumUsbComplete()
{
    if(m_bAllDeviceEnum){
        GetEnumUsbComplete();
        TestSendPower();
    }
    else{
        m_pDeviceOperator->CompleteEnumTest(m_usSequenceNumber);

//        ui->pb_TestEnum_1->setEnabled(true);
//        ui->pb_TestEnum_2->setEnabled(true);
//        ui->pb_TestWholeDUT->setEnabled(true);
    }
}

void FirewareDebug::slot_SendPowerTestComplete()
{
    if(m_bAllDeviceEnum){
        ExitUsbEnumAndSendTest();
        GetSendPowerTestCompleteData();
        m_bAllDeviceEnum = false;

        ui->pb_TestEnum_1->setEnabled(true);
        ui->pb_TestEnum_2->setEnabled(true);
        ui->pb_TestWholeDUT->setEnabled(true);
    }
    else{
        m_pDeviceOperator->ExitSendPowerTest();
        m_pDeviceOperator->PCACK_StartOneGroupPowerTest(m_usSequenceNumber);
    }
}

void FirewareDebug::slot_ReceiveCmd(ushort us_SequenceNumber,
                                    uchar uc_Command)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetReceiveCmdData(uc_Command);
}

void FirewareDebug::slot_WriteCmd(ushort us_SequenceNumber,
                                  uchar uc_Command)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetWriteCmdData(uc_Command);
}

void FirewareDebug::slot_MachineState(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetMachineStateData();
}

void FirewareDebug::slot_PowerSelfTest(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetPowerSelfTestData();
}

void FirewareDebug::slot_StartTestNotice(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    InitUsbEnumAndSendTest();
    GetStartTestNoticeData();
}

void FirewareDebug::slot_TestTimeOut(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetTestTimeOutData();
}

void FirewareDebug::slot_StartOneGroupPowerTest(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    QList<int> list_OneGroupPowerTestMaskCode;
    GetStartOneGroupPowerTestData(list_OneGroupPowerTestMaskCode);
    StartUsbPowerTestByOneGroup(list_OneGroupPowerTestMaskCode);
}

void FirewareDebug::slot_StartOneGroupEnumTest(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    QList<int> list_OneGroupEnumTestMaskCode;
    GetStartOneGroupEnumTestData(list_OneGroupEnumTestMaskCode);
    StartUsbEnumTestByOneGroup(list_OneGroupEnumTestMaskCode);
}

void FirewareDebug::slot_UploadRFPowerResult(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetUploadRFPowerResultData();
}

void FirewareDebug::slot_CompleteTest(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

//    GetEnumUsbComplete();
    GetSendPowerTestCompleteData();
    GetCompleteTestData();

//    WorkSleep(50);
    ExitUsbEnumAndSendTest();

    ui->pb_TestEnum_1->setEnabled(true);
    ui->pb_TestEnum_2->setEnabled(true);
    ui->pb_TestWholeDUT->setEnabled(true);
}
