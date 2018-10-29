#ifndef POWERDEBUG_H
#define POWERDEBUG_H
#include <QWidget>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"

namespace Ui {
class PowerDebug;
}

class PowerDebug : public QWidget
{
    Q_OBJECT

public:
    explicit PowerDebug(ushort us_SequenceNumber, QWidget *parent = 0);
    ~PowerDebug();

    void SetSequenceNumber(ushort us_SequenceNumber);

private:
    void InitPowerDebug();

    void ExitPowerDebug();

    void AdjustPowerData();

    void ReadPowerData();

    QString AutoCoverDouble(int &n_Data);

    QString AutoCoverDouble(QString str_Info);

    void ShowInfo(QString str_Info);

    void GetReceiveCmdData(uchar uc_Command);

    void GetWriteCmdData(uchar uc_Command);

private:
    void EnterDebugModel();

    void ExitDebugModel();

    void AdjustPower_1();

    void AdjustPower_2();

    void ReadPower();

    void ClearOnePointPower();

    void ClearAllPower();

signals:

public slots:
    void slot_ReceiveCmd(ushort us_SequenceNumber,
                         uchar uc_Command);

    void slot_WriteCmd(ushort us_SequenceNumber,
                       uchar uc_Command);

    void slot_AdjustPower(ushort us_SequenceNumber);

    void slot_ReadPower(ushort us_SequenceNumber);

private:
    Ui::PowerDebug *ui;

    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;

    ushort m_usSequenceNumber;
};

#endif // POWERDEBUG_H
