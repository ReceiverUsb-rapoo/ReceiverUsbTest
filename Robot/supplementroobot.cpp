#include "supplementroobot.h"
#include <QDebug>

SupplementRoobot::SupplementRoobot(QObject *parent)
    :QObject(parent)
{
    m_strIP = "";
    InitSupplementRobot();
}

SupplementRoobot::~SupplementRoobot()
{

}

bool SupplementRoobot::SetIP(const QString &str_IP)
{
    m_strIP = str_IP;
    return true;
}

bool SupplementRoobot::GetIP(QString &str_IP)
{
    str_IP = m_strIP;
    return true;
}

void SupplementRoobot::SendSupplementData(ushort us_FWStation,
                                          const QString &str_Data)
{
    if(m_strIP == ""){
        return;
    }

    Q_UNUSED(us_FWStation);
    QString str_Action = /*SupplementRobot_Request +
                         QString::number(us_FWStation) +
                         "-" +*/
                         str_Data +
                         "\r\n";

    qDebug()<<"SendSupplementData  "<<str_Data;

    emit sig_SendData(str_Action.toLatin1(),
                       m_strIP);
}

void SupplementRoobot::InitSupplementRobot()
{
    m_pTcpServer = m_oTcpServerInstanceGetter.GetInstance();

    connect(this, &SupplementRoobot::sig_SendData,
            m_pTcpServer, &TcpServer::sig_SentData);
    connect(m_pTcpServer, SIGNAL(sig_DataRecieved(STRUCT_TCPDATA)),
            this, SLOT(slot_SupplementReceiveData(STRUCT_TCPDATA)));
}

void SupplementRoobot::slot_SupplementReceiveData(STRUCT_TCPDATA struct_TcpData)
{
    if(struct_TcpData.str_IP != m_strIP){
        return;
    }

    if(struct_TcpData.byte_Data.count() != 4){
        return;
    }

    ushort us_FWStation = (ushort)struct_TcpData.byte_Data.at(1) - 48;
    QString str_RobotAction = (QString)struct_TcpData.byte_Data.at(0);

    qDebug()<<"SupplementReceiveData "<<str_RobotAction;

    emit sig_GetRequest(us_FWStation, str_RobotAction);
}
