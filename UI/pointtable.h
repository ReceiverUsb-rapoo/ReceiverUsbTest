#ifndef POINTTABLE_H
#define POINTTABLE_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class PointTable;
}

class PointTable : public QWidget
{
    Q_OBJECT
public:
    explicit PointTable(QWidget *parent = 0);
    ~PointTable();

    bool SetLEValue(const QList<QString> &list_LEValue);

    bool GetLEValue(QList<QString> &list_LEValue);

    bool SetRepeatData(const bool &b_Repeat);

    bool ClearData();

private:
    void InitPointTable();

    void SetLEColor();

private slots:
    void slot_LEValueChange();

private:
    Ui::PointTable *ui;

    QList<QLineEdit *> m_listLE;
    bool m_bRepeatData;
    int m_nGroupNumber;
};

#endif // POINTTABLE_H
