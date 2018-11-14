#include "catchrobot.h"

CatchRobot::CatchRobot(QObject *parent)
    :QObject(parent)
{
    InitCatchRobot();
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

void CatchRobot::SendAction(const ushort &us_FWStation,
                            const QString &str_RobotAction)
{
    QString str_Action = str_RobotAction + QString::number(us_FWStation);

    emit sig_SendData(str_Action.toLatin1(),
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

    if(struct_TcpData.byte_Data.count() != 2){
        return;
    }

    ushort us_FWStation = (ushort)struct_TcpData.byte_Data.at(1);
    QString str_RobotAction = (QString)struct_TcpData.byte_Data.at(0);

    emit sig_GetAction(us_FWStation, str_RobotAction);

}
