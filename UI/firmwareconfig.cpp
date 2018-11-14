#include "firmwareconfig.h"
#include "ui_firmwareconfig.h"
#include "DataFile/configfile.h"

FirmwareConfig::FirmwareConfig(ushort us_SequenceNumber, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirmwareConfig)
{
    ui->setupUi(this);
//    m_pEnumPointTable = NULL;
    m_pPT_FWPositionNumber = NULL;
    m_pPT_PowerTestGroup = NULL;
    m_pPT_PowerTestPositionNumber = NULL;

    m_usSequenceNumber = us_SequenceNumber;

//    this->setAttribute(Qt::WA_DeleteOnClose);

    InitFirmwareConfig();
}

FirmwareConfig::~FirmwareConfig()
{
//    if(m_pEnumPointTable != NULL){
//        delete m_pEnumPointTable;
//        m_pEnumPointTable = NULL;
//    }

    if(m_pPT_FWPositionNumber != NULL){
        delete m_pPT_FWPositionNumber;
        m_pPT_FWPositionNumber = NULL;
    }

    if(m_pPT_PowerTestGroup != NULL){
        delete m_pPT_PowerTestGroup;
        m_pPT_PowerTestGroup = NULL;
    }

    if(m_pPT_PowerTestPositionNumber){
        delete m_pPT_PowerTestPositionNumber;
        m_pPT_PowerTestPositionNumber = NULL;
    }

    delete ui;
}

void FirmwareConfig::SetSequenceNumber(const ushort &us_SequenceNumber)
{
    m_usSequenceNumber = us_SequenceNumber;
}

void FirmwareConfig::SaveCofig()
{
    SaveData();
}

void FirmwareConfig::InitFirmwareConfig()
{
    InitFWUI();

    ShowData();

    connect(ui->pb_Save, &QPushButton::clicked,
            this, &FirmwareConfig::SaveCofig);
}

void FirmwareConfig::InitFWUI()
{
//    m_pEnumPointTable = new EnumPointTable(ui->gb_EnumPositionNumber);
    m_pPT_FWPositionNumber = new PointTable(ui->gb_FWPositionNumber);
    m_pPT_PowerTestGroup = new PointTable(ui->gb_PowerTestGroup);
    m_pPT_PowerTestPositionNumber = new PointTable(ui->gb_PowerTestPositionNumber);

//    m_pEnumPointTable->move(0,20);
    m_pPT_FWPositionNumber->move(0,20);
    m_pPT_PowerTestGroup->move(0,20);
    m_pPT_PowerTestPositionNumber->move(0,20);

//    m_pEnumPointTable->SetRepeatData(false);
    m_pPT_FWPositionNumber->SetRepeatData(false);
    m_pPT_PowerTestPositionNumber->SetRepeatData(false);
    m_pPT_PowerTestGroup->SetRepeatData(true);

}

void FirmwareConfig::ShowData()
{
    STRUCT_PCTESTCONFIG struct_PCTestConfig;
    ConfigFile o_ConfigFile;
    o_ConfigFile.GetPCTestConfig(m_usSequenceNumber,
                                 struct_PCTestConfig);

    ui->le_DischargeTime->setText(QString::number(struct_PCTestConfig.us_DischargeTimeSelect));
    ui->le_EnumTime->setText(QString::number(struct_PCTestConfig.us_EnumTime));
    ui->le_SinglePowerTime->setText(QString::number(struct_PCTestConfig.us_SinglePowerTime));

    ui->le_PoweredDelay->setText(QString::number(struct_PCTestConfig.us_PoweredDelay));
    ui->le_ReadRFDelay->setText(QString::number(struct_PCTestConfig.us_ReadRFDelay));
    ui->le_DUTPowerDelay->setText(QString::number(struct_PCTestConfig.us_SinglePowerDelay));

    ui->le_DUTVoltage->setText(QString::number(struct_PCTestConfig.us_DUTVoltageSelect));
    ui->le_PowerAvgTimes->setText(QString::number(struct_PCTestConfig.uc_PowerAvgTimes));
    ui->le_SingleTestCount->setText(QString::number(struct_PCTestConfig.uc_SingleTestCount));
    ui->le_PowerChannel->setText("0");

    if(struct_PCTestConfig.uc_PowerTestSwitch != 0)
        ui->cb_PowerSwitch->setCheckState(Qt::Checked);
    else
        ui->cb_PowerSwitch->setCheckState(Qt::Unchecked);

    if(struct_PCTestConfig.uc_EnumTestSwitch != 0)
        ui->cb_EnumSwitch->setCheckState(Qt::Checked);
    else
        ui->cb_EnumSwitch->setCheckState(Qt::Unchecked);

    if(struct_PCTestConfig.uc_RetestTimes != 0)
        ui->cb_RetestSwitch->setCheckState(Qt::Checked);
    else
        ui->cb_RetestSwitch->setCheckState(Qt::Unchecked);

    ui->le_FWSequenceNumber->setText(QString::number(struct_PCTestConfig.un_FWPosition));
    ui->le_FWType->setText(QString::number(struct_PCTestConfig.TestType));
    ui->le_ProjectName->setText(struct_PCTestConfig.str_ProjectName);
    ui->le_ProjectVersions->setText(struct_PCTestConfig.str_ProjectVersions);

    QList<QString> list_DUTFWPositions;
    QList<QString> list_PowerDUTPositions;
    QList<QString> list_PowerTestGroups;

    o_ConfigFile.TransformToList(struct_PCTestConfig.str_DUTFWPositions,
                                 list_DUTFWPositions);
    o_ConfigFile.TransformToList(struct_PCTestConfig.str_PowerDUTPositions,
                                 list_PowerDUTPositions);
    o_ConfigFile.TransformToList(struct_PCTestConfig.str_PowerTestGroups,
                                 list_PowerTestGroups);

    m_pPT_FWPositionNumber->SetLEValue(list_DUTFWPositions);
    m_pPT_PowerTestPositionNumber->SetLEValue(list_PowerDUTPositions);
    m_pPT_PowerTestGroup->SetLEValue(list_PowerTestGroups);

//    STRUCT_USBCONTROLCONFIG m_structUsbControlConfig;
//    QString str_EnumPositionPort;
//    QList<QString> list_EnumPositionPort;

//    o_ConfigFile.GetUsbControlConfig(m_usSequenceNumber,
//                                     m_structUsbControlConfig);
//    o_ConfigFile.TransformToQString(m_structUsbControlConfig.map_StationPort,
//                                    str_EnumPositionPort);
//    o_ConfigFile.TransformToList(str_EnumPositionPort,
//                                 list_EnumPositionPort);
//    m_pEnumPointTable->SetLEValue(list_EnumPositionPort);
}

void FirmwareConfig::SaveData()
{
    STRUCT_PCTESTCONFIG struct_PCTestConfig;

    struct_PCTestConfig.us_DischargeTimeSelect = ui->le_DischargeTime->text().toUShort();
    struct_PCTestConfig.us_EnumTime = ui->le_EnumTime->text().toUShort();
    struct_PCTestConfig.us_SinglePowerTime = ui->le_SinglePowerTime->text().toUShort();

    struct_PCTestConfig.us_PoweredDelay = ui->le_PoweredDelay->text().toUShort();
    struct_PCTestConfig.us_ReadRFDelay = ui->le_ReadRFDelay->text().toUShort();
    struct_PCTestConfig.us_SinglePowerDelay = ui->le_DUTPowerDelay->text().toUShort();

    struct_PCTestConfig.us_DUTVoltageSelect = ui->le_DUTVoltage->text().toUShort();
    struct_PCTestConfig.uc_PowerAvgTimes = (uchar)ui->le_PowerAvgTimes->text().toUShort();
    struct_PCTestConfig.uc_SingleTestCount = (uchar)ui->le_SingleTestCount->text().toUShort();
    struct_PCTestConfig.uc_PowerChannel = (uchar)ui->le_PowerChannel->text().toUShort();

    if(ui->cb_PowerSwitch->checkState() == Qt::Checked)
        struct_PCTestConfig.uc_PowerTestSwitch = (uchar)0x01;
    else
        struct_PCTestConfig.uc_PowerTestSwitch = (uchar)0x00;

    if(ui->cb_EnumSwitch->checkState() == Qt::Checked)
        struct_PCTestConfig.uc_EnumTestSwitch = (uchar)0x01;
    else
        struct_PCTestConfig.uc_EnumTestSwitch = (uchar)0x00;

    if(ui->cb_RetestSwitch->checkState() == Qt::Checked)
        struct_PCTestConfig.uc_RetestTimes = (uchar)0x01;
    else
        struct_PCTestConfig.uc_RetestTimes = (uchar)0x00;

    struct_PCTestConfig.un_FWPosition = ui->le_FWSequenceNumber->text().toUInt();
    struct_PCTestConfig.TestType = (ENUM_TESTTYPE)ui->le_FWType->text().toInt();
    struct_PCTestConfig.str_ProjectName = ui->le_ProjectName->text();
    struct_PCTestConfig.str_ProjectVersions = ui->le_ProjectVersions->text();

    QList<QString> list_DUTFWPositions;
    QList<QString> list_PowerDUTPositions;
    QList<QString> list_PowerTestGroups;
    ConfigFile o_ConfigFile;

    m_pPT_FWPositionNumber->GetLEValue(list_DUTFWPositions);
    m_pPT_PowerTestPositionNumber->GetLEValue(list_PowerDUTPositions);
    m_pPT_PowerTestGroup->GetLEValue(list_PowerTestGroups);

    o_ConfigFile.TransformToQString(list_DUTFWPositions,
                                    struct_PCTestConfig.str_DUTFWPositions);
    o_ConfigFile.TransformToQString(list_PowerDUTPositions,
                                    struct_PCTestConfig.str_PowerDUTPositions);
    o_ConfigFile.TransformToQString(list_PowerTestGroups,
                                    struct_PCTestConfig.str_PowerTestGroups);

    o_ConfigFile.SavePCTestConfig(m_usSequenceNumber,
                                 struct_PCTestConfig);

//    STRUCT_USBCONTROLCONFIG struct_UsbControlConfig;
//    QList<QString> list_EnumPositionPort;
//    QString str_EnumPositionPort;
//    m_pEnumPointTable->GetLEValue(list_EnumPositionPort);
//    o_ConfigFile.TransformToQString(list_EnumPositionPort,
//                                    str_EnumPositionPort);

//    o_ConfigFile.TransformToMap(str_EnumPositionPort,
//                                struct_UsbControlConfig.map_StationPort);

//    o_ConfigFile.SaveUsbControlConfig(m_usSequenceNumber,
//                                      struct_UsbControlConfig);
}
