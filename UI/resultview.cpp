#include "resultview.h"
#include <QDebug>

ResultView::ResultView(QWidget *parent) :
    QWidget(parent)
{
    InitResultView();
//    this->setWindowTitle("ResultView");
}

ResultView::~ResultView()
{

}

bool ResultView::CreatResultView(const ushort &us_SequenceNumber)
{
    QList<ResultLabel *> list_ResultLabel;

    QWidget *p_QWidget = new QWidget();
    QGridLayout *p_QGridLayout = new QGridLayout(p_QWidget);
    for(int i = 0; i < OneGroupUsbNumber_ResultView; i++){
        ResultLabel *p_ResultLabel = new ResultLabel;
        p_ResultLabel->setParent(p_QWidget);
        p_ResultLabel->SetState(Ready_Test);
        p_QGridLayout->addWidget(p_ResultLabel,i/5,i%5 + 1,1,1);
        list_ResultLabel.append(p_ResultLabel);
    }

    m_mapStackedWidget.insert(us_SequenceNumber, p_QWidget);
    m_mapResultView.insert(us_SequenceNumber, list_ResultLabel);
    m_pQStackedWidget->addWidget(p_QWidget);

//    m_pQStackedWidget->insertWidget((int)us_SequenceNumber, p_QWidget);

    return true;
}

bool ResultView::RemoveResultView(const ushort &us_SequenceNumber)
{
    if(!m_mapStackedWidget.contains(us_SequenceNumber)){
        return false;
    }

    QWidget *p_QWidget = m_mapStackedWidget.value(us_SequenceNumber);
    m_mapStackedWidget.remove(us_SequenceNumber);
    m_mapResultView.remove(us_SequenceNumber);

    m_pQStackedWidget->removeWidget(p_QWidget);

    delete p_QWidget;
    p_QWidget = NULL;

    return true;
}

bool ResultView::ShowResultView(const ushort &us_SequenceNumber)
{
//    m_pQStackedWidget->setCurrentIndex(us_SequenceNumber);

    m_pQStackedWidget->setCurrentWidget(m_mapStackedWidget.value(us_SequenceNumber));

    return true;
}

bool ResultView::SetResultViewShow(const ushort &us_SequenceNumber,
                                   const ENUM_TESTSTATE &TestState)
{
    if(!m_mapResultView.contains(us_SequenceNumber)){
        return false;
    }

    for(int i = 0; i < OneGroupUsbNumber_ResultView; i++){
        m_mapResultView.value(us_SequenceNumber).at(i)->SetState(TestState);

    }
    return true;
}

bool ResultView::UpdataResultView(const ushort &us_SequenceNumber,
                                  const QList<bool> &list_TestResult,
                                  const QList<QString> &list_ResultInfo)
{
    if(list_TestResult.count() != OneGroupUsbNumber_ResultView){
        return false;
    }

    if(!m_mapResultView.contains(us_SequenceNumber)){
        return false;
    }

    for(int i = 0; i < OneGroupUsbNumber_ResultView; i++){
        if(list_TestResult.at(i)){
            m_mapResultView.value(us_SequenceNumber).at(i)->SetState(Pass_Test);
        }
        else{
            m_mapResultView.value(us_SequenceNumber).at(i)->SetState(Faile_Test);
            m_mapResultView.value(us_SequenceNumber).at(i)->setText("\n" +list_ResultInfo.at(i));
        }
    }
    return true;
}

void ResultView::InitResultView()
{
    QGridLayout *p_QGridLayout = new QGridLayout(this);
    m_pQStackedWidget = new QStackedWidget(this);

    p_QGridLayout->addWidget(m_pQStackedWidget);

    for(int i = 0; i < m_pQStackedWidget->count(); i++){
        m_pQStackedWidget->removeWidget(m_pQStackedWidget->widget(i));
    }
}
