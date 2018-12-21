#ifndef POWERTESTINSTANCEGETTER_H
#define POWERTESTINSTANCEGETTER_H
#include "powertest.h"

//功率测试类管理类，主要用于并发 单例类处理 

class PowerTestInstanceGetter
{
public:
    PowerTestInstanceGetter();
    ~PowerTestInstanceGetter();

public:
    PowerTest * GetInstance();

private:
    static PowerTest *m_pPowerTestInstance;
    static uint m_unPowerTestReference;
};

#endif // POWERTESTINSTANCEGETTER_H
