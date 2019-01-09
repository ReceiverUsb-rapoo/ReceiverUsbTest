#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QApplication>

#include "DataFile/testdatafile.h"
#include "DataFile/logfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowTitle("Receiver-Test");

    m_pMasterControl = NULL;
    m_pResultView = NULL;
    m_pQButtonGroup = NULL;
    m_pQTimer= NULL;
    m_pQThread = NULL;

    m_bInit = false;

    m_ShowRecordData = TOTAL_SHOW;

//    m_SelectSequenceNumber = 1;

    InitMainWindow();

    m_pQTimer = new QTimer;
    connect(m_pQTimer,SIGNAL(timeout()),
            this, SLOT(slot_SaveStatisticalTableLog()));
    m_pQTimer->start(1000*60*30);
}

MainWindow::~MainWindow()
{
    SaveTestRecordData();

    qDebug()<<"~MainWindow";

    if(m_pQButtonGroup != NULL){
        delete m_pQButtonGroup;
        m_pQButtonGroup = NULL;
    }

//    if(m_pResultView != NULL){
//        delete m_pResultView;
//        m_pResultView = NULL;
//    }

    if(m_pQTimer != NULL){
        m_pQTimer->stop();
        delete m_pQTimer;
        m_pQTimer = NULL;
    }

    if(m_pMasterControl != NULL){
        delete m_pMasterControl;
        m_pMasterControl = NULL;
    }

    delete ui;
}

void MainWindow::InitMainWindow()
{
    m_pQButtonGroup = new QButtonGroup;
    m_pCountTestData = m_oCountTestResultInstanceGetter.GetInstance();

    connect(m_pQButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_ButtonClick(int)));

    TestDataFile o_TestDataFile;
    o_TestDataFile.PooledData();

    InitMasterControl();

    ConnectClick();

    InitUI();
}

void MainWindow::InitUI()
{
    m_pResultView = new ResultView();
    m_pResultView->setGeometry(65,160,800,490);
    m_pResultView->setParent(this);

    m_pMasterControl->GetAllFWSequenceNumber(m_listSequenceNumber);

    for(int i = 0; i < m_listSequenceNumber.count(); i++){
        QPushButton *p_QPushButton = new QPushButton;
        p_QPushButton->setText("夹具 - " + QString::number(m_listSequenceNumber.at(i)));
        p_QPushButton->setStyleSheet("QPushButton{background-color:rgb(211, 215, 207); color:black; border-radius:8px; border-style:inset; min-height:40px;}");
        ui->gl_Button->addWidget(p_QPushButton,0,i,1,1);
        m_pQButtonGroup->addButton(p_QPushButton, m_listSequenceNumber.at(i));
        m_pResultView->CreatResultView(m_listSequenceNumber.at(i));
    }

    if(!m_listSequenceNumber.isEmpty()){
        m_pResultView->ShowResultView(m_listSequenceNumber.first());
    }

    ui->lb_BoxState_1->setText("屏蔽箱-1 未连接");
    ui->lb_BoxState_2->setText("屏蔽箱-2 未连接");
    ui->lb_SupplementRobotState->setText("补料机器手 未连接");
    ui->lb_CatchRobotState->setText("抓取机器手 未连接");

    ui->pb_InitTest->setEnabled(true);
    ui->pb_StartAutoTest->setEnabled(false);
    ui->pb_StartOneTest->setEnabled(false);
    ui->pb_StopTest->setEnabled(false);
    ui->pb_ResettingTest->setEnabled(true);

    ShowRecordDataSwitch();
}

void MainWindow::InitMasterControl()
{
    m_pMasterControl = new MasterControl;

    m_pQThread = new QThread(this);
    m_pMasterControl->moveToThread(m_pQThread);
    m_pQThread->start();

    connect(m_pMasterControl, SIGNAL(sig_FWDiscoverd()),
            this, SLOT(slot_FWDiscoverd()));
    connect(m_pMasterControl, SIGNAL(sig_FWRemove()),
            this, SLOT(slot_FWRemove()));
    connect(m_pMasterControl, SIGNAL(sig_BoxDiscoverd()),
            this, SLOT(slot_BoxDiscoverd()));
    connect(m_pMasterControl, SIGNAL(sig_BoxRemove()),
            this, SLOT(slot_BoxRemove()));
    connect(m_pMasterControl, SIGNAL(sig_CRobotDiscoverd()),
            this, SLOT(slot_CRobotDiscoverd()));
    connect(m_pMasterControl, SIGNAL(sig_CRobotRemove()),
            this, SLOT(slot_CRobotRemove()));
    connect(m_pMasterControl, SIGNAL(sig_SRobotDiscoverd()),
            this, SLOT(slot_SRobotDiscoverd()));
    connect(m_pMasterControl, SIGNAL(sig_SRobotRemove()),
            this, SLOT(slot_SRobotRemove()));
    connect(m_pMasterControl, SIGNAL(sig_ReadyTest(ushort)),
            this, SLOT(slot_ReadyTest(ushort)));
    connect(m_pMasterControl, SIGNAL(sig_StartTest(ushort)),
            this, SLOT(slot_StartTest(ushort)));
    connect(m_pMasterControl, SIGNAL(sig_CompleteTest(ushort)),
            this, SLOT(slot_CompleteTest(ushort)));
}

void MainWindow::ConnectClick()
{
    connect(ui->a_Exit, &QAction::triggered,
            this, &MainWindow::ExitProgram);
    connect(ui->a_USBConfig, &QAction::triggered,
            this, &MainWindow::USBConfig);
    connect(ui->a_FWConfig, &QAction::triggered,
            this, &MainWindow::FWConfigUI);
    connect(ui->a_EMConfig, &QAction::triggered,
            this, &MainWindow::EMConfigUI);
    connect(ui->a_PowerLimit, &QAction::triggered,
            this, &MainWindow::CPLimitUI);
    connect(ui->a_FWDebug, &QAction::triggered,
            this, &MainWindow::FWDebugUI);
    connect(ui->a_EMDebug, &QAction::triggered,
            this, &MainWindow::EMControlUI);
    connect(ui->a_PowerDebug, &QAction::triggered,
            this, &MainWindow::PWDebugUI);
    connect(ui->a_DataTable, &QAction::triggered,
            this, &MainWindow::DataTableUI);

    connect(ui->pb_InitTest,&QPushButton::clicked,
            this, &MainWindow::InitTest);
    connect(ui->pb_StartOneTest, &QPushButton::clicked,
            this, &MainWindow::StartOneTest);
    connect(ui->pb_StartAutoTest, &QPushButton::clicked,
            this, &MainWindow::StartAutoTest);
    connect(ui->pb_StopTest, &QPushButton::clicked,
            this, &MainWindow::StopTest);
    connect(ui->pb_ResettingTest, &QPushButton::clicked,
            this, &MainWindow::Resetting);

    connect(ui->pb_ShowSwitch, &QPushButton::clicked,
            this, &MainWindow::ShowRecordDataSwitch);
}

void MainWindow::ExitProgram()
{
    QApplication *p_QApplication;
    p_QApplication->quit();
}

void MainWindow::USBConfig()
{
    UsbControlConfig *p_UsbControlConfig = new UsbControlConfig(m_SelectSequenceNumber);
    p_UsbControlConfig->SetSequenceNumber(m_SelectSequenceNumber);
    p_UsbControlConfig->show();
}

void MainWindow::FWConfigUI()
{
    FirmwareConfig *p_FirmwareConfig = new FirmwareConfig(m_SelectSequenceNumber);
    p_FirmwareConfig->SetSequenceNumber(m_SelectSequenceNumber);
    p_FirmwareConfig->exec();
    p_FirmwareConfig->deleteLater();
    p_FirmwareConfig = NULL;
}

void MainWindow::EMConfigUI()
{
    EquitmentConnectConfig *p_EquitmentConnectConfig = new EquitmentConnectConfig;
    p_EquitmentConnectConfig->show();
}

void MainWindow::CPLimitUI()
{
    if(!m_bInit){
        QMessageBox::information(NULL, "提示", "  请初始化测试       ", QMessageBox::Yes, QMessageBox::Yes);
    }
    else{
        CountPowerLimit *p_CountPowerLimit = new CountPowerLimit(m_SelectSequenceNumber);
        p_CountPowerLimit->SetSequenceNumber(m_SelectSequenceNumber);
        p_CountPowerLimit->show();
    }
}

void MainWindow::FWDebugUI()
{
    FirewareDebug *p_FirewareDebug = new FirewareDebug(m_SelectSequenceNumber);
    p_FirewareDebug->SetSequenceNumber(m_SelectSequenceNumber);
    p_FirewareDebug->show();
}

void MainWindow::EMControlUI()
{
    EquitmentControl *p_EquitmentControl = new EquitmentControl;
    p_EquitmentControl->show();
}

void MainWindow::PWDebugUI()
{
    PowerDebug *p_PowerDebug = new PowerDebug(m_SelectSequenceNumber);
    p_PowerDebug->SetSequenceNumber(m_SelectSequenceNumber);
    p_PowerDebug->show();
}

void MainWindow::DataTableUI()
{
    StatisticalTable *p_StatisticalTable = new StatisticalTable(m_SelectSequenceNumber);
    p_StatisticalTable->SetSequenceNumber(m_SelectSequenceNumber);
    p_StatisticalTable->show();
}

void MainWindow::InitTest()
{
    qDebug()<<"InitTest";
    m_pMasterControl->InitTest();
    ui->pb_InitTest->setEnabled(false);
    ui->pb_StartAutoTest->setEnabled(true);
    ui->pb_StartOneTest->setEnabled(true);
    ui->pb_StopTest->setEnabled(true);
    ui->pb_ResettingTest->setEnabled(false);

    ShowImportantMenu(false);

    m_bInit = true;
    LogFile::Addlog("初始化测试");
}

void MainWindow::StartOneTest()
{
    m_pMasterControl->StartOneTest();
    ui->pb_InitTest->setEnabled(false);
    ui->pb_StartAutoTest->setEnabled(false);
    ui->pb_StartOneTest->setEnabled(true);
    ui->pb_StopTest->setEnabled(true);
    ui->pb_ResettingTest->setEnabled(false);
    SaveTestRecordData();
    UpdataTestRecordData(m_ShowRecordData);
    LogFile::Addlog("开始一次测试");
}

void MainWindow::StartAutoTest()
{
    m_pMasterControl->StartAutoTest();
    ui->pb_InitTest->setEnabled(false);
    ui->pb_StartAutoTest->setEnabled(false);
    ui->pb_StartOneTest->setEnabled(false);
    ui->pb_StopTest->setEnabled(true);
    ui->pb_ResettingTest->setEnabled(false);

    LogFile::Addlog("开始自动测试");
}

void MainWindow::StopTest()
{
    qDebug()<<"StopTest";
    m_pMasterControl->StopTest();
    ui->pb_InitTest->setEnabled(false);
    ui->pb_StartAutoTest->setEnabled(false);
    ui->pb_StartOneTest->setEnabled(false);
    ui->pb_StopTest->setEnabled(true);
    ui->pb_ResettingTest->setEnabled(true);
    SaveTestRecordData();
    UpdataTestRecordData(m_ShowRecordData);

    m_bInit = false;

    LogFile::Addlog("停止测试");
}

void MainWindow::Resetting()
{
    m_pMasterControl->Resetting();
    ui->pb_InitTest->setEnabled(true);
    ui->pb_StartAutoTest->setEnabled(false);
    ui->pb_StartOneTest->setEnabled(false);
    ui->pb_StopTest->setEnabled(false);
    ui->pb_ResettingTest->setEnabled(true);

    ShowImportantMenu(true);
    LogFile::Addlog("重置测试");
}

void MainWindow::ShowTestState(const ENUM_TESTSTATE &TestState)
{
    switch(TestState) {
    case Ready_Test:{
        ui->lb_TestState->setStyleSheet("background:rgb(235, 255, 233)");
        break;
    }
    case Run_Test:{
        ui->lb_TestState->setStyleSheet("background:rgb(252, 233, 79)");
        break;
    }
    case Faile_Test:{
        ui->lb_TestState->setStyleSheet("background:rgb(204, 0, 0)");
        break;
    }
    case Pass_Test:{
        ui->lb_TestState->setStyleSheet("background:rgb(115, 210, 22)");
        break;
    }
    default:
        break;
    }
}

void MainWindow::ShowConnectState(QLabel *&p_QLabel,
                                  const EMUN_CONNECTSTATE &ConnectState)
{
    if(ConnectState == CONNECTTOPC){
        p_QLabel->setStyleSheet("color:rgb(78, 154, 6)");
        QFont o_QFont;
        o_QFont.setPointSize(11);
        o_QFont.setBold(true);
        p_QLabel->setFont(o_QFont);
    }
    else if(ConnectState == DISCONNECTTOPC){
        p_QLabel->setStyleSheet("color:rgb(46, 52, 54)");
        QFont o_QFont;
        o_QFont.setPointSize(9);
        p_QLabel->setFont(o_QFont);
    }
}

void MainWindow::ShowImportantMenu(const bool &b_Show)
{
    ui->a_EMConfig->setEnabled(b_Show);
    ui->a_EMDebug->setEnabled(b_Show);
    ui->a_FWConfig->setEnabled(b_Show);
    ui->a_FWDebug->setEnabled(b_Show);
    ui->a_PowerDebug->setEnabled(b_Show);
    ui->a_USBConfig->setEnabled(b_Show);
}

void MainWindow::ShowRecordDataSwitch()
{
    if(ui->pb_ShowSwitch->text() == "当前数据"){
        ui->pb_ShowSwitch->setText("累计数据");
        m_ShowRecordData = TOTAL_SHOW;
        UpdataTestRecordData(m_ShowRecordData);
    }
    else{
        ui->pb_ShowSwitch->setText("当前数据");
        m_ShowRecordData = CURRENT_SHOW;
        UpdataTestRecordData(m_ShowRecordData);
    }
}

void MainWindow::UpdataTestRecordData(const ENUM_SHOWRECORDDATA &ShowRecordData)
{
    RECORDDATA CurrentData;
    RECORDDATA TotalData;

    m_pCountTestData->GetTestTotal(CurrentData.n_TestTotal,
                                   CurrentData.n_RetestTotal,
                                   CurrentData.n_PassTotal,
                                   CurrentData.n_FaileTotal,
                                   CurrentData.n_TestNumber);

    TestDataFile o_TestDataFile;
    o_TestDataFile.GetTestTotalRecordData(TotalData);

    o_TestDataFile.SaveCurrentTestRecordData(CurrentData);

    TotalData.n_TestTotal += CurrentData.n_TestTotal;
    TotalData.n_RetestTotal += CurrentData.n_RetestTotal;
    TotalData.n_PassTotal += CurrentData.n_PassTotal;
    TotalData.n_FaileTotal += CurrentData.n_FaileTotal;
    TotalData.n_TestNumber += CurrentData.n_TestNumber;

    if(ShowRecordData == CURRENT_SHOW){
        ui->le_TestTotal->setText(QString::number(CurrentData.n_TestTotal));
        ui->le_RetestTotal->setText(QString::number(CurrentData.n_RetestTotal));
        ui->le_PassTotal->setText(QString::number(CurrentData.n_PassTotal));
        ui->le_FaileTotal->setText(QString::number(CurrentData.n_FaileTotal));
        ui->le_TestNumber->setText(QString::number(CurrentData.n_TestNumber));
    }
    else if(ShowRecordData == TOTAL_SHOW){
        ui->le_TestTotal->setText(QString::number(TotalData.n_TestTotal));
        ui->le_RetestTotal->setText(QString::number(TotalData.n_RetestTotal));
        ui->le_PassTotal->setText(QString::number(TotalData.n_PassTotal));
        ui->le_FaileTotal->setText(QString::number(TotalData.n_FaileTotal));
        ui->le_TestNumber->setText(QString::number(TotalData.n_TestNumber));
    }
}

void MainWindow::SaveTestRecordData()
{
    TestDataFile o_TestDataFile;
    o_TestDataFile.PooledData();

    m_pCountTestData->ClearTestTotal();
}

void MainWindow::UpdataFWSequenceNumber()
{
    LogFile::Addlog("Com拔插 FW更新");

    for(int i = 0; i < m_listSequenceNumber.count(); i++){
        QAbstractButton *p_QQAbstractButton = m_pQButtonGroup->button(m_listSequenceNumber.at(i));
        m_pQButtonGroup->removeButton(p_QQAbstractButton);
        ui->gl_Button->removeWidget(p_QQAbstractButton);
        m_pResultView->RemoveResultView(m_listSequenceNumber.at(i));
        delete p_QQAbstractButton;
        p_QQAbstractButton = NULL;
    }

    m_pMasterControl->GetAllFWSequenceNumber(m_listSequenceNumber);

    for(int i = 0; i < m_listSequenceNumber.count(); i++){
        QPushButton *p_QPushButton = new QPushButton;
        p_QPushButton->setText("夹具 - " + QString::number(m_listSequenceNumber.at(i)));
        p_QPushButton->setStyleSheet("QPushButton{background-color:rgb(211, 215, 207); color:black; border-radius:8px; border-style:inset; min-height:40px;}");
//        p_QPushButton->setStyleSheet("QPushButton{background-color:rgb(85, 170, 255);color: black; border-radius: 8px;  border: 2px groove gray; border-style: inset;}"
//                                     "QPushButton:hover{background-color:white; color:rgb(42,48,43);}"
//                                     "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset;}");

        ui->gl_Button->addWidget(p_QPushButton,0,i,1,1);
        m_pQButtonGroup->addButton(p_QPushButton, m_listSequenceNumber.at(i));
        m_pResultView->CreatResultView(m_listSequenceNumber.at(i));
    }

    if(!m_listSequenceNumber.isEmpty()){
        m_pResultView->ShowResultView(m_listSequenceNumber.first());
        m_SelectSequenceNumber = m_listSequenceNumber.first();

        slot_ButtonClick(m_listSequenceNumber.first());
    }
}

void MainWindow::UpdataBoxSequenceNumber()
{
    QList<ushort> list_SequenceNumber;
    m_pMasterControl->GetAllBoxSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        QString str_Info = "屏蔽箱-1 未连接,屏蔽箱-2 未连接";
        LogFile::Addlog(str_Info);

        ui->lb_BoxState_1->setText("屏蔽箱-1 未连接");
        ShowConnectState(ui->lb_BoxState_1, DISCONNECTTOPC);

        ui->lb_BoxState_2->setText("屏蔽箱-2 未连接");
        ShowConnectState(ui->lb_BoxState_2, DISCONNECTTOPC);

        return;
    }

    if(list_SequenceNumber.contains(1)){
        QString str_Info = "屏蔽箱-1 连接";
        ui->lb_BoxState_1->setText(str_Info);
        LogFile::Addlog(str_Info);
        ShowConnectState(ui->lb_BoxState_1, CONNECTTOPC);
    }
    else{
        QString str_Info = "屏蔽箱-1 未连接";
        ui->lb_BoxState_1->setText(str_Info);
        LogFile::Addlog(str_Info);
        ShowConnectState(ui->lb_BoxState_1, DISCONNECTTOPC);
    }

    if(list_SequenceNumber.contains(2)){
        QString str_Info = "屏蔽箱-2 连接";
        ui->lb_BoxState_2->setText(str_Info);
        LogFile::Addlog(str_Info);
        ShowConnectState(ui->lb_BoxState_2, CONNECTTOPC);
    }
    else{
        QString str_Info = "屏蔽箱-2 未连接";
        ui->lb_BoxState_2->setText(str_Info);
        LogFile::Addlog(str_Info);
        ShowConnectState(ui->lb_BoxState_2, DISCONNECTTOPC);
    }
}

void MainWindow::slot_FWDiscoverd()
{
    UpdataFWSequenceNumber();
}

void MainWindow::slot_FWRemove()
{
    UpdataFWSequenceNumber();
}

void MainWindow::slot_BoxDiscoverd()
{
    UpdataBoxSequenceNumber();
}

void MainWindow::slot_BoxRemove()
{
    UpdataBoxSequenceNumber();
}

void MainWindow::slot_CRobotDiscoverd()
{
    QString str_Info = "抓取机器手 连接";
    LogFile::Addlog(str_Info);
    ui->lb_CatchRobotState->setText(str_Info);
    ShowConnectState(ui->lb_CatchRobotState, CONNECTTOPC);
}

void MainWindow::slot_CRobotRemove()
{
    QString str_Info = "抓取机器手 未连接";
    LogFile::Addlog(str_Info);
    ui->lb_CatchRobotState->setText(str_Info);
    ShowConnectState(ui->lb_CatchRobotState, DISCONNECTTOPC);
}

void MainWindow::slot_SRobotDiscoverd()
{
    QString str_Info = "补料机器手 连接";
    LogFile::Addlog(str_Info);
    ui->lb_SupplementRobotState->setText(str_Info);
    ShowConnectState(ui->lb_SupplementRobotState, CONNECTTOPC);
}

void MainWindow::slot_SRobotRemove()
{
    QString str_Info = "补料机器手 未连接";
    LogFile::Addlog(str_Info);
    ui->lb_SupplementRobotState->setText(str_Info);
    ShowConnectState(ui->lb_SupplementRobotState, DISCONNECTTOPC);
}

void MainWindow::slot_ReadyTest(ushort us_SequenceNumber)
{
    m_pResultView->SetResultViewShow(us_SequenceNumber,
                                     Ready_Test);
    ShowTestState(Ready_Test);

    qDebug()<<"slot_ReadyTest"<<us_SequenceNumber;

    LogFile::Addlog("FW-" + QString::number(us_SequenceNumber) + " 准备测试");
}

void MainWindow::slot_StartTest(ushort us_SequenceNumber)
{
    m_pResultView->SetResultViewShow(us_SequenceNumber,
                                     Run_Test);
    ShowTestState(Run_Test);

    qDebug()<<"slot_StartTest"<<us_SequenceNumber;

    LogFile::Addlog("FW-" + QString::number(us_SequenceNumber) + " 通知开始测试");
}

void MainWindow::slot_CompleteTest(ushort us_SequenceNumber)
{
    QList<bool> list_SingleResult;
    QList<QString> list_ResultString;
    bool b_Result;
    m_pCountTestData->GetSingleResultErrorString(us_SequenceNumber,
                                                 list_SingleResult,
                                                 list_ResultString);

    m_pResultView->UpdataResultView(us_SequenceNumber,
                                    list_SingleResult,
                                    list_ResultString);


    m_pCountTestData->GetResult(us_SequenceNumber,
                                b_Result);

    if(b_Result)
        ShowTestState(Pass_Test);
    else
        ShowTestState(Faile_Test);

    UpdataTestRecordData(m_ShowRecordData);

    LogFile::Addlog("FW-" + QString::number(us_SequenceNumber) + " 通知完成测试");
}

void MainWindow::slot_ButtonClick(int n_ID)
{
    qDebug()<<"n_ID"<<n_ID;

    QList<QAbstractButton*> list_Button = m_pQButtonGroup->buttons();
    for(int i = 0; i < list_Button.count(); i++){
        list_Button.at(i)->setStyleSheet("QPushButton{background-color:rgb(211, 215, 207); color:black; border-radius:8px; border-style:inset; min-height:40px;}");
    }

    m_pQButtonGroup->button(n_ID)->setStyleSheet("QPushButton{background-color:rgb(114, 159, 207); color:black; border-radius:8px; border-style:outset; min-height:40px;}");

    m_SelectSequenceNumber = (ushort)n_ID;
    m_pResultView->ShowResultView((ushort)n_ID);
}

void MainWindow::slot_SaveStatisticalTableLog()
{
    if(m_listSequenceNumber.isEmpty()){
        return;
    }

    for(int i = 0; i < m_listSequenceNumber.count(); i++){
        StatisticalTable o_StatisticalTable(m_listSequenceNumber.at(i));
        o_StatisticalTable.SetSequenceNumber(m_listSequenceNumber.at(i));
        o_StatisticalTable.SaveTableLog();
    }
}
