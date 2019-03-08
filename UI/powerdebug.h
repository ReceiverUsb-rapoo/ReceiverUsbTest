#ifndef POWERDEBUG_H
#define POWERDEBUG_H
#include <QWidget>
#include "UpperLogic/deviceobserverinstancegetter.h"
#include "UpperLogic/deviceoperatorinstancegetter.h"

/*
* 功率调试界面
* 用于调试固件功率
*/

namespace Ui {
class PowerDebug;
}

class PowerDebug : public QWidget
{
    Q_OBJECT

public:
    explicit PowerDebug(ushort us_SequenceNumber, QWidget *parent = 0);
    ~PowerDebug();
    //设置工作序号
    void SetSequenceNumber(ushort us_SequenceNumber);

private:
    //初始化
    void InitPowerDebug();
    //退出
    void ExitPowerDebug();
    //校准功率数据 
    void AdjustPowerData();
    //读取功率数据
    void ReadPowerData();
    //数据显示长度补全 1位自动添加空格（1 ） 
    QString AutoCoverDouble(int &n_Data);
    //数据位置显示补全 1位自动补0（01）
    QString AutoCoverDouble(QString str_Info);
    //显示调试信息
    void ShowInfo(QString str_Info);
    //获取 接收cmd数据
    void GetReceiveCmdData(uchar uc_Command);
    //获取 写入cmd数据
    void GetWriteCmdData(uchar uc_Command);

private:
    //进入调试模式
    void EnterDebugModel();
    //退出调试模式
    void ExitDebugModel();
    //校准功率1
    void AdjustPower_1();
    //校准功率2
    void AdjustPower_2();
    //读取校准功率
    void ReadPower();
    //清除校准功率
    void ClearOnePointPower();
    //清除所有校准功率v数据
    void ClearAllPower();

signals:

public slots:
    //收到 接收cmd数据
    void slot_ReceiveCmd(ushort us_SequenceNumber,
                         uchar uc_Command);
    //收到 写入cmd数据
    void slot_WriteCmd(ushort us_SequenceNumber,
                       uchar uc_Command);
    //收到 校准功率
    void slot_AdjustPower(ushort us_SequenceNumber);
    //收到 读取校准功率
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
