#ifndef EQUITMENTCONTROL_H
#define EQUITMENTCONTROL_H

#include <QWidget>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"

/*
* 调试外围设备流程使用 
* 屏蔽箱 抓取机器人 分料机器人 操作指令以及数据调试
*/

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
    //初始化
    void InitEquitmentControl();
    //初始化设备状态
    void InitEquitmentState();
    //初始化UI
    void InitUI();

private:
    //按位置打开箱子
    void OpenBox(const ushort &us_SequenceNumber);
    //按位置关闭箱子
    void CloseBox(const ushort &us_SequenceNumber);
    //发送抓取cmd到抓取机器人
    void SendCatchRobotAction(const ushort &us_FWStation,
                              const QString &str_RobotAction);
    //发送测试数据到分料机器人
    void SendSupplementRobotData(const ushort &us_FWStation);

public slots:
    //发现新匹配box
    void slot_BoxDiscoverd();
    //发现新匹配抓取机器人
    void slot_CatchRobotDiscoverd();
    //发现新匹配分料机器人
    void slot_SupplementRobotDiscoverd();
    //移除已匹配的箱子
    void slot_BoxRemove();
    //移除已匹配的抓取机器人
    void slot_CatchRobotRemove();
    //移除已匹配的分料机器人
    void slot_SupplementRobotRemove();
    //箱子cmd更新
    void slot_BoxOperatorUpdata(ushort us_SequenceNumber);
    //抓取机器人cmd更新
    void slot_CatchRobotGetActionUpdata(ushort us_SequenceNumber);
    //分料机器人cmd更新
    void slot_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber);


private:
    Ui::EquitmentControl *ui;

    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;   //观察者管理类
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;   //操作者管理类

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;
};

#endif // EQUITMENTCONTROL_H
