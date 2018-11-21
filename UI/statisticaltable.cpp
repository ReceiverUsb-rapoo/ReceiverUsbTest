#include "statisticaltable.h"
#include <QTableWidget>
#include <QPushButton>
#include <QFont>
#include "DataFile/tablefile.h"

const int n_TableColumnCount = 4;

StatisticalTable::StatisticalTable(ushort us_SequenceNumber, QWidget *parent) :
    QDialog(parent)
{
    m_pQTableWidget = NULL;
    m_pCountTestData = NULL;

    m_usSequenceNumber = us_SequenceNumber;

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("StatisticalTable");

    QFont o_QFont;
    o_QFont.setPointSize(9);
    this->setFont(o_QFont);

    InitStatisticalTable();
}

StatisticalTable::~StatisticalTable()
{
    if(m_pQTableWidget != NULL){
        delete m_pQTableWidget;
        m_pQTableWidget = NULL;
    }
}

void StatisticalTable::SetSequenceNumber(const ushort &us_SequenceNumber)
{
    m_usSequenceNumber = us_SequenceNumber;
}

void StatisticalTable::SaveStatisticalTable()
{
    SaveAsTableData();
}

void StatisticalTable::SaveTableLog()
{
    TableFile o_TableFile;
    o_TableFile.SaveTableData_Log(m_usSequenceNumber,
                                  m_pQTableWidget);
}

void StatisticalTable::SaveAsTableData()
{
    TableFile o_TableFile;
    o_TableFile.SaveTableData_SelectAddress(m_usSequenceNumber,
                                            m_pQTableWidget);
}

void StatisticalTable::ClearTableData()
{
    m_pCountTestData->ClearAllResultData(m_usSequenceNumber);
    m_pQTableWidget->clearContents();
}

void StatisticalTable::UpdateCurrentDataAndSave()
{
    UpdataTableData();
    TableFile o_TableFile;
    o_TableFile.SaveTableData_FixeAddress(m_usSequenceNumber,
                                          m_pQTableWidget);
}

void StatisticalTable::InitStatisticalTable()
{
    m_pCountTestData = m_oCountTestResultInstanceGetter.GetInstance();

    connect(m_pCountTestData, SIGNAL(sig_ResultUpdata()),
            this, SLOT(slot_ResultUpdata()));

    InitUI();
    InitResultData();
}

void StatisticalTable::InitUI()
{
    this->setFixedSize(560,690);

    m_pQTableWidget = new QTableWidget(0,n_TableColumnCount);
    m_pQTableWidget->setParent(this);
    m_pQTableWidget->setGeometry(30,20,500,590);
    m_pQTableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    m_pQTableWidget->setShowGrid(false);             //设置不显示格子线
    QStringList strlist_HHeaderLabels;
    strlist_HHeaderLabels<<"枚举异常"<<"打开设备异常"<<"发送指令异常"<<"RF功率异常";
    m_pQTableWidget->setHorizontalHeaderLabels(strlist_HHeaderLabels);
    QFont o_QFont;
    o_QFont.setPointSize(9);
    m_pQTableWidget->setFont(o_QFont);

    QPushButton *p_QPushButton_UpdataAndSave = new QPushButton(this);
    p_QPushButton_UpdataAndSave->setText("更新/保存");
    p_QPushButton_UpdataAndSave->setGeometry(100, 630, 70, 30);

    QPushButton *p_QPushButton_Save = new QPushButton(this);
    p_QPushButton_Save->setText("另保为");
    p_QPushButton_Save->setGeometry(240, 630, 70, 30);

    QPushButton *p_QPushButton_Clear = new QPushButton(this);
    p_QPushButton_Clear->setText("清除");
    p_QPushButton_Clear->setGeometry(390, 630, 70, 30);

    connect(p_QPushButton_UpdataAndSave, &QPushButton::clicked,
            this, &StatisticalTable::UpdateCurrentDataAndSave);
    connect(p_QPushButton_Save, &QPushButton::clicked,
            this, &StatisticalTable::SaveAsTableData);
    connect(p_QPushButton_Clear, &QPushButton::clicked,
            this, &StatisticalTable::ClearTableData);
}

void StatisticalTable::InitResultData()
{
    QMap<uint, QMap<ENUM_RESULTTYPE, uint>> map_ErrorAmount;
    m_pCountTestData->GetAllResultData(m_usSequenceNumber,
                                       map_ErrorAmount);
    QMap<ENUM_RESULTTYPE, uint> map_TypeErrorTimes;

    for(int i = 0; i < map_ErrorAmount.count(); i++){
        InsertTableRow();
        map_TypeErrorTimes = map_ErrorAmount.value(i + 1);

        for(int j = 0; j < n_TableColumnCount; j++){
            m_pQTableWidget->item(i, j)->setText(
                        QString::number(map_TypeErrorTimes.value((ENUM_RESULTTYPE)j)));
        }
    }
}

void StatisticalTable::InsertTableRow()
{
    int n_RowCount = m_pQTableWidget->rowCount();
    m_pQTableWidget->insertRow(n_RowCount);
}

void StatisticalTable::UpdataTableData()
{
    QMap<uint, QMap<ENUM_RESULTTYPE, uint>> map_ErrorAmount;
    m_pCountTestData->GetAllResultData(m_usSequenceNumber,
                                       map_ErrorAmount);
    QMap<ENUM_RESULTTYPE, uint> map_TypeErrorTimes;

    if(map_ErrorAmount.isEmpty()){
        return;
    }

    if(map_ErrorAmount.count() != m_pQTableWidget->rowCount() ||
            m_pQTableWidget->rowCount() == 0){
        InitResultData();
    }

    map_TypeErrorTimes = map_ErrorAmount.value((uint)map_ErrorAmount.count());

    for(int j = 0; j < n_TableColumnCount; j++){
        if(map_TypeErrorTimes.contains((ENUM_RESULTTYPE)j)){
            m_pQTableWidget->item(map_ErrorAmount.count(), j)->setText(
                        QString::number(map_TypeErrorTimes.value((ENUM_RESULTTYPE)j)));
        }
        else{
            m_pQTableWidget->item(map_ErrorAmount.count(), j)->setText("0");
        }

    }
}

void StatisticalTable::slot_ResultUpdata()
{
    UpdataTableData();
}
