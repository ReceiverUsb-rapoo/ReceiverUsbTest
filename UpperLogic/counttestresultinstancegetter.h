#ifndef COUNTTESTRESULTINSTANCEGETTER_H
#define COUNTTESTRESULTINSTANCEGETTER_H
#include "counttestdata.h"

class CountTestResultInstanceGetter
{
public:
    CountTestResultInstanceGetter();
    ~CountTestResultInstanceGetter();

public:
    CountTestData *GetInstance();

private:
    static CountTestData *m_pCountTestDataInstance;
    static uint m_unCountTestDataReference;
};

#endif // COUNTTESTRESULTINSTANCEGETTER_H
