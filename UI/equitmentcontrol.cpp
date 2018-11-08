#include "equitmentcontrol.h"
#include "ui_equitmentcontrol.h"

EquitmentControl::EquitmentControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EquitmentControl)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

}

EquitmentControl::~EquitmentControl()
{
    delete ui;
}
