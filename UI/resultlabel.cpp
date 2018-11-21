#include "resultlabel.h"

ResultLabel::ResultLabel(QWidget *parent)
    :QLabel(parent)
{
    InitResultLable();
}

void ResultLabel::SetState(const ENUM_TESTSTATE &TestState)
{
    switch(TestState) {
    case Ready_Test:{
        QString str_Qss = "{border-image:url(" + ReadyImagePath + ");}";
        this->setStyleSheet(str_Qss);
        break;
    }
    case Run_Test:{
        QString str_Qss = "{border-image:url(" + RunImagePath + ");}";
        this->setStyleSheet(str_Qss);
        break;
    }
    case Faile_Test:{
        QString str_Qss = "{border-image:url(" + FaileImagePath + ");}";
        this->setStyleSheet(str_Qss);
        break;
    }
    case Pass_Test:{
        QString str_Qss = "{border-image:url(" + PassImagePath + ");}";
        this->setStyleSheet(str_Qss);
        break;
    }
    default:
        break;
    }
}

void ResultLabel::InitResultLable()
{
    QString str_Qss = "{border-image:url(" + ReadyImagePath + ");}";
    this->setStyleSheet(str_Qss);
}
