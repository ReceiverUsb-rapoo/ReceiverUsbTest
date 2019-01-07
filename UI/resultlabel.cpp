#include "resultlabel.h"

ResultLabel::ResultLabel(QWidget *parent)
    :QLabel(parent)
{
    InitResultLable();
    this->setAlignment(Qt::AlignVCenter);
    this->setAlignment(Qt::AlignHCenter);

    QFont o_QFont;
    o_QFont.setPointSize(9);
    this->setFont(o_QFont);
}

void ResultLabel::SetState(const ENUM_TESTSTATE &TestState)
{
    switch(TestState) {
    case Ready_Test:{
        QString str_Qss = "QWidget{image:url(" + ReadyImagePath + ");}";
        this->setText("");
        this->setStyleSheet(str_Qss);
        break;
    }
    case Run_Test:{
        QString str_Qss = "QWidget{image:url(" + RunImagePath + ");}";
        this->setStyleSheet(str_Qss);
        this->setText("");
        break;
    }
    case Faile_Test:{
        QString str_Qss = "QWidget{image:url(" + FaileImagePath + ");}";
        this->setStyleSheet(str_Qss);
        break;
    }
    case Pass_Test:{
        QString str_Qss = "QWidget{image:url(" + PassImagePath + ");}";
        this->setStyleSheet(str_Qss);
        break;
    }
    default:
        break;
    }
}

void ResultLabel::InitResultLable()
{
    QString str_Qss = "QWidget{image:url(" + ReadyImagePath + ");}";
    this->setStyleSheet(str_Qss);
}
