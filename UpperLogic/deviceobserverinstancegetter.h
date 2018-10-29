#ifndef DEVICEOBSERVERINSTANCEGETTER_H
#define DEVICEOBSERVERINSTANCEGETTER_H
#include "deviceobserver.h"

class DeviceObserverInstanceGetter
{
public:
    DeviceObserverInstanceGetter();    
    ~DeviceObserverInstanceGetter();

public:
    DeviceObserver * GetInstance();

private:
    static DeviceObserver *m_pDeviceObserverInstance;
    static uint m_unDeviceObserverReference;
};

#endif // DEVICEOBSERVERINSTANCEGETTER_H
