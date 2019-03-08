#include "enumpointtable.h"
#include "ui_enumpointtable.h"
#include <QDebug>

const QString COLORNAME[2] = {"#00c9b0", "#0195ca"};
const int OneGroupUsbNumber_PonintTable = 20;
const QString RegExp = "^(([0-9]|(\\d{7})))$";

EnumPointTable::EnumPointTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnumPointTable)
{
    ui->setupUi(this);
    InitPointTable();
}

EnumPointTable::~EnumPointTable()
{
    delete ui;
}

bool EnumPointTable::SetLEValue(const QList<QString> &list_LEValue)
{
    if(list_LEValue.count() != OneGroupUsbNumber_PonintTable){
        return false;
    }

    for(int i = 0; i < OneGroupUsbNumber_PonintTable; i++){
//        qDebug()<<m_listLE.at(i)<<" "<<list_LEValue.at(i);

        m_listLE.at(i)->setText(list_LEValue.at(i));
    }

    SetLEColor();

    return true;
}

bool EnumPointTable::GetLEValue(QList<QString> &list_LEValue)
{
    for(int i = 0; i < OneGroupUsbNumber_PonintTable; i++){
        list_LEValue.append(m_listLE.at(i)->text());
    }

    return true;
}

bool EnumPointTable::SetRepeatData(const bool &b_Repeat)
{
    m_bRepeatData = b_Repeat;
    return true;
}

bool EnumPointTable::ClearData()
{
    for(int i = 0; i < OneGroupUsbNumber_PonintTable; i++){
        m_listLE.at(i)->setText("0");
    }

    return true;
}

void EnumPointTable::InitPointTable()
{
    m_bRepeatData = false;
    m_nGroupNumber = 0;

    m_listLE<<ui->lineEdit_1<<ui->lineEdit_2<<ui->lineEdit_3<<ui->lineEdit_4
            <<ui->lineEdit_5<<ui->lineEdit_6<<ui->lineEdit_7<<ui->lineEdit_8
            <<ui->lineEdit_9<<ui->lineEdit_10<<ui->lineEdit_11<<ui->lineEdit_12
            <<ui->lineEdit_13<<ui->lineEdit_14<<ui->lineEdit_15<<ui->lineEdit_16
            <<ui->lineEdit_17<<ui->lineEdit_18<<ui->lineEdit_19<<ui->lineEdit_20;

    QRegExp o_QRegExp(RegExp);
    QValidator *p_Validator = new QRegExpValidator(o_QRegExp, this);

    for(int i = 0; i < OneGroupUsbNumber_PonintTable; i++){
        connect(m_listLE.at(i), SIGNAL(editingFinished()),
                this, SLOT(slot_LEValueChange()));

        m_listLE.at(i)->setValidator(p_Validator);
        m_listLE.at(i)->setText("0");
    }

//    delete p_Validator;
//    p_Validator = NULL;

    SetLEColor();
}

void EnumPointTable::SetLEColor()
{
    for(int i = 0; i < OneGroupUsbNumber_PonintTable; i++){
        if(m_listLE.at(i)->text() == "0"){
            m_listLE.at(i)->setStyleSheet("background-color:" + COLORNAME[0]);
        }
        else{
            m_listLE.at(i)->setStyleSheet("background-color:" + COLORNAME[1]);
        }
    }
}

void EnumPointTable::slot_LEValueChange()
{
    QLineEdit *p_LE = qobject_cast<QLineEdit *>(sender());

    if(m_bRepeatData == false){
        for(int i = 0; i < OneGroupUsbNumber_PonintTable; i++){
            if(p_LE->text() == m_listLE.at(i)->text() &&
                    p_LE != m_listLE.at(i)){
                p_LE->setText("0");
            }
        }
    }
    else{
        //nothing to do
    }

    SetLEColor();
}
