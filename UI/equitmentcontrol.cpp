#include "equitmentcontrol.h"
#include "ui_equitmentcontrol.h"
#include <QRegExp>
#include "DataFile/logfile.h"

EquitmentControl::EquitmentControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EquitmentControl)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("EquitmentControl");

    InitUI();
    InitEquitmentControl();
    InitEquitmentState();
}

EquitmentControl::~EquitmentControl()
{
    delete ui;
}

void EquitmentControl::InitEquitmentControl()
{
    m_pDeviceObserver = m_oDeviceObserverInstanceGetter.GetInstance();
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();

    connect(m_pDeviceObserver, SIGNAL(sig_BoxDiscoverd()),
            this, SLOT(slot_BoxDiscoverd()));
    connect(m_pDeviceObserver, SIGNAL(sig_CatchRobotDiscoverd()),
            this, SLOT(slot_CatchRobotDiscoverd()));
    connect(m_pDeviceObserver, SIGNAL(sig_SupplementRobotDiscoverd()),
            this, SLOT(slot_SupplementRobotDiscoverd()));
    connect(m_pDeviceObserver, SIGNAL(sig_BoxRemove()),
            this, SLOT(slot_BoxRemove()));
    connect(m_pDeviceObserver, SIGNAL(sig_CatchRobotRemove()),
            this, SLOT(slot_CatchRobotRemove()));
    connect(m_pDeviceObserver, SIGNAL(sig_SupplementRobotRemove()),
            this, SLOT(slot_SupplementRobotRemove()));
    connect(m_pDeviceObserver, SIGNAL(sig_BoxOperatorUpdata(ushort)),
            this, SLOT(slot_BoxOperatorUpdata(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_CatchRobotGetActionUpdata(ushort)),
            this, SLOT(slot_CatchRobotGetActionUpdata(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_SupplementRobotGetRequestUpdata(ushort)),
            this, SLOT(slot_SupplementRobotGetRequestUpdata(ushort)));

    connect(ui->pb_Close_1, &QPushButton::clicked,
            [&](){CloseBox(1);});
    connect(ui->pb_Close_2, &QPushButton::clicked,
            [&](){CloseBox(2);});
    connect(ui->pb_Open_1, &QPushButton::clicked,
            [&](){OpenBox(1);});
    connect(ui->pb_Open_2, &QPushButton::clicked,
            [&](){OpenBox(2);});

    connect(ui->pb_Put_1, &QPushButton::clicked,
            [&](){SendCatchRobotAction(1, CatchRobot_Put);});
    connect(ui->pb_Put_2, &QPushButton::clicked,
            [&](){SendCatchRobotAction(2, CatchRobot_Put);});
    connect(ui->pb_Get_1, &QPushButton::clicked,
            [&](){SendCatchRobotAction(1, CatchRobot_Get);});
    connect(ui->pb_Get_2, &QPushButton::clicked,
            [&](){SendCatchRobotAction(2, CatchRobot_Get);});

    connect(ui->pb_SendSupplement_1, &QPushButton::clicked,
            [&](){SendSupplementRobotData(1);});
    connect(ui->pb_SendSupplement_2, &QPushButton::clicked,
            [&](){SendSupplementRobotData(2);});

}

void EquitmentControl::InitEquitmentState()
{
    QList<ushort> list_SequenceNumber;
    m_pDeviceObserver->GetAllBoxSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.contains(1)){
        ui->pb_Open_1->setEnabled(true);
        ui->pb_Close_1->setEnabled(true);
    }

    if(list_SequenceNumber.contains(2)){
        ui->pb_Open_2->setEnabled(true);
        ui->pb_Close_2->setEnabled(true);
    }

    if(m_pDeviceObserver->GetCatchRobotConnectState()){
        ui->pb_Put_1->setEnabled(true);
        ui->pb_Put_2->setEnabled(true);
        ui->pb_Get_1->setEnabled(true);
        ui->pb_Get_2->setEnabled(true);
    }

    if(m_pDeviceObserver->GetSupplementRobotConnectState()){
        ui->pb_SendSupplement_1->setEnabled(true);
        ui->pb_SendSupplement_2->setEnabled(true);
    }

    QRegExp o_QRegExp("^([0-1]{5})$");
    ui->le_SupplementData_1->setValidator(new QRegExpValidator(o_QRegExp, this));
    ui->le_SupplementData_2->setValidator(new QRegExpValidator(o_QRegExp, this));
    ui->le_SupplementData_3->setValidator(new QRegExpValidator(o_QRegExp, this));
    ui->le_SupplementData_4->setValidator(new QRegExpValidator(o_QRegExp, this));
}

void EquitmentControl::InitUI()
{
    ui->pb_Open_1->setEnabled(false);
    ui->pb_Close_1->setEnabled(false);
    ui->pb_Open_2->setEnabled(false);
    ui->pb_Close_2->setEnabled(false);

    ui->pb_Put_1->setEnabled(false);
    ui->pb_Put_2->setEnabled(false);
    ui->pb_Get_1->setEnabled(false);
    ui->pb_Get_2->setEnabled(false);

    ui->pb_SendSupplement_1->setEnabled(false);
    ui->pb_SendSupplement_2->setEnabled(false);
}

void EquitmentControl::OpenBox(const ushort &us_SequenceNumber)
{
    m_pDeviceOperator->OpenBox(us_SequenceNumber);

    ui->tb_Info->append(LogFile::Addlog("Send Box-" +
                                 QString::number(us_SequenceNumber) +
                                 "\n" +
                                 QString(OpenCmd)));
}

void EquitmentControl::CloseBox(const ushort &us_SequenceNumber)
{
    m_pDeviceOperator->CloseBox(us_SequenceNumber);

    ui->tb_Info->append(LogFile::Addlog("Send Box-" +
                                        QString::number(us_SequenceNumber) +
                                        "\n" +
                                        QString(CloseCmd)));
}

void EquitmentControl::SendCatchRobotAction(const ushort &us_FWStation,
                                            const QString &str_RobotAction)
{
    m_pDeviceOperator->SendCatchRobotAction(us_FWStation,
                                            str_RobotAction);

    ui->tb_Info->append(LogFile::Addlog("Send CatchRobot \n" +
                                        str_RobotAction +
                                        QString::number(us_FWStation)));
}

void EquitmentControl::SendSupplementRobotData(const ushort &us_FWStation)
{
    QString str_SupplementData = "";

    str_SupplementData += ui->le_SupplementData_1->text();
    str_SupplementData += ui->le_SupplementData_2->text();
    str_SupplementData += ui->le_SupplementData_3->text();
    str_SupplementData += ui->le_SupplementData_4->text();

    m_pDeviceOperator->SendSupplementRobotData(us_FWStation,
                                               str_SupplementData);

    ui->tb_Info->append(LogFile::Addlog("Send SupplementRobot \n" +
                                        str_SupplementData/* +
                                        QString::number(us_FWStation)*/));
}

void EquitmentControl::slot_BoxDiscoverd()
{
    ui->pb_Open_1->setEnabled(false);
    ui->pb_Close_1->setEnabled(false);
    ui->pb_Open_2->setEnabled(false);
    ui->pb_Close_2->setEnabled(false);

    QList<ushort> list_SequenceNumber;
    m_pDeviceObserver->GetAllBoxSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.contains(1)){
        ui->pb_Open_1->setEnabled(true);
        ui->pb_Close_1->setEnabled(true);
    }

    if(list_SequenceNumber.contains(2)){
        ui->pb_Open_2->setEnabled(true);
        ui->pb_Close_2->setEnabled(true);
    }
}

void EquitmentControl::slot_CatchRobotDiscoverd()
{
    ui->pb_Put_1->setEnabled(true);
    ui->pb_Put_2->setEnabled(true);
    ui->pb_Get_1->setEnabled(true);
    ui->pb_Get_2->setEnabled(true);
}

void EquitmentControl::slot_SupplementRobotDiscoverd()
{
    ui->pb_SendSupplement_1->setEnabled(true);
    ui->pb_SendSupplement_2->setEnabled(true);
}

void EquitmentControl::slot_BoxRemove()
{
    ui->pb_Open_1->setEnabled(false);
    ui->pb_Close_1->setEnabled(false);
    ui->pb_Open_2->setEnabled(false);
    ui->pb_Close_2->setEnabled(false);

    QList<ushort> list_SequenceNumber;
    m_pDeviceObserver->GetAllBoxSequenceNumber(list_SequenceNumber);

    if(list_SequenceNumber.contains(1)){
        ui->pb_Open_1->setEnabled(true);
        ui->pb_Close_1->setEnabled(true);
    }

    if(list_SequenceNumber.contains(2)){
        ui->pb_Open_2->setEnabled(true);
        ui->pb_Close_2->setEnabled(true);
    }
}

void EquitmentControl::slot_CatchRobotRemove()
{
    ui->pb_Put_1->setEnabled(false);
    ui->pb_Put_2->setEnabled(false);
    ui->pb_Get_1->setEnabled(false);
    ui->pb_Get_2->setEnabled(false);
}

void EquitmentControl::slot_SupplementRobotRemove()
{
    ui->pb_SendSupplement_1->setEnabled(false);
    ui->pb_SendSupplement_2->setEnabled(false);
}

void EquitmentControl::slot_BoxOperatorUpdata(ushort us_SequenceNumber)
{
    BOX_OPERATOR box_Operator;
    QString str_Operator;
    m_pDeviceObserver->GetBoxOperator(us_SequenceNumber, box_Operator);

    if(box_Operator == OPENBOX_OK){
        str_Operator = "OPENBOX_OK";
    }
    else if(box_Operator == CLOSEBOX_OK){
        str_Operator = "CLOSEBOX_OK";
    }
    else if(box_Operator == ERRORBOX){
        str_Operator = "ERRORBOX";
    }

    ui->tb_Info->append(LogFile::Addlog("Receiver Box-" +
                                         QString::number(us_SequenceNumber) +
                                         "\n" +
                                         str_Operator));
}

void EquitmentControl::slot_CatchRobotGetActionUpdata(ushort us_SequenceNumber)
{
    QString str_Action;
    m_pDeviceObserver->GetCatchRobotGetAction(us_SequenceNumber, str_Action);

    ui->tb_Info->append(LogFile::Addlog("Receiver CatchRobot \n" +
                                        str_Action +
                                        QString::number(us_SequenceNumber)));

    ui->le_ReceiveCatchData->setText(str_Action + QString::number(us_SequenceNumber));
}

void EquitmentControl::slot_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber)
{
    QString str_Request;

    m_pDeviceObserver->GetSupplementRobotGetRequest(us_SequenceNumber, str_Request);

    ui->tb_Info->append(LogFile::Addlog("Receiver SupplementRobot \n" +
                                        str_Request +
                                        QString::number(us_SequenceNumber)));
}
