#ifndef DEVICEOPERATORINSTANCEGETTER_H
#define DEVICEOPERATORINSTANCEGETTER_H
#include "deviceoperator.h"

/*
* 操作者类管理类
*/

class DeviceOperatorInstanceGetter
{
public:
    DeviceOperatorInstanceGetter();
    ~DeviceOperatorInstanceGetter();

public:
    DeviceOperator * GetInstance();

private:
    static DeviceOperator *m_pDeviceOperatorInstance;
    static uint m_unDeviceOperatorReference;
};

#endif // DEVICEOPERATORINSTANCEGETTER_H
