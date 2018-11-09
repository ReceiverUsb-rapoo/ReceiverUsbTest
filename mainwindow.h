#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "UI/usbcontrolconfig.h"
#include "UI/firmwareconfig.h"
#include "UI/equitmentconnectconfig.h"
#include "UI/countpowerlimit.h"

#include "UI/firewaredebug.h"
#include "UI/equitmentcontrol.h"
#include "UI/powerdebug.h"

#include "UI/statisticaltable.h"

#include "UpperLogic/mastercontrol.h"

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

    void USBConfig();
    void FWConfigUI();
    void EMConfigUI();
    void CPLimitUI();

    void FWDebugUI();
    void EMControlUI();
    void PWDebugUI();

    void DataTableUI();

private:
    void UpdataSequenceNumber();

public slots:
    void slot_FWDiscoverd();

    void slot_FWRemove();

private slots:



private:
    Ui::MainWindow *ui;
    MasterControl *m_pMasterControl;

    QList<ushort> m_listSequenceNumber;
    ushort m_SelectSequenceNumber;
};

#endif // MAINWINDOW_H
