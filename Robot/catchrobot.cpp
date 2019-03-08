#include "catchrobot.h"

CatchRobot::CatchRobot(QObject *parent)
    :QObject(parent)
{
    m_strIP = "";
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
    if(m_strIP == ""){
        return;
    }

    QString str_Action = str_RobotAction +
                         QString::number(us_FWStation) +
                         "\r\n";

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

    if(struct_TcpData.byte_Data.count() != 4){
        return;
    }

    ushort us_FWStation = (ushort)struct_TcpData.byte_Data.at(1) - 48;
    QString str_RobotAction = (QString)struct_TcpData.byte_Data.at(0);

//    qDebug()<<"slot_CatchRobotReceiveData";

    emit sig_GetAction(us_FWStation, str_RobotAction);
}
