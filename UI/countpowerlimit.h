#ifndef COUNTPOWERLIMIT_H
#define COUNTPOWERLIMIT_H

#include <QWidget>
#include <QLineEdit>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"


namespace Ui {
class CountPowerLimit;
}

class CountPowerLimit : public QWidget
{
    Q_OBJECT

public:
    explicit CountPowerLimit(ushort us_SequenceNumber, QWidget *parent = 0);
    ~CountPowerLimit();

    void SetSequenceNumber(const ushort &us_SequenceNumber);

private:
    void InitCountPowerLimit();

    void ExitCountPowerLimit();

    void StartCollect();

    void CountAverage();

    void CollectComplete();

    void CountAverageData();

    void CollectCompleteData();

    void StartTestNotice();

    void CompleteTest();

public slots:
    void slot_StartTestNotice(ushort us_SequenceNumber);

    void slot_CompleteTest(ushort us_SequenceNumber);

private:
    Ui::CountPowerLimit *ui;

    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;

    ushort m_usSequenceNumber;

    uint m_unCollectTime;
    QList<QLineEdit *> m_listAverageVaule;
    QList<QLineEdit *> m_listCollectVaule;
    QList<QLineEdit *> m_listUpperLimit;
    QList<QLineEdit *> m_listDowmLimit;
};

#endif // COUNTPOWERLIMIT_H
