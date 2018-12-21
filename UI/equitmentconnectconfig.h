#ifndef EQUITMENTCONNECTCONFIG_H
#define EQUITMENTCONNECTCONFIG_H

#include <QWidget>

/*
* 保存外围设备配置参数，无其作用
*/

namespace Ui {
class EquitmentConnectConfig;
}

class EquitmentConnectConfig : public QWidget
{
    Q_OBJECT

public:
    explicit EquitmentConnectConfig(QWidget *parent = 0);
    ~EquitmentConnectConfig();

private:
    //初始化 显示配置参数
    void InitEquitmentConfig();
    //显示配置参数
    void ShowConfig();
    //保存配置参数
    void SaveConfig();

private:
    Ui::EquitmentConnectConfig *ui;
};

#endif // EQUITMENTCONNECTCONFIG_H
