#ifndef EQUITMENTCONTROL_H
#define EQUITMENTCONTROL_H

#include <QWidget>

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
    Ui::EquitmentControl *ui;
};

#endif // EQUITMENTCONTROL_H
