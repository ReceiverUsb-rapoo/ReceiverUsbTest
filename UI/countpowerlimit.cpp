#include "countpowerlimit.h"
#include "ui_countpowerlimit.h"
#include <QGridLayout>
#include <QLabel>
#include "DataFile/configfile.h"

CountPowerLimit::CountPowerLimit(ushort us_SequenceNumber, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CountPowerLimit)
{
    ui->setupUi(this);
    m_pDeviceObserver = NULL;
    m_pDeviceOperator = NULL;
    m_usSequenceNumber = us_SequenceNumber;
    m_unCollectTime = 0;

    InitCountPowerLimit();
    this->setAttribute(Qt::WA_DeleteOnClose);
}

CountPowerLimit::~CountPowerLimit()
{
    ExitCountPowerLimit();
    delete ui;
}

void CountPowerLimit::SetSequenceNumber(const ushort &us_SequenceNumber)
{
    m_usSequenceNumber = us_SequenceNumber;
}

void CountPowerLimit::InitCountPowerLimit()
{

    m_pDeviceObserver = m_oDeviceObserverInstanceGetter.GetInstance();
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();

    QList<Firmware *> list_FWObjectPointer;
    m_pDeviceObserver->GetFWObjectPointer(list_FWObjectPointer);
    m_pDeviceOperator->SetFWObjectPointer(list_FWObjectPointer);

    connect(m_pDeviceObserver, SIGNAL(sig_StartTestNotice(ushort)),
            this, SLOT(slot_StartTestNotice(ushort)));

    connect(m_pDeviceObserver, SIGNAL(sig_CompleteTest(ushort)),
            this, SLOT(slot_CompleteTest(ushort)));


    m_listAverageVaule<<ui->le_AverageValue_1<<ui->le_AverageValue_2<<ui->le_AverageValue_3
                      <<ui->le_AverageValue_4<<ui->le_AverageValue_5<<ui->le_AverageValue_6
                      <<ui->le_AverageValue_7<<ui->le_AverageValue_8<<ui->le_AverageValue_9
                      <<ui->le_AverageValue_10<<ui->le_AverageValue_11<<ui->le_AverageValue_12
                      <<ui->le_AverageValue_13<<ui->le_AverageValue_14<<ui->le_AverageValue_15
                      <<ui->le_AverageValue_16<<ui->le_AverageValue_17<<ui->le_AverageValue_18
                      <<ui->le_AverageValue_19<<ui->le_AverageValue_20<<ui->le_AverageValue_21
                      <<ui->le_AverageValue_22<<ui->le_AverageValue_23<<ui->le_AverageValue_24
                      <<ui->le_AverageValue_25<<ui->le_AverageValue_26<<ui->le_AverageValue_27
                      <<ui->le_AverageValue_28<<ui->le_AverageValue_29<<ui->le_AverageValue_30
                      <<ui->le_AverageValue_31<<ui->le_AverageValue_32;


    m_listCollectVaule<<ui->le_CollectVaule_1<<ui->le_CollectVaule_2<<ui->le_CollectVaule_3
                      <<ui->le_CollectVaule_4<<ui->le_CollectVaule_5<<ui->le_CollectVaule_6
                      <<ui->le_CollectVaule_7<<ui->le_CollectVaule_8<<ui->le_CollectVaule_9
                      <<ui->le_CollectVaule_10<<ui->le_CollectVaule_11<<ui->le_CollectVaule_12
                      <<ui->le_CollectVaule_13<<ui->le_CollectVaule_14<<ui->le_CollectVaule_15
                      <<ui->le_CollectVaule_16<<ui->le_CollectVaule_17<<ui->le_CollectVaule_18
                      <<ui->le_CollectVaule_19<<ui->le_CollectVaule_20<<ui->le_CollectVaule_21
                      <<ui->le_CollectVaule_22<<ui->le_CollectVaule_23<<ui->le_CollectVaule_24
                      <<ui->le_CollectVaule_25<<ui->le_CollectVaule_26<<ui->le_CollectVaule_27
                      <<ui->le_CollectVaule_28<<ui->le_CollectVaule_29<<ui->le_CollectVaule_30
                      <<ui->le_CollectVaule_31<<ui->le_CollectVaule_32;

    m_listUpperLimit<<ui->le_UpperLimit_1<<ui->le_UpperLimit_2<<ui->le_UpperLimit_3
                    <<ui->le_UpperLimit_4<<ui->le_UpperLimit_5<<ui->le_UpperLimit_6
                    <<ui->le_UpperLimit_7<<ui->le_UpperLimit_8<<ui->le_UpperLimit_9
                    <<ui->le_UpperLimit_10<<ui->le_UpperLimit_11<<ui->le_UpperLimit_12
                    <<ui->le_UpperLimit_13<<ui->le_UpperLimit_14<<ui->le_UpperLimit_15
                    <<ui->le_UpperLimit_16<<ui->le_UpperLimit_17<<ui->le_UpperLimit_18
                    <<ui->le_UpperLimit_19<<ui->le_UpperLimit_20<<ui->le_UpperLimit_21
                    <<ui->le_UpperLimit_22<<ui->le_UpperLimit_23<<ui->le_UpperLimit_24
                    <<ui->le_UpperLimit_25<<ui->le_UpperLimit_26<<ui->le_UpperLimit_27
                    <<ui->le_UpperLimit_28<<ui->le_UpperLimit_29<<ui->le_UpperLimit_30
                    <<ui->le_UpperLimit_31<<ui->le_UpperLimit_32;

    m_listDowmLimit<<ui->le_DownLimit_1<<ui->le_DownLimit_2<<ui->le_DownLimit_3
                   <<ui->le_DownLimit_4<<ui->le_DownLimit_5<<ui->le_DownLimit_6
                   <<ui->le_DownLimit_7<<ui->le_DownLimit_8<<ui->le_DownLimit_9
                   <<ui->le_DownLimit_10<<ui->le_DownLimit_11<<ui->le_DownLimit_12
                   <<ui->le_DownLimit_13<<ui->le_DownLimit_14<<ui->le_DownLimit_15
                   <<ui->le_DownLimit_16<<ui->le_DownLimit_17<<ui->le_DownLimit_18
                   <<ui->le_DownLimit_19<<ui->le_DownLimit_20<<ui->le_DownLimit_21
                   <<ui->le_DownLimit_22<<ui->le_DownLimit_23<<ui->le_DownLimit_24
                   <<ui->le_DownLimit_25<<ui->le_DownLimit_26<<ui->le_DownLimit_27
                   <<ui->le_DownLimit_28<<ui->le_DownLimit_29<<ui->le_DownLimit_30
                   <<ui->le_DownLimit_31<<ui->le_DownLimit_32;

    /*
    QWidget *p_QWidget = new QWidget;
    p_QWidget->setFixedSize(600,400);
//    p_QWidget->move(55,55);

    QGridLayout *p_QGridLayout = new QGridLayout(p_QWidget);
    QLabel *p_QLabel_1 = new QLabel(QStringLiteral("位置编号"));
    QLabel *p_QLabel_2 = new QLabel(QStringLiteral("平均值"));
    QLabel *p_QLabel_3 = new QLabel(QStringLiteral("采集值"));
    QLabel *p_QLabel_4 = new QLabel(QStringLiteral("上限幅度"));
    QLabel *p_QLabel_5 = new QLabel(QStringLiteral("下限幅度"));
    QLabel *p_QLabel_6 = new QLabel(QStringLiteral("位置编号"));
    QLabel *p_QLabel_7 = new QLabel(QStringLiteral("平均值"));
    QLabel *p_QLabel_8 = new QLabel(QStringLiteral("采集值"));
    QLabel *p_QLabel_9 = new QLabel(QStringLiteral("上限幅度"));
    QLabel *p_QLabel_10 = new QLabel(QStringLiteral("下限幅度"));

    p_QLabel_1->setAlignment(Qt::AlignCenter);
    p_QLabel_2->setAlignment(Qt::AlignCenter);
    p_QLabel_3->setAlignment(Qt::AlignCenter);
    p_QLabel_4->setAlignment(Qt::AlignCenter);
    p_QLabel_5->setAlignment(Qt::AlignCenter);
    p_QLabel_6->setAlignment(Qt::AlignCenter);
    p_QLabel_7->setAlignment(Qt::AlignCenter);
    p_QLabel_8->setAlignment(Qt::AlignCenter);
    p_QLabel_9->setAlignment(Qt::AlignCenter);
    p_QLabel_10->setAlignment(Qt::AlignCenter);

    p_QGridLayout->addWidget(p_QLabel_1,0,1,1,1);
    p_QGridLayout->addWidget(p_QLabel_2,0,2,1,1);
    p_QGridLayout->addWidget(p_QLabel_3,0,3,1,1);
    p_QGridLayout->addWidget(p_QLabel_4,0,4,1,1);
    p_QGridLayout->addWidget(p_QLabel_5,0,5,1,1);
    p_QGridLayout->addWidget(p_QLabel_6,0,8,1,1);
    p_QGridLayout->addWidget(p_QLabel_7,0,9,1,1);
    p_QGridLayout->addWidget(p_QLabel_8,0,10,1,1);
    p_QGridLayout->addWidget(p_QLabel_9,0,11,1,1);
    p_QGridLayout->addWidget(p_QLabel_10,0,12,1,1);

    for(int i = 1; i <= 32; i++){
        QLabel *p_QLabel = new QLabel(QString::number(i));
        QLineEdit *p_QLineEdit_1 = new QLineEdit("0");
        QLineEdit *p_QLineEdit_2 = new QLineEdit("0");
        QLineEdit *p_QLineEdit_3 = new QLineEdit("0");
        QLineEdit *p_QLineEdit_4 = new QLineEdit("0");

        p_QLabel->setAlignment(Qt::AlignCenter);
        p_QLineEdit_1->setAlignment(Qt::AlignCenter);
        p_QLineEdit_2->setAlignment(Qt::AlignCenter);
        p_QLineEdit_3->setAlignment(Qt::AlignCenter);
        p_QLineEdit_4->setAlignment(Qt::AlignCenter);
        qDebug()<<i;

        if(i <= 16){
            p_QGridLayout->addWidget(p_QLabel,i,1,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_1,i,2,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_2,i,3,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_3,i,4,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_4,i,5,1,1,Qt::AlignCenter);
        }
        else{
            int b = i - 16;
            p_QGridLayout->addWidget(p_QLabel,b,8,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_1,b,9,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_2,b,10,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_3,b,11,1,1,Qt::AlignCenter);
            p_QGridLayout->addWidget(p_QLineEdit_4,b,12,1,1,Qt::AlignCenter);
        }
    }
    */

    ui->pb_CollectComplete->setEnabled(true);
    ui->pb_CollectOn->setEnabled(false);

    connect(ui->pb_StartOneCollect, &QPushButton::clicked,
            this, &CountPowerLimit::StartCollect);
    connect(ui->pb_CollectOn, &QPushButton::clicked,
            this, &CountPowerLimit::CountAverage);
    connect(ui->pb_CollectComplete, &QPushButton::clicked,
            this, &CountPowerLimit::CollectComplete);
}

void CountPowerLimit::ExitCountPowerLimit()
{
    disconnect(m_pDeviceObserver, SIGNAL(sig_StartTestNotice(ushort)),
            this, SLOT(slot_StartTestNotice(ushort)));

    disconnect(m_pDeviceObserver, SIGNAL(sig_CompleteTest(ushort)),
            this, SLOT(slot_CompleteTest(ushort)));
}

void CountPowerLimit::StartCollect()
{
    m_pDeviceOperator->StartOneTest(m_usSequenceNumber);

    for(int i = 0; i < 32; i++){
        m_listCollectVaule.at(i)->setText("0");
    }
}

void CountPowerLimit::CountAverage()
{
    ui->pb_CollectOn->setEnabled(false);
    CountAverageData();
}

void CountPowerLimit::CollectComplete()
{
    CollectCompleteData();
}

void CountPowerLimit::CountAverageData()
{
    int n_CountVaule = 0;
    for(int i = 0; i < 32; i++){
        n_CountVaule = (m_listAverageVaule.at(i)->text().toInt() +
                        m_listCollectVaule.at(i)->text().toInt())/2;
        m_listDowmLimit.at(i)->setText(QString::number(n_CountVaule));
    }
}

void CountPowerLimit::CollectCompleteData()
{
    ConfigFile o_ConfigFile;
    STRUCT_PCTESTCONFIG struct_PCTestConfig;
    QList<int> list_UpperLimit;
    QList<int> list_DownLimit;
    QString str_UpperLimit;
    QString str_DownLimit;

    int n_CountVaule = 0;

    o_ConfigFile.GetPCTestConfig(m_usSequenceNumber, struct_PCTestConfig);

    for(int i = 0; i < 32; i++){
        n_CountVaule = m_listAverageVaule.at(i)->text().toInt() +
                        m_listUpperLimit.at(i)->text().toInt();

        list_UpperLimit.append(n_CountVaule);
    }

    for(int i = 0; i < 32; i++){
        n_CountVaule = m_listAverageVaule.at(i)->text().toInt() +
                        m_listUpperLimit.at(i)->text().toInt();

        list_DownLimit.append(n_CountVaule);
    }

    o_ConfigFile.TransformToQString(list_UpperLimit, str_UpperLimit);
    o_ConfigFile.TransformToQString(list_DownLimit, str_DownLimit);

    struct_PCTestConfig.str_RFPowerDBUponLimit = str_UpperLimit;
    struct_PCTestConfig.str_RFPowerDBLowerLimit = str_DownLimit;

    o_ConfigFile.SavePCTestConfig(m_usSequenceNumber, struct_PCTestConfig);
}

void CountPowerLimit::StartTestNotice()
{
    ui->pb_StartOneCollect->setEnabled(false);
}

void CountPowerLimit::CompleteTest()
{
    QList<short> list_Power_db;

    m_unCollectTime ++;
    ui->pb_StartOneCollect->setEnabled(true);
    ui->pb_CollectComplete->setEnabled(true);
    ui->le_CollectTime->setText(QString::number(m_unCollectTime));

    m_pDeviceObserver->GetUploadRFPowerResultData(CURRENT,
                                                  m_usSequenceNumber,
                                                  list_Power_db);

    if(list_Power_db.count() != 32){
        return;
    }

    ui->pb_CollectOn->setEnabled(true);

    for(int i = 0; i < 32; i++){
        m_listCollectVaule.at(i)->setText(QString::number(list_Power_db.at(i)));
    }
}

void CountPowerLimit::slot_StartTestNotice(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber){
        return;
    }

    StartTestNotice();
}

void CountPowerLimit::slot_CompleteTest(ushort us_SequenceNumber)
{
    if(us_SequenceNumber != m_usSequenceNumber){
        return;
    }

    CompleteTest();
}
