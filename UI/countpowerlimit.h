#ifndef COUNTPOWERLIMIT_H
#define COUNTPOWERLIMIT_H

#include <QWidget>
#include <QLineEdit>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"
#include "UpperLogic/counttestresultinstancegetter.h"

/*
 * 用于计算功率上下限，并设置测试上下限。
 * 所有采集过程都是一个完整DUT测试
 * 通过采集上下限，通过加权平均计算稳定的功率 ，并加减部分上下限额定数值，计算出所有位置点的上下限  
*/

namespace Ui {
class CountPowerLimit;
}

class CountPowerLimit : public QWidget
{
    Q_OBJECT

public:
    explicit CountPowerLimit(ushort us_SequenceNumber, QWidget *parent = 0);
    ~CountPowerLimit();
    //设置工作序号
    void SetSequenceNumber(const ushort &us_SequenceNumber);

private:
    //初始化 固件配置 界面配置 获取观察者和操作者指针
    void InitCountPowerLimit();
    //退出 保存清除配置 
    void ExitCountPowerLimit();
    //开始采集数据 
    void StartCollect();
    //计算平均值
    void CountAverage();
    //采集完成，保存上下限结果 
    void CollectComplete();
    //计算平均值数据
    void CountAverageData();
    //采集完成数据
    void CollectCompleteData();
    //开始测试通知
    void StartTestNotice();
    //完成测试
    void CompleteTest();

private:
    //延时时间
    void WorkSleep(uint un_Msec);

public slots:
    //开始测试通知
    void slot_StartTestNotice(ushort us_SequenceNumber);
    //完成测试通知
    void slot_CompleteTest(ushort us_SequenceNumber);

private:
    Ui::CountPowerLimit *ui;

    DeviceObserverInstanceGetter m_oDeviceObserverInstanceGetter;   //观察者管理类
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;   //操作者管理类
    CountTestResultInstanceGetter m_oCountTestResultInstanceGetter;

    DeviceObserver *m_pDeviceObserver;
    DeviceOperator *m_pDeviceOperator;
    CountTestData *m_pCountTestData;

    ushort m_usSequenceNumber;      //工作序号

    uint m_unCollectTime;
    QList<QLineEdit *> m_listAverageVaule;  //平均值 
    QList<QLineEdit *> m_listCollectVaule;  //采集值
    QList<QLineEdit *> m_listUpperLimit;    //上限
    QList<QLineEdit *> m_listDowmLimit;     //下限 
};

#endif // COUNTPOWERLIMIT_H
