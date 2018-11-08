#include "catchrobot.h"

CatchRobot::CatchRobot(QObject *parent)
    :QObject(parent)
{

}

CatchRobot::~CatchRobot()
{

}

bool CatchRobot::SetIP(const QString &str_IP)
{
    m_strIP = str_IP;
    return true;
}

bool CatchRobot::GetIP(QString &str_IP)
{
    str_IP = m_strIP;
    return true;
}

void CatchRobot::SendCatchData(const QString &str_Data)
{
    QByteArray byte_Data = str_Data.toLatin1();

    emit sig_SendData(byte_Data,
                      m_strIP);
}

void CatchRobot::InitCatchRobot()
{
    m_pTcpServer = m_oTcpServerInstanceGetter.GetInstance();

    connect(this, &CatchRobot::sig_SendData,
            m_pTcpServer, &TcpServer::sig_SentData);
    connect(m_pTcpServer, SIGNAL(sig_DataRecieved(STRUCT_TCPDATA)),
            this, SLOT(slot_CatchRobotReceiveData(STRUCT_TCPDATA)));
}

void CatchRobot::slot_CatchRobotReceiveData(STRUCT_TCPDATA struct_TcpData)
{
    if(struct_TcpData.str_IP != m_strIP){
        return;
    }
}
