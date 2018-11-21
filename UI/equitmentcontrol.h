#ifndef EQUITMENTCONTROL_H
#define EQUITMENTCONTROL_H

#include <QWidget>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"

namespace Ui {
class EquitmentControl;
}

class EquitmentControl : public QWidget
{
    Q_OBJECT
public:
    explicit EquitmentControl(QWidget *parent = 0);
    ~EquitmentControl();

private:
    void InitEquitmentControl();

    void InitEquitmentState();

    void InitUI();

private:
    void OpenBox(const ushort &us_SequenceNumber);

    void CloseBox(const ushort &us_SequenceNumber);

    void SendCatchRobotAction(const ushort &us_FWStation,
                              const QString &str_RobotAction);

    void SendSupplementRobotData(const ushort &us_FWStation);

public slots:
    void slot_BoxDiscoverd();

    void slot_CatchRobotDiscoverd();

    void slot_SupplementRobotDiscoverd();

    void slot_BoxRemove();

    void slot_CatchRobotRemove();

    void slot_SupplementRobotRemove();

    void slot_BoxOperatorUpdata(ushort us_SequenceNumber);

    void slot_CatchRobotGetActionUpdata(ushort us_SequenceNumber);

    void slot_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber);


private:
    Ui::EquitmentControl *ui;

    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;
};

#endif // EQUITMENTCONTROL_H
