#include "box.h"

Box::Box(QObject *parent)
    :QObject(parent)
{
    m_strIP = "192.1.1.1";
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
    emit sig_SendData(OpenCmd,
                      m_strIP);
}

void Box::CloseBox()
{
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

void Box::slot_BoxReceiveData(STRUCT_TCPDATA struct_TcpData)
{
    if(struct_TcpData.str_IP != m_strIP){
        return;
    }

    if(struct_TcpData.byte_Data == Open_ACK_Ok){
        m_nCountResendOpen = 0;
        emit sig_BoxOperator(m_usSequenceNumber, OPENBOX);
    }
    else if(struct_TcpData.byte_Data == Close_ACK_OK){
        m_nCountResendClose = 0;
        emit sig_BoxOperator(m_usSequenceNumber, CLOSEBOX);
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

