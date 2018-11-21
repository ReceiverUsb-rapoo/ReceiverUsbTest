#ifndef RESULTLABEL_H
#define RESULTLABEL_H

#include "QLabel"
#include "QObject"
#include "ui_define.h"

class ResultLabel : public QLabel
{
public:
    explicit ResultLabel(QWidget *parent = NULL);

    void SetState(const ENUM_TESTSTATE &TestState);

private:
    void InitResultLable();

};

#endif // RESULTLABEL_H
