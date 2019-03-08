#ifndef POINTTABLE_H
#define POINTTABLE_H

#include <QWidget>
#include <QLineEdit>

/*
* 点表格UII
* 用于DUT位置，分组，组数
*/

namespace Ui {
class PointTable;
}

class PointTable : public QWidget
{
    Q_OBJECT
public:
    explicit PointTable(QWidget *parent = 0);
    ~PointTable();
    //设置单个值
    bool SetLEValue(const QList<QString> &list_LEValue);
    //获取单个值
    bool GetLEValue(QList<QString> &list_LEValue);
    //设置重复数据检测
    bool SetRepeatData(const bool &b_Repeat);
    //清除数据
    bool ClearData();

private:    
    //初始化
    void InitPointTable();
    //设置单个格背景颜色
    void SetLEColor();

private slots:
    //收到  单个格数据变化
    void slot_LEValueChange();

private:
    Ui::PointTable *ui;

    QList<QLineEdit *> m_listLE;    //单个格 list集合 
    bool m_bRepeatData;             //重复数据检测
    int m_nGroupNumber;             //无意义
};

#endif // POINTTABLE_H
