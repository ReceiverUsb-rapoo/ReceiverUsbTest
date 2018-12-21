#ifndef TCPSERVERINSTANCEGETTER_H
#define TCPSERVERINSTANCEGETTER_H
#include "tcpserver.h"
//多线程TCP服务器管理类

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
