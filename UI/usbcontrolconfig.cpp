#include "usbcontrolconfig.h"
#include "ui_usbcontrolconfig.h"
#include "DataFile/configfile.h"
#include <QDebug>

UsbControlConfig::UsbControlConfig(ushort us_SequenceNumber, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsbControlConfig)
{
    ui->setupUi(this);
    m_pEnumPointTable = NULL;
    m_usSequenceNumber = us_SequenceNumber;

    this->setAttribute(Qt::WA_DeleteOnClose);

    InitUsbControlConfig();
}

UsbControlConfig::~UsbControlConfig()
{
    if(m_pEnumPointTable != NULL){
        delete m_pEnumPointTable;
        m_pEnumPointTable = NULL;
    }
    delete ui;
}

void UsbControlConfig::SetSequenceNumber(const ushort &us_SequenceNumber)
{
    m_usSequenceNumber = us_SequenceNumber;
}

void UsbControlConfig::SaveCofig()
{
    SaveData();
}

void UsbControlConfig::InitUsbControlConfig()
{
    m_pEnumPointTable = new EnumPointTable(ui->gb_EnumPositionNumber);
    m_pEnumPointTable->move(0,20);
    m_pEnumPointTable->SetRepeatData(true);

    QString RegExp_PID = "^(\\w{4})$";
    QRegExp o_QRegExp_PID(RegExp_PID);
    QValidator *p_Validator_PID = new QRegExpValidator(o_QRegExp_PID, this);

    ui->le_PID->setValidator(p_Validator_PID);
    ui->le_VID->setValidator(p_Validator_PID);

    QString RegExp_Time = "^(\\d{6})$";
    QRegExp o_QRegExp_Time(RegExp_Time);
    QValidator *p_Validator_Time = new QRegExpValidator(o_QRegExp_Time, this);

    ui->le_EnumTime->setValidator(p_Validator_Time);

    ShowData();

    connect(ui->pb_Save, &QPushButton::clicked,
            this, &UsbControlConfig::SaveCofig);
}

void UsbControlConfig::ShowData()
{
    STRUCT_USBCONTROLCONFIG struct_UsbControlConfig;
    QString str_EnumPositionPort;
    QList<QString> list_EnumPositionPort;

    ConfigFile o_ConfigFile;
    o_ConfigFile.GetUsbControlConfig(m_usSequenceNumber,
                                     struct_UsbControlConfig);

    ui->le_PID->setText(QString::number(struct_UsbControlConfig.un_Pid, 16));
    ui->le_VID->setText(QString::number(struct_UsbControlConfig.un_Vid, 16));
    ui->le_EnumTime->setText(QString::number(struct_UsbControlConfig.n_Time));

    o_ConfigFile.TransformToQString(struct_UsbControlConfig.map_StationPort,
                                    str_EnumPositionPort);
    o_ConfigFile.TransformToList(str_EnumPositionPort,
                                 list_EnumPositionPort);

//    qDebug()<<list_EnumPositionPort;
    m_pEnumPointTable->SetLEValue(list_EnumPositionPort);
}

void UsbControlConfig::SaveData()
{
    STRUCT_USBCONTROLCONFIG struct_UsbControlConfig;
    QList<QString> list_EnumPositionPort;
    QString str_EnumPositionPort;

    ConfigFile o_ConfigFile;
    m_pEnumPointTable->GetLEValue(list_EnumPositionPort);
    o_ConfigFile.TransformToQString(list_EnumPositionPort,
                                    str_EnumPositionPort);

    o_ConfigFile.TransformToMap(str_EnumPositionPort,
                                struct_UsbControlConfig.map_StationPort);

    struct_UsbControlConfig.un_Pid = ui->le_PID->text().toUInt(NULL,16);
    struct_UsbControlConfig.un_Vid = ui->le_VID->text().toUInt(NULL,16);
    struct_UsbControlConfig.n_Time = ui->le_EnumTime->text().toInt();

    o_ConfigFile.SaveUsbControlConfig(m_usSequenceNumber,
                                      struct_UsbControlConfig);
}
