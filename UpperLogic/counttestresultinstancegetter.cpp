#include "counttestresultinstancegetter.h"
CountTestData *CountTestResultInstanceGetter::m_pCountTestDataInstance = NULL;
uint CountTestResultInstanceGetter::m_unCountTestDataReference = 0;

CountTestResultInstanceGetter::CountTestResultInstanceGetter()
{
    if(m_pCountTestDataInstance == NULL){
        m_pCountTestDataInstance = new CountTestData;
    }

    m_unCountTestDataReference ++;
}

CountTestResultInstanceGetter::~CountTestResultInstanceGetter()
{
    m_unCountTestDataReference --;
    if(m_unCountTestDataReference == 0){
        if(m_pCountTestDataInstance != NULL){
            delete m_pCountTestDataInstance;
            m_pCountTestDataInstance = NULL;
        }
    }
}

CountTestData *CountTestResultInstanceGetter::GetInstance()
{
    return m_pCountTestDataInstance;
}
