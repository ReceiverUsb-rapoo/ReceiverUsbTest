#ifndef TCPSERVERINSTANCEGETTER_H
#define TCPSERVERINSTANCEGETTER_H
#include "tcpserver.h"

class TcpServerInstanceGetter
{
public:
    TcpServerInstanceGetter();
    ~TcpServerInstanceGetter();

public:
    TcpServer *GetInstance();

private:
    static TcpServer *m_pTcpServerInstance;
    static uint m_unTcpServerReference;
};

#endif // TCPSERVERINSTANCEGETTER_H
