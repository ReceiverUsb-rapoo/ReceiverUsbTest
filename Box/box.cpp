#include "box.h"
#include <QDebug>

Box::Box(QObject *parent)
    :QObject(parent)
{
    m_strIP = "";
    m_nCountResendOpen = 0;
    m_nCountResendClose = 0;
    m_usSequenceNumber = 0;

    InitBox();
}

Box::~Box()
{

}

bool Box::SetIP(const QString &str_IP)
{
    m_strIP = str_IP;
    return true;
}

bool Box::GetIP(QString &str_IP)
{
    str_IP = m_strIP;
    return true;
}

bool Box::SetSequenceNumber(const ushort &us_SequenceNumber)
{
    m_usSequenceNumber = us_SequenceNumber;
    return true;
}

bool Box::GetSequenceNumber(ushort &us_SequenceNumber)
{
    us_SequenceNumber = m_usSequenceNumber;
    return true;
}

void Box::OpenBox()
{
    if(m_strIP == ""){
        return;
    }

    emit sig_SendData(OpenCmd,
                      m_strIP);
}

void Box::CloseBox()
{
    if(m_strIP == ""){
        return;
    }

    emit sig_SendData(CloseCmd,
                      m_strIP);
}

void Box::InitBox()
{
    m_pTcpServer = m_oTcpServerInstanceGetter.GetInstance();

    connect(this, &Box::sig_SendData,
            m_pTcpServer, &TcpServer::sig_SentData);
    connect(m_pTcpServer, SIGNAL(sig_DataRecieved(STRUCT_TCPDATA)),
            this, SLOT(slot_BoxReceiveData(STRUCT_TCPDATA)));
}

bool Box::TntervalTimeReceive(const QByteArray &byte_Data)
{
    if(m_mapCmd_Interval.contains(byte_Data)){
        QDateTime o_QDateTime = QDateTime::fromString(m_mapCmd_Interval.value(byte_Data),
                                                      "yyyy.MM.dd hh.mm.ss.zzz");
        if(o_QDateTime.msecsTo(QDateTime::currentDateTime()) > 500){
            QString str_Time;
            str_Time = QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm.ss.zzz");
            m_mapCmd_Interval.insert(byte_Data, str_Time);
            return true;
        }
        else{
            return false;
        }
    }
    else{
        QString str_Time;
        str_Time = QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm.ss.zzz");
        m_mapCmd_Interval.insert(byte_Data, str_Time);
    }

    return true;
}

void Box::slot_BoxReceiveData(STRUCT_TCPDATA struct_TcpData)
{
    if(struct_TcpData.str_IP != m_strIP){
        return;
    }

    if(!TntervalTimeReceive(struct_TcpData.byte_Data))
        return;

    qDebug()<<"BoxReceiveData"<<struct_TcpData.str_IP<<struct_TcpData.byte_Data;

    if(struct_TcpData.byte_Data == Open_ACK_Ok){
        m_nCountResendOpen = 0;
        emit sig_BoxOperator(m_usSequenceNumber, OPENBOX_OK);
    }
    else if(struct_TcpData.byte_Data == Close_ACK_OK){
        m_nCountResendClose = 0;
        emit sig_BoxOperator(m_usSequenceNumber, CLOSEBOX_OK);
    }
    else if(struct_TcpData.byte_Data == Open_ACK_Faile){
        if(m_nCountResendOpen != 3){
            OpenBox();
            m_nCountResendOpen ++;
        }
        else{
            m_nCountResendOpen = 0;
            emit sig_BoxOperator(m_usSequenceNumber, ERRORBOX);
        }
    }
    else if(struct_TcpData.byte_Data == Close_ACK_Faile){
        if(m_nCountResendClose != 3){
            CloseBox();
            m_nCountResendClose ++;
        }
        else{
            m_nCountResendClose = 0;
            emit sig_BoxOperator(m_usSequenceNumber, ERRORBOX);
        }
    }
}

