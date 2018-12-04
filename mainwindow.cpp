#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"DataFile/testdatafile.h"

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
    m_SelectSequenceNumber = 1;

    InitMainWindow();

    QTimer::singleShot(1000*60*60, this, SLOT(slot_SaveStatisticalTableLog()));
}

MainWindow::~MainWindow()
{
    SaveTestRecordData();

    if(m_pQButtonGroup != NULL){
        delete m_pQButtonGroup;
        m_pQButtonGroup = NULL;
    }

//    if(m_pResultView != NULL){
//        delete m_pResultView;
//        m_pResultView = NULL;
//    }

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


    InitMasterControl();

    ConnectClick();

    InitUI();
}

void MainWindow::InitUI()
{
    m_pResultView = new ResultView();
    m_pResultView->setGeometry(30,160,860,490);
    m_pResultView->setParent(this);
//    m_pResultView->show();

    m_pMasterControl->GetAllFWSequenceNumber(m_listSequenceNumber);

    for(int i = 0; i < m_listSequenceNumber.count(); i++){
        QPushButton *p_QPushButton = new QPushButton;
        p_QPushButton->setText("夹具 - " + QString::number(m_listSequenceNumber.at(i)));
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
}

void MainWindow::InitMasterControl()
{
    m_pMasterControl = new MasterControl;

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
    connect(m_pMasterControl, SIGNAL(sig_StartTest(ushort)),
            this, SLOT(slot_StartTest(ushort)));
    connect(m_pMasterControl, SIGNAL(sig_CompleteTest(ushort)),
            this, SLOT(slot_CompleteTest(ushort)));
}

void MainWindow::ConnectClick()
{
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

    connect(ui->pb_ShowSwitch, &QPushButton::clicked,
            this, &MainWindow::ShowRecordDataSwitch);
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
    CountPowerLimit *p_CountPowerLimit = new CountPowerLimit(m_SelectSequenceNumber);
    p_CountPowerLimit->SetSequenceNumber(m_SelectSequenceNumber);
    p_CountPowerLimit->show();
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
    m_pMasterControl->InitTest();
}

void MainWindow::StartOneTest()
{
    m_pMasterControl->StartOneTest();
}

void MainWindow::StartAutoTest()
{
    m_pMasterControl->StartAutoTest();
}

void MainWindow::StopTest()
{
    m_pMasterControl->StopTest();
    SaveTestRecordData();
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
    int n_TestTotal = 0;
    int n_RetestTotal = 0;
    int n_PassTotal = 0;
    int n_FaileTotal = 0;
    int n_TestNumber = 0;

    int n_AllTestTotal = 0;
    int n_AllRetestTotal = 0;
    int n_AllPassTotal = 0;
    int n_AllFaileTotal = 0;
    int n_AllTestNumber = 0;

    m_pCountTestData->GetTestTotal(n_TestTotal,
                                   n_RetestTotal,
                                   n_PassTotal,
                                   n_FaileTotal,
                                   n_TestNumber);
    TestDataFile o_TestDataFile;
    o_TestDataFile.GetTestRecordData(n_AllTestTotal,
                                     n_AllRetestTotal,
                                     n_AllPassTotal,
                                     n_AllFaileTotal,
                                     n_AllTestNumber);

    n_AllTestTotal += n_TestTotal;
    n_AllRetestTotal += n_RetestTotal;
    n_AllPassTotal += n_PassTotal;
    n_AllFaileTotal += n_FaileTotal;
    n_AllTestNumber += n_TestNumber;

    if(ShowRecordData == CURRENT_SHOW){
        ui->le_TestTotal->setText(QString::number(n_TestTotal));
        ui->le_RetestTotal->setText(QString::number(n_RetestTotal));
        ui->le_PassTotal->setText(QString::number(n_PassTotal));
        ui->le_FaileTotal->setText(QString::number(n_FaileTotal));
        ui->le_TestNumber->setText(QString::number(n_TestNumber));
    }
    else if(ShowRecordData == TOTAL_SHOW){
        ui->le_TestTotal->setText(QString::number(n_AllTestTotal));
        ui->le_RetestTotal->setText(QString::number(n_AllRetestTotal));
        ui->le_PassTotal->setText(QString::number(n_AllPassTotal));
        ui->le_FaileTotal->setText(QString::number(n_AllFaileTotal));
        ui->le_TestNumber->setText(QString::number(n_AllTestNumber));
    }
}

void MainWindow::SaveTestRecordData()
{
    int n_TestTotal = 0;
    int n_RetestTotal = 0;
    int n_PassTotal = 0;
    int n_FaileTotal = 0;
    int n_TestNumber = 0;

    int n_AllTestTotal = 0;
    int n_AllRetestTotal = 0;
    int n_AllPassTotal = 0;
    int n_AllFaileTotal = 0;
    int n_AllTestNumber = 0;

    m_pCountTestData->GetTestTotal(n_TestTotal,
                                   n_RetestTotal,
                                   n_PassTotal,
                                   n_FaileTotal,
                                   n_TestNumber);
    TestDataFile o_TestDataFile;
    o_TestDataFile.GetTestRecordData(n_AllTestTotal,
                                     n_AllRetestTotal,
                                     n_AllPassTotal,
                                     n_AllFaileTotal,
                                     n_AllTestNumber);

    n_AllTestTotal += n_TestTotal;
    n_AllRetestTotal += n_RetestTotal;
    n_AllPassTotal += n_PassTotal;
    n_AllFaileTotal += n_FaileTotal;
    n_AllTestNumber += n_TestNumber;

    o_TestDataFile.SaveTestRecordData(n_AllTestTotal,
                                      n_AllRetestTotal,
                                      n_AllPassTotal,
                                      n_AllFaileTotal,
                                      n_AllTestNumber);

    m_pCountTestData->ClearTestTotal();
}

void MainWindow::UpdataFWSequenceNumber()
{
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
        ui->gl_Button->addWidget(p_QPushButton,0,i,1,1);
        m_pQButtonGroup->addButton(p_QPushButton, m_listSequenceNumber.at(i));
        m_pResultView->CreatResultView(m_listSequenceNumber.at(i));
    }

    if(!m_listSequenceNumber.isEmpty()){
        m_pResultView->ShowResultView(m_listSequenceNumber.first());
    }
}

void MainWindow::UpdataBoxSequenceNumber()
{
    QList<ushort> list_SequenceNumber;
    m_pMasterControl->GetAllBoxSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.isEmpty()){
        return;
    }

    if(list_SequenceNumber.contains(1))
        ui->lb_BoxState_1->setText("屏蔽箱-1 连接");
    else
        ui->lb_BoxState_1->setText("屏蔽箱-1 未连接");


    if(list_SequenceNumber.contains(2))
        ui->lb_BoxState_2->setText("屏蔽箱-2 连接");
    else
        ui->lb_BoxState_2->setText("屏蔽箱-2 未连接");
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
    ui->lb_CatchRobotState->setText("抓取机器手 连接");
}

void MainWindow::slot_CRobotRemove()
{
    ui->lb_CatchRobotState->setText("抓取机器手 未连接");
}

void MainWindow::slot_SRobotDiscoverd()
{
    ui->lb_SupplementRobotState->setText("补料机器手 连接");
}

void MainWindow::slot_SRobotRemove()
{
    ui->lb_SupplementRobotState->setText("补料机器手 未连接");
}

void MainWindow::slot_StartTest(ushort us_SequenceNumber)
{
    m_pResultView->SetResultViewShow(us_SequenceNumber,
                                     Run_Test);
}

void MainWindow::slot_CompleteTest(ushort us_SequenceNumber)
{
    QList<bool> list_SingleResult;
    QList<QString> list_ResultString;
    m_pCountTestData->GetSingleResultErrorString(us_SequenceNumber,
                                                 list_SingleResult,
                                                 list_ResultString);

    m_pResultView->UpdataResultView(us_SequenceNumber,
                                    list_SingleResult,
                                    list_ResultString);

    UpdataTestRecordData(m_ShowRecordData);
}

void MainWindow::slot_ButtonClick(int n_ID)
{
    qDebug()<<"n_ID"<<n_ID;

    m_SelectSequenceNumber = (ushort)n_ID;
    m_pResultView->ShowResultView((ushort)n_ID);
}

void MainWindow::slot_SaveStatisticalTableLog()
{
    if(m_listSequenceNumber.isEmpty()){
        return;
    }

    for(int i = 0; i < m_listSequenceNumber.count(); i++){
        StatisticalTable o_StatisticalTable(m_listSequenceNumber.at(0));
        o_StatisticalTable.SetSequenceNumber(m_listSequenceNumber.at(0));
        o_StatisticalTable.SaveTableLog();
    }
}
