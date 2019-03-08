#include "powertestinstancegetter.h"
#include <QDebug>
#include <QMutexLocker>

PowerTest *PowerTestInstanceGetter::m_pPowerTestInstance = NULL;
uint PowerTestInstanceGetter::m_unPowerTestReference = 0;

PowerTestInstanceGetter::PowerTestInstanceGetter()
{
    QMutexLocker o_Locker(&m_oQMutex);

    if(m_pPowerTestInstance == NULL){
        m_pPowerTestInstance = new PowerTest;
    }

    m_unPowerTestReference ++;
}

PowerTestInstanceGetter::~PowerTestInstanceGetter()
{
    QMutexLocker o_Locker(&m_oQMutex);

    m_unPowerTestReference --;
    if(m_unPowerTestReference == 0){
        if(m_pPowerTestInstance != NULL){
            qDebug()<<"PowerTestInstanceGetter";
            delete m_pPowerTestInstance;
            m_pPowerTestInstance = NULL;
        }
    }
}

PowerTest *PowerTestInstanceGetter::GetInstance()
{
    return m_pPowerTestInstance;
}
