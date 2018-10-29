#ifndef EQUITMENTCONNECTCONFIG_H
#define EQUITMENTCONNECTCONFIG_H

#include <QWidget>

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
    void InitEquitmentConfig();

    void ShowConfig();

    void SaveConfig();

private:
    Ui::EquitmentConnectConfig *ui;
};

#endif // EQUITMENTCONNECTCONFIG_H
