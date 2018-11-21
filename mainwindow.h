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

enum ENUM_SHOWRECORDDATA{
    CURRENT_SHOW,
    TOTAL_SHOW
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
    void InitMainWindow();
    void InitUI();

    void InitMasterControl();

    void ConnectClick();

private:
    void USBConfig();
    void FWConfigUI();
    void EMConfigUI();
    void CPLimitUI();

    void FWDebugUI();
    void EMControlUI();
    void PWDebugUI();

    void DataTableUI();

private:
    void InitTest();

    void StartOneTest();

    void StartAutoTest();

    void StopTest();

private:
    void ShowRecordDataSwitch();

    void UpdataTestRecordData(const ENUM_SHOWRECORDDATA &ShowRecordData);

    void SaveTestRecordData();

private:
    void UpdataFWSequenceNumber();

    void UpdataBoxSequenceNumber();

public slots:
    void slot_FWDiscoverd();

    void slot_FWRemove();

    void slot_BoxDiscoverd();

    void slot_BoxRemove();

    void slot_CRobotDiscoverd();

    void slot_CRobotRemove();

    void slot_SRobotDiscoverd();

    void slot_SRobotRemove();

public slots:
    void slot_StartTest(ushort us_SequenceNumber);

    void slot_CompleteTest(ushort us_SequenceNumber);

private slots:
    void slot_ButtonClick(int n_ID);

    void slot_SaveStatisticalTableLog();

private:
    Ui::MainWindow *ui;
    CountTestResultInstanceGetter m_oCountTestResultInstanceGetter;

    MasterControl *m_pMasterControl;
    ResultView *m_pResultView;
    CountTestData *m_pCountTestData;

    QButtonGroup *m_pQButtonGroup;

    QList<ushort> m_listSequenceNumber;
    ushort m_SelectSequenceNumber;
    ENUM_SHOWRECORDDATA m_ShowRecordData;
};

#endif // MAINWINDOW_H
