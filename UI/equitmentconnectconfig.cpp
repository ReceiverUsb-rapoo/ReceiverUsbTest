#include "equitmentconnectconfig.h"
#include "ui_equitmentconnectconfig.h"
#include "DataFile/configfile.h"

EquitmentConnectConfig::EquitmentConnectConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EquitmentConnectConfig)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    InitEquitmentConfig();
    ShowConfig();
}

EquitmentConnectConfig::~EquitmentConnectConfig()
{
    delete ui;
}

void EquitmentConnectConfig::InitEquitmentConfig()
{
    QString RegExp_IP = "^((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]\\d))|([0-9]))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]\\d)|([0-9])))$";
    QRegExp o_QRegExp_ip(RegExp_IP);
    QValidator *p_Validator_ip = new QRegExpValidator(o_QRegExp_ip, this);

    ui->le_BoxIP_1->setValidator(p_Validator_ip);
    ui->le_BoxIP_2->setValidator(p_Validator_ip);
    ui->le_RobotIP->setValidator(p_Validator_ip);

    QString RegExp_Com = "^((ttyS\\d)|(ttyS[1-9]\\d)|(ttyUSB\\d)|(ttyUSB[1-9]\\d))$";
    QRegExp o_QRegExp_Com(RegExp_Com);
    QValidator *p_Validator_Com = new QRegExpValidator(o_QRegExp_Com, this);

    ui->le_ComName_1->setValidator(p_Validator_Com);
    ui->le_ComName_2->setValidator(p_Validator_Com);

    QString RegExp_PID = "^(\\w{4})$";
    QRegExp o_QRegExp_PID(RegExp_PID);
    QValidator *p_Validator_PID = new QRegExpValidator(o_QRegExp_PID, this);

    ui->le_ComPID->setValidator(p_Validator_PID);
    ui->le_ComVID->setValidator(p_Validator_PID);

    connect(ui->pb_SaveConfig, &QPushButton::clicked,
            this, &EquitmentConnectConfig::SaveConfig);
}

void EquitmentConnectConfig::ShowConfig()
{
    STRUCT_EQUITMENTCONFIG struct_EquitmentConfig;

    ConfigFile o_ConfigFile;
    if(!o_ConfigFile.GetEquitmentConfig(struct_EquitmentConfig)){
        return;
    }

    ui->le_BoxIP_1->setText(struct_EquitmentConfig.list_BoxIP.at(0));
    ui->le_BoxIP_2->setText(struct_EquitmentConfig.list_BoxIP.at(1));

    ui->le_RobotIP->setText(struct_EquitmentConfig.list_RobotIP.at(0));

    ui->le_ComName_1->setText(struct_EquitmentConfig.list_ConName.at(0));
    ui->le_ComName_2->setText(struct_EquitmentConfig.list_ConName.at(1));

    ui->le_ComPID->setText(QString::number(struct_EquitmentConfig.un_ComPid, 16).toUpper());
    ui->le_ComVID->setText(QString::number(struct_EquitmentConfig.un_ComVid, 16).toUpper());

    if(struct_EquitmentConfig.n_OpemWithComName == 1){
        ui->cb_Com->setCheckState(Qt::Checked);
    }
    else{
        ui->cb_Com->setCheckState(Qt::Unchecked);
    }

    if(struct_EquitmentConfig.n_OpemWithPidVid == 1){
        ui->cb_PIDVID->setCheckState(Qt::Checked);
    }
    else{
        ui->cb_PIDVID->setCheckState(Qt::Unchecked);
    }

    if(struct_EquitmentConfig.n_BoxUnuse == 1){
        ui->cb_BoxUnuse->setCheckState(Qt::Checked);
    }
    else{
        ui->cb_BoxUnuse->setCheckState(Qt::Unchecked);
    }

    if(struct_EquitmentConfig.n_RobotUnuse == 1){
        ui->cb_RobotUnuse->setCheckState(Qt::Checked);
    }
    else{
        ui->cb_RobotUnuse->setCheckState(Qt::Unchecked);
    }
}

void EquitmentConnectConfig::SaveConfig()
{
    STRUCT_EQUITMENTCONFIG struct_EquitmentConfig;
    QList<QString> list_ConName;
    QList<QString> list_BoxIP;
    QList<QString> list_RobotIP;

    list_ConName.append(ui->le_ComName_1->text());
    list_ConName.append(ui->le_ComName_2->text());

    list_BoxIP.append(ui->le_BoxIP_1->text());
    list_BoxIP.append(ui->le_BoxIP_2->text());

    list_RobotIP.append(ui->le_RobotIP->text());

    struct_EquitmentConfig.list_ConName = list_ConName;
    struct_EquitmentConfig.list_BoxIP = list_BoxIP;
    struct_EquitmentConfig.list_RobotIP = list_RobotIP;

    struct_EquitmentConfig.un_ComPid = ui->le_ComPID->text().toUInt(NULL,16);
    struct_EquitmentConfig.un_ComVid = ui->le_ComVID->text().toUInt(NULL,16);

    if(ui->cb_Com->checkState() == Qt::Checked){
        struct_EquitmentConfig.n_OpemWithComName = 1;
    }
    else{
        struct_EquitmentConfig.n_OpemWithComName = 0;
    }

    if(ui->cb_PIDVID->checkState() == Qt::Checked){
        struct_EquitmentConfig.n_OpemWithPidVid = 1;
    }
    else{
        struct_EquitmentConfig.n_OpemWithPidVid = 0;
    }

    if(ui->cb_BoxUnuse->checkState() == Qt::Checked){
        struct_EquitmentConfig.n_BoxUnuse = 1;
    }
    else{
        struct_EquitmentConfig.n_BoxUnuse = 0;
    }

    if(ui->cb_RobotUnuse->checkState() == Qt::Checked){
        struct_EquitmentConfig.n_RobotUnuse = 1;
    }
    else{
        struct_EquitmentConfig.n_RobotUnuse = 0;
    }

    ConfigFile o_ConfigFile;
    o_ConfigFile.SaveEquitmenConfig(struct_EquitmentConfig);
}
