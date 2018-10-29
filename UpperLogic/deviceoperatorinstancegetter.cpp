#include "deviceoperatorinstancegetter.h"
DeviceOperator *DeviceOperatorInstanceGetter::m_pDeviceOperatorInstance = NULL;
uint DeviceOperatorInstanceGetter::m_unDeviceOperatorReference = 0;

DeviceOperatorInstanceGetter::DeviceOperatorInstanceGetter()
{
    if(m_pDeviceOperatorInstance == NULL){
        m_pDeviceOperatorInstance = new DeviceOperator;
    }

    m_unDeviceOperatorReference ++;
}

DeviceOperatorInstanceGetter::~DeviceOperatorInstanceGetter()
{
    m_unDeviceOperatorReference --;
    if(m_unDeviceOperatorReference == 0){
        if(m_pDeviceOperatorInstance != NULL){
            delete m_pDeviceOperatorInstance;
            m_pDeviceOperatorInstance = NULL;
        }
    }
}

DeviceOperator *DeviceOperatorInstanceGetter::GetInstance()
{
    return m_pDeviceOperatorInstance;
}


