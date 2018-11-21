#ifndef STATISTICALTABLE_H
#define STATISTICALTABLE_H
#include <QDialog>
#include <QTableWidget>
#include "UpperLogic/counttestresultinstancegetter.h"

class StatisticalTable : public QDialog
{
    Q_OBJECT
public:
    explicit StatisticalTable(ushort us_SequenceNumber, QWidget *parent = 0);
    ~StatisticalTable();

    void SetSequenceNumber(const ushort &us_SequenceNumber);

    void SaveStatisticalTable();

    void SaveTableLog();

private:
    void SaveAsTableData();

    void ClearTableData();

    void UpdateCurrentDataAndSave();

private:
    void InitStatisticalTable();

    void InitUI();

    void InitResultData();

    void InsertTableRow();

    void UpdataTableData();

public slots:
    void slot_ResultUpdata();

private:
    QTableWidget *m_pQTableWidget;
    CountTestResultInstanceGetter m_oCountTestResultInstanceGetter;

    CountTestData *m_pCountTestData;
    uint m_unCurrentRow;

    ushort m_usSequenceNumber;
};

#endif // STATISTICALTABLE_H
