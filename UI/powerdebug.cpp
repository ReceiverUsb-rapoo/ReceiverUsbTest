#include "powerdebug.h"
#include "ui_powerdebug.h"
#include <QtMath>
#include <QRegExp>
#include <QDebug>
#include "DataFile/configfile.h"

const int StationNumber = 32;

PowerDebug::PowerDebug(ushort us_SequenceNumber, QWidget *parent) :
    QWidget(parent), m_usSequenceNumber(us_SequenceNumber),
    ui(new Ui::PowerDebug)
{
    ui->setupUi(this);
    m_pDeviceObserver = NULL;
    m_pDeviceOperator = NULL;
    m_usSequenceNumber = 0;

    this->setAttribute(Qt::WA_DeleteOnClose);

    InitPowerDebug();
}

PowerDebug::~PowerDebug()
{
    ExitPowerDebug();
    delete ui;
}

void PowerDebug::SetSequenceNumber(ushort us_SequenceNumber)
{
    m_usSequenceNumber = us_SequenceNumber;
}

void PowerDebug::InitPowerDebug()
{
    m_pDeviceObserver = m_oDeviceObserverInstanceGetter.GetInstance();
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();

    QList<Firmware *> list_FWObjectPointer;
    m_pDeviceObserver->GetFWObjectPointer(list_FWObjectPointer);
    m_pDeviceOperator->SetFWObjectPointer(list_FWObjectPointer);

    connect(m_pDeviceObserver, SIGNAL(sig_ReceiveCmd(ushort,uchar)),
            this, SLOT(slot_ReceiveCmd(ushort,uchar)));
    connect(m_pDeviceObserver, SIGNAL(sig_WriteCmd(ushort,uchar)),
            this, SLOT(slot_WriteCmd(ushort,uchar)));
    connect(m_pDeviceObserver, SIGNAL(sig_AdjustPower(ushort)),
            this, SLOT(slot_AdjustPower(ushort)));
    connect(m_pDeviceObserver, SIGNAL(sig_ReadPower(ushort)),
            this, SLOT(slot_ReadPower(ushort)));

    QString RegExp = "^(([1-9]|([1-2]\\d|(3[0-2]))))$";
    QRegExp o_QRegExp(RegExp);
    QValidator *p_Validator = new QRegExpValidator(o_QRegExp, this);
    ui->le_TestPositionNumber->setValidator(p_Validator);

    connect(ui->pb_EnterDebugModel, &QPushButton::pressed,
            this, &PowerDebug::EnterDebugModel);
    connect(ui->pb_ExitDebugModel, &QPushButton::pressed,
            this, &PowerDebug::ExitDebugModel);
    connect(ui->pb_AdjustPower_1, &QPushButton::pressed,
            this, &PowerDebug::AdjustPower_1);
    connect(ui->pb_AdjustPower_2, &QPushButton::pressed,
            this, &PowerDebug::AdjustPower_2);
    connect(ui->pb_ReadPower, &QPushButton::pressed,
            this, &PowerDebug::ReadPower);
    connect(ui->pb_ClearAdjustPowerByPositionNumber, &QPushButton::pressed,
            this, &PowerDebug::ClearOnePointPower);
    connect(ui->pb_ClearAllAdjustPower, &QPushButton::pressed,
            this, &PowerDebug::ClearAllPower);
}

void PowerDebug::ExitPowerDebug()
{
    disconnect(m_pDeviceObserver, SIGNAL(sig_ReceiveCmd(ushort,uchar)),
            this, SLOT(slot_ReceiveCmd(ushort,uchar)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_WriteCmd(ushort,uchar)),
            this, SLOT(slot_WriteCmd(ushort,uchar)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_AdjustPower(ushort)),
            this, SLOT(slot_AdjustPower(ushort)));
    disconnect(m_pDeviceObserver, SIGNAL(sig_ReadPower(ushort)),
               this, SLOT(slot_ReadPower(ushort)));
}

void PowerDebug::AdjustPowerData()
{
    short s_Vslop = 0;
    short s_VslopAdc = 0;
    short s_Intercept = 0;
    m_pDeviceObserver->GetAdjustPowerData(CURRENT,
                                          m_usSequenceNumber,
                                          s_Vslop,
                                          s_VslopAdc,
                                          s_Intercept);

    ui->le_Vslop->setText(QString::number(s_Vslop));
    ui->le_VslopAdc->setText(QString::number(s_VslopAdc));
    ui->le_Intercept->setText(QString::number(s_Intercept));
}

void PowerDebug::ReadPowerData()
{
    QString str_Info = "读取校准功率\n";
    QList<short> list_Vslop;
    QList<short> list_VslopAdc;
    QList<short> list_Intercept;
    QList<char> list_db_Compensation;
    short s_ReferenceVoltage_1;
    short s_ReferenceVoltage_2;

    m_pDeviceObserver->GetReadPowerData(CURRENT,
                                        m_usSequenceNumber,
                                        list_Vslop,
                                        list_VslopAdc,
                                        list_Intercept,
                                        list_db_Compensation,
                                        s_ReferenceVoltage_1,
                                        s_ReferenceVoltage_2);

    if(list_db_Compensation.count() != StationNumber ||
       list_Intercept.count() != StationNumber ||
       list_Vslop.count() != StationNumber ||
       list_VslopAdc.count() != StationNumber){
        return;
    }

    for(int i = 0; i < StationNumber; i ++){
        str_Info += "编号" + AutoCoverDouble(i) + "  " +
                "Vslop " + QString::number(list_Vslop.at(i)) + "  ";
        str_Info += "VslopAdc " + QString::number(list_VslopAdc.at(i)) + "  ";
        str_Info += "Intercept " + QString::number(list_Intercept.at(i)) + "  ";
        str_Info += "补偿db " + QString::number((signed char)list_db_Compensation.at(i)) + "\n";
    }

    str_Info += "参考电压1 " + QString::number(s_ReferenceVoltage_1) + "mV\n";
    str_Info += "参考电压2 " + QString::number(s_ReferenceVoltage_2) + "mV\n";

    ShowInfo(str_Info);
}

QString PowerDebug::AutoCoverDouble(int &n_Data)
{
    QString str_Data = QString::number(n_Data);
    if(str_Data.count() == 1){
        str_Data += "  ";
    }

    return str_Data;
}

QString PowerDebug::AutoCoverDouble(QString str_Info)
{
    if(str_Info.count() == 1){
        str_Info = "0" + str_Info;
    }

    return str_Info;
}

void PowerDebug::ShowInfo(QString str_Info)
{
    QDateTime o_QDateTime = QDateTime::currentDateTime();
    QString str_InfoAddTime = QString("%1:%2:%3 %4\n").
            arg(o_QDateTime.time().hour()).
            arg(o_QDateTime.time().minute()).
            arg(o_QDateTime.time().second()).
            arg(str_Info);

    ui->tb_InfoShow->append(str_InfoAddTime);
}

void PowerDebug::GetReceiveCmdData(uchar uc_Command)
{
    STRUCT_COMMANDANDDATA struct_CommandAndData;
    QString str_Info;

    m_pDeviceObserver->GetReceiveCommandAndData(m_usSequenceNumber,
                                                uc_Command,
                                                struct_CommandAndData);

    str_Info += "receive cmd  \n" +
            QString::number(struct_CommandAndData.uc_Command, 16).toUpper() + "  ";

    for(int i = 0; i < (int)struct_CommandAndData.un_DataLength; i++){
        str_Info += AutoCoverDouble(QString::number((uchar)struct_CommandAndData.DATA[i], 16))+ " ";
    }

    ShowInfo(str_Info);
}

void PowerDebug::GetWriteCmdData(uchar uc_Command)
{
    STRUCT_COMMANDANDDATA struct_CommandAndData;
    QString str_Info;

    m_pDeviceObserver->GetWriteCommandAndData(m_usSequenceNumber,
                                              uc_Command,
                                              struct_CommandAndData);

    str_Info += "write cmd  \n" +
            QString::number(struct_CommandAndData.uc_Command, 16).toUpper() + "  ";

    for(int i = 0; i < (int)struct_CommandAndData.un_DataLength; i++){
        str_Info += AutoCoverDouble(QString::number((uchar)struct_CommandAndData.DATA[i], 16)) + " ";
    }

    ShowInfo(str_Info);
}

void PowerDebug::EnterDebugModel()
{
    m_pDeviceOperator->EnterDebugModel(m_usSequenceNumber);
}

void PowerDebug::ExitDebugModel()
{
    m_pDeviceOperator->ExitDebugModel(m_usSequenceNumber);
}

void PowerDebug::AdjustPower_1()
{
    char c_Funtion = 0x00;
    short s_ReferenceVoltage_1 = ui->le_ADCVoltage_1->text().toShort();
    short s_ReferenceVoltage_2 = ui->le_ADCVoltage_2->text().toShort();
    uchar uc_Station = (uchar)ui->le_TestPositionNumber->text().toShort();
    char c_AdjustPoint = (signed char)ui->le_CalibratedPoint->text().toShort();
    char c_dp_Compensation = (signed char)ui->le_Compensation_db->text().toShort();

    m_pDeviceOperator->AdjustPower(m_usSequenceNumber,
                                   c_Funtion,
                                   s_ReferenceVoltage_1,
                                   s_ReferenceVoltage_2,
                                   uc_Station,
                                   c_AdjustPoint,
                                   c_dp_Compensation);
}

void PowerDebug::AdjustPower_2()
{
    char c_Funtion = 0x01;
    short s_ReferenceVoltage_1 = ui->le_ADCVoltage_1->text().toShort();
    short s_ReferenceVoltage_2 = ui->le_ADCVoltage_2->text().toShort();
    uchar uc_Station = (uchar)ui->le_TestPositionNumber->text().toShort();
    char c_AdjustPoint = (signed char)ui->le_CalibratedPoint->text().toShort();
    char c_dp_Compensation = (signed char)ui->le_Compensation_db->text().toShort();

    m_pDeviceOperator->AdjustPower(m_usSequenceNumber,
                                   c_Funtion,
                                   s_ReferenceVoltage_1,
                                   s_ReferenceVoltage_2,
                                   uc_Station,
                                   c_AdjustPoint,
                                   c_dp_Compensation);
}

void PowerDebug::ReadPower()
{
    m_pDeviceOperator->ReadPowerData(m_usSequenceNumber);
}

void PowerDebug::ClearOnePointPower()
{
//    double d_Station = ui->le_TestPositionNumber->text().toDouble();
//    double d_Result = qPow(2,d_Station);
//    m_pDeviceOperator->ClearPowerConfig(m_usSequenceNumber,
//                                        (int)d_Result);
    int n_Station = ui->le_TestPositionNumber->text().toInt() - 1;
    int n_Poistion = 1;
    n_Poistion = n_Poistion<<n_Station;

    m_pDeviceOperator->ClearPowerConfig(m_usSequenceNumber,
                                        n_Poistion);
}

void PowerDebug::ClearAllPower()
{
    int n_Poistion = -1;
    m_pDeviceOperator->ClearPowerConfig(m_usSequenceNumber,
                                        n_Poistion);
}

void PowerDebug::slot_ReceiveCmd(ushort us_SequenceNumber,
                                 uchar uc_Command)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetReceiveCmdData(uc_Command);
}

void PowerDebug::slot_WriteCmd(ushort us_SequenceNumber,
                               uchar uc_Command)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    GetWriteCmdData(uc_Command);
}

void PowerDebug::slot_AdjustPower(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    AdjustPowerData();
}

void PowerDebug::slot_ReadPower(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber)
        return;

    ReadPowerData();
}
