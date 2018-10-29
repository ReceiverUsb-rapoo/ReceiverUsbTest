#include "pointtable.h"
#include "ui_pointtable.h"
#include <QRegExp>

const QString COLORNAME[2] = {"#00c9b0", "#0195ca"};
const int LENumber = 32;
const QString RegExp = "^((\\d|([1-9]\\d)))$";


PointTable::PointTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointTable)
{
    ui->setupUi(this);

    InitPointTable();
}

PointTable::~PointTable()
{
    delete ui;
}

bool PointTable::SetLEValue(const QList<QString> &list_LEValue)
{
    if(list_LEValue.count() != LENumber){
        return false;
    }

    for(int i = 0; i < LENumber; i++){
        m_listLE.at(i)->setText(list_LEValue.at(i));
    }

    SetLEColor();

    return true;
}

bool PointTable::GetLEValue(QList<QString> &list_LEValue)
{
    for(int i = 0; i < LENumber; i++){
        list_LEValue.append(m_listLE.at(i)->text());
    }

    return true;
}

bool PointTable::SetRepeatData(const bool &b_Repeat)
{
    m_bRepeatData = b_Repeat;
    return true;
}

bool PointTable::ClearData()
{
    for(int i = 0; i < LENumber; i++){
        m_listLE.at(i)->setText("0");
    }

    return true;
}

void PointTable::InitPointTable()
{
    m_bRepeatData = false;
    m_nGroupNumber = 0;

    m_listLE<<ui->lineEdit_1<<ui->lineEdit_2<<ui->lineEdit_3<<ui->lineEdit_4
            <<ui->lineEdit_5<<ui->lineEdit_6<<ui->lineEdit_7<<ui->lineEdit_8
            <<ui->lineEdit_9<<ui->lineEdit_10<<ui->lineEdit_11<<ui->lineEdit_12
            <<ui->lineEdit_13<<ui->lineEdit_14<<ui->lineEdit_15<<ui->lineEdit_16
            <<ui->lineEdit_17<<ui->lineEdit_18<<ui->lineEdit_19<<ui->lineEdit_20
            <<ui->lineEdit_21<<ui->lineEdit_22<<ui->lineEdit_23<<ui->lineEdit_24
            <<ui->lineEdit_25<<ui->lineEdit_26<<ui->lineEdit_27<<ui->lineEdit_28
            <<ui->lineEdit_29<<ui->lineEdit_30<<ui->lineEdit_31<<ui->lineEdit_32;

    QRegExp o_QRegExp(RegExp);
    QValidator *p_Validator = new QRegExpValidator(o_QRegExp, this);

    for(int i = 0; i < LENumber; i++){
        connect(m_listLE.at(i), SIGNAL(editingFinished()),
                this, SLOT(slot_LEValueChange()));

        m_listLE.at(i)->setValidator(p_Validator);
        m_listLE.at(i)->setText("0");
    }

//    delete p_Validator;
//    p_Validator = NULL;

    SetLEColor();
}

void PointTable::SetLEColor()
{
    for(int i = 0; i < LENumber; i++){
        if(m_listLE.at(i)->text() == "0"){
            m_listLE.at(i)->setStyleSheet("background-color:" + COLORNAME[0]);
        }
        else{
            m_listLE.at(i)->setStyleSheet("background-color:" + COLORNAME[1]);
        }
    }
}

void PointTable::slot_LEValueChange()
{
    QLineEdit *p_LE = qobject_cast<QLineEdit *>(sender());

    if(m_bRepeatData == false){
        for(int i = 0; i < LENumber; i++){
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
