#ifndef STATISTICALTABLE_H
#define STATISTICALTABLE_H
#include <QDialog>
#include <QTableWidget>
#include "UpperLogic/counttestresultinstancegetter.h"

/*
* 测试结果数据表格UI
* 显示数据，保存数据表格log    
*/

class StatisticalTable : public QDialog
{
    Q_OBJECT
public:
    explicit StatisticalTable(ushort us_SequenceNumber, QWidget *parent = 0);
    ~StatisticalTable();
    //设置工作序号
    void SetSequenceNumber(const ushort &us_SequenceNumber);
    //保存数据表格
    void SaveStatisticalTable();
    //保存表格数据log
    void SaveTableLog();

private:
    //另存为表格数据
    void SaveAsTableData();
    //清除表格数据
    void ClearTableData();
    //更新当前数据并保存
    void UpdateCurrentDataAndSave();

private:    
    //初始化数据表格
    void InitStatisticalTable();
    //初始化UI
    void InitUI();
    //初始化结果数据
    void InitResultData();
    //插入表格 行
    void InsertTableRow();
    //更新表格数据
    void UpdataTableData();

public slots:
    //结果数据更新 处理
    void slot_ResultUpdata();

private:
    QTableWidget *m_pQTableWidget;      //表格对象指针 
    CountTestResultInstanceGetter m_oCountTestResultInstanceGetter;     //计算结果类管理类对象

    CountTestData *m_pCountTestData;
    uint m_unCurrentRow;    //当前行数

    ushort m_usSequenceNumber;  //当前工作序号
};

#endif // STATISTICALTABLE_H
