#ifndef RESULTLABEL_H
#define RESULTLABEL_H

#include <QLabel>
#include <QObject>
#include "ui_define.h"

/*
* 单独结果显示UI
* 继承qlabel， 用于图片显示 
*/

class ResultLabel : public QLabel
{
public:
    explicit ResultLabel(QWidget *parent = NULL);
    //设置状态，显示状态图片
    void SetState(const ENUM_TESTSTATE &TestState);

private:
    //初始化 
    void InitResultLable();

};

#endif // RESULTLABEL_H
