#ifndef ENUMPOINTTABLE_H
#define ENUMPOINTTABLE_H

#include <QWidget>
#include <QLineEdit>

/*
* 用于保存夹具20位置的com端口识别ID
* 作为一个嵌入型ui（控件），只提供数据显示，数据保存，数据重复识别 
*/

namespace Ui {
class EnumPointTable;
}

class EnumPointTable : public QWidget
{
    Q_OBJECT
public:
    explicit EnumPointTable(QWidget *parent = 0);
    ~EnumPointTable();

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
    Ui::EnumPointTable *ui;

    QList<QLineEdit *> m_listLE;
    bool m_bRepeatData;
    int m_nGroupNumber;
};

#endif // ENUMPOINTTABLE_H
