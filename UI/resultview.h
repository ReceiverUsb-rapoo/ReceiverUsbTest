#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include <QWidget>
#include<QMap>
#include <QGridLayout>
#include <QStackedWidget>
#include "resultlabel.h"

/*
* 主界面显示结果UI，图片显示
* 
*/

const int OneGroupUsbNumber_ResultView = 20;

class ResultView : public QWidget
{
    Q_OBJECT

public:
    explicit ResultView(QWidget *parent = 0);
    ~ResultView();
    //创建结果显示界面
    bool CreatResultView(const ushort &us_SequenceNumber);
    //移除结果显示界面
    bool RemoveResultView(const ushort  &us_SequenceNumber);
    //显示结果显示界面
    bool ShowResultView(const ushort &us_SequenceNumber);
    //设置结果显示界面
    bool SetResultViewShow(const ushort &us_SequenceNumber,
                           const ENUM_TESTSTATE &TestState);
    //更新结果显示界面
    bool UpdataResultView(const ushort &us_SequenceNumber,
                          const QList<bool> &list_TestResult,
                          const QList<QString> &list_ResultInfo);

private:    
    //初始化UI
    void InitResultView();

private:
    QStackedWidget *m_pQStackedWidget;

    QMap<ushort, QWidget* > m_mapStackedWidget;         //结果UI <工作序号,结果ui>
    QMap<ushort, QList<ResultLabel *>> m_mapResultView; //结果单独结果显示UI， <工作序号, 单独结果显示UI>
};

#endif // RESULTVIEW_H
