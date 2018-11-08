#include "tcpserverinstancegetter.h"
TcpServer *TcpServerInstanceGetter::m_pTcpServerInstance = NULL;
uint TcpServerInstanceGetter::m_unTcpServerReference = 0;

TcpServerInstanceGetter::TcpServerInstanceGetter()
{
    if(m_pTcpServerInstance == NULL){
        m_pTcpServerInstance = new TcpServer;
    }

    m_unTcpServerReference ++;
}

TcpServerInstanceGetter::~TcpServerInstanceGetter()
{
    m_unTcpServerReference --;
    if(m_unTcpServerReference == 0){
        if(m_pTcpServerInstance != NULL){
            delete m_pTcpServerInstance;
            m_pTcpServerInstance = NULL;
        }
    }
}

TcpServer *TcpServerInstanceGetter::GetInstance()
{
    return m_pTcpServerInstance;
}
