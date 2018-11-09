#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    m_pMasterControl = NULL;

    InitMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMainWindow()
{
    m_pMasterControl = new MasterControl;
    InitUI();
}

void MainWindow::InitUI()
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
}

void MainWindow::USBConfig()
{
    UsbControlConfig *p_UsbControlConfig = new UsbControlConfig(1);
    p_UsbControlConfig->SetSequenceNumber(1);
    p_UsbControlConfig->show();

}

void MainWindow::FWConfigUI()
{
    FirmwareConfig *p_FirmwareConfig = new FirmwareConfig(1);
    p_FirmwareConfig->SetSequenceNumber(1);
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
    CountPowerLimit *p_CountPowerLimit = new CountPowerLimit(1);
    p_CountPowerLimit->SetSequenceNumber(1);
    p_CountPowerLimit->show();
}

void MainWindow::FWDebugUI()
{
    FirewareDebug *p_FirewareDebug = new FirewareDebug(1);
    p_FirewareDebug->SetSequenceNumber(1);
    p_FirewareDebug->show();
}

void MainWindow::EMControlUI()
{
    EquitmentControl *p_EquitmentControl = new EquitmentControl;
    p_EquitmentControl->show();
}

void MainWindow::PWDebugUI()
{
    PowerDebug *p_PowerDebug = new PowerDebug(1);
    p_PowerDebug->SetSequenceNumber(1);
    p_PowerDebug->show();
}

void MainWindow::DataTableUI()
{
    StatisticalTable *p_StatisticalTable = new StatisticalTable(1);
    p_StatisticalTable->SetSequenceNumber(1);
    p_StatisticalTable->show();
}

void MainWindow::UpdataSequenceNumber()
{
    m_pMasterControl->GetAllSequenceNumber(m_listSequenceNumber);
}

void MainWindow::slot_FWDiscoverd()
{

}

void MainWindow::slot_FWRemove()
{

}
