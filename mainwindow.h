#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QButtonGroup>
#include "UI/usbcontrolconfig.h"
#include "UI/firmwareconfig.h"
#include "UI/equitmentconnectconfig.h"
#include "UI/countpowerlimit.h"

#include "UI/firewaredebug.h"
#include "UI/equitmentcontrol.h"
#include "UI/powerdebug.h"
#include "UI/resultview.h"
#include "UI/statisticaltable.h"

#include "UpperLogic/mastercontrol.h"

/*
* 主界面
*/

//显示记录数据类型
enum ENUM_SHOWRECORDDATA{
    CURRENT_SHOW,
    TOTAL_SHOW
};

//外围设备连接状态
enum EMUN_CONNECTSTATE{
    CONNECTTOPC,
    DISCONNECTTOPC
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //初始化
    void InitMainWindow();
    //初始化UI
    void InitUI();
    //初始化主控
    void InitMasterControl();
    //连接信号
    void ConnectClick();

private:
    //usb配置
    void USBConfig();
    //固件配置 UI
    void FWConfigUI();
    //设备配置 UI
    void EMConfigUI();
    //功率上下限设置 UI
    void CPLimitUI();
    //固件调试 UI
    void FWDebugUI();
    //设备调试控制 UI
    void EMControlUI();
    //功率调试 UI
    void PWDebugUI();
    //数据表格 UI
    void DataTableUI();

private:
    //初始化测试
    void InitTest();
    //开始一次DUT测试
    void StartOneTest();
    //开始自动DUT测试
    void StartAutoTest();
    //停止测试
    void StopTest();
    //重置测试
    void Resetting();
    //显示测试状态
    void ShowTestState(const ENUM_TESTSTATE &TestState);
    //显示连接状态 qlabel
    void ShowConnectState(QLabel *&p_QLabel,
                          const EMUN_CONNECTSTATE &ConnectState);

private:
    //显示记录数据类型开关
    void ShowRecordDataSwitch();
    //更新测试记录数据
    void UpdataTestRecordData(const ENUM_SHOWRECORDDATA &ShowRecordData);
    //保存测试记录数据
    void SaveTestRecordData();

private:    
    //更新固件工作序号
    void UpdataFWSequenceNumber();
    //更新箱子工作序号
    void UpdataBoxSequenceNumber();

public slots:
    //添加匹配新固件
    void slot_FWDiscoverd();
    //移除已匹配固件
    void slot_FWRemove();
    //添加匹配新箱子
    void slot_BoxDiscoverd();
    //添加抓取机器人
    void slot_BoxRemove();
    //添加分料机器人
    void slot_CRobotDiscoverd();
    //移除已匹配箱子
    void slot_CRobotRemove();
    //移除抓取机器人
    void slot_SRobotDiscoverd();
    //移除分料机器人
    void slot_SRobotRemove();

public slots:
    void slot_ReadyTest(ushort us_SequenceNumber);
    //开始测试通知    
    void slot_StartTest(ushort us_SequenceNumber);
    //完成测试通知
    void slot_CompleteTest(ushort us_SequenceNumber);

private slots:
    //按钮触发 选择工作序号
    void slot_ButtonClick(int n_ID);
    //保存数据表格log
    void slot_SaveStatisticalTableLog();

private:
    Ui::MainWindow *ui;
    CountTestResultInstanceGetter m_oCountTestResultInstanceGetter;

    MasterControl *m_pMasterControl;
    ResultView *m_pResultView;
    CountTestData *m_pCountTestData;

    QButtonGroup *m_pQButtonGroup;

    QList<ushort> m_listSequenceNumber;     //工作序号列表
    ushort m_SelectSequenceNumber;          //当前选择工作序号
    ENUM_SHOWRECORDDATA m_ShowRecordData;   //记录数据类型

    QTimer *m_pQTimer;                      //定时器 用于表格log保存

    bool m_bInit;

};

#endif // MAINWINDOW_H
