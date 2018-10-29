#include "deviceobserverinstancegetter.h"
DeviceObserver *DeviceObserverInstanceGetter::m_pDeviceObserverInstance = NULL;
uint DeviceObserverInstanceGetter::m_unDeviceObserverReference = 0;

DeviceObserverInstanceGetter::DeviceObserverInstanceGetter()
{
    if(m_pDeviceObserverInstance == NULL){
        m_pDeviceObserverInstance = new DeviceObserver;
    }

    m_unDeviceObserverReference ++;
}

DeviceObserverInstanceGetter::~DeviceObserverInstanceGetter()
{
    m_unDeviceObserverReference --;
    if(m_unDeviceObserverReference == 0){
        if(m_pDeviceObserverInstance != NULL){
            delete m_pDeviceObserverInstance;
            m_pDeviceObserverInstance = NULL;
        }
    }
}

DeviceObserver *DeviceObserverInstanceGetter::GetInstance()
{
    return m_pDeviceObserverInstance;
}
