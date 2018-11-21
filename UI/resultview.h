#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include <QWidget>
#include<QMap>
#include <QGridLayout>
#include <QStackedWidget>
#include "resultlabel.h"

const int OneGroupUsbNumber_ResultView = 20;

class ResultView : public QWidget
{
    Q_OBJECT

public:
    explicit ResultView(QWidget *parent = 0);
    ~ResultView();

    bool CreatResultView(const ushort &us_SequenceNumber);

    bool RemoveResultView(const ushort  &us_SequenceNumber);

    bool ShowResultView(const ushort &us_SequenceNumber);

    bool SetResultViewShow(const ushort &us_SequenceNumber,
                           const ENUM_TESTSTATE &TestState);

    bool UpdataResultView(const ushort &us_SequenceNumber,
                          const QList<bool> &list_TestResult,
                          const QList<QString> &list_ResultInfo);

private:
    void InitResultView();

private:
    QStackedWidget *m_pQStackedWidget;

    QMap<ushort, QWidget* > m_mapStackedWidget;
    QMap<ushort, QList<ResultLabel *>> m_mapResultView;
};

#endif // RESULTVIEW_H
