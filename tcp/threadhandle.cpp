#include "threadhandle.h"
#include <QAbstractEventDispatcher>

ThreadHandle::ThreadHandle()
{
    b_InitFist = false;
}

//停止所有线程，并释放资源
ThreadHandle::~ThreadHandle()
{
    QThread * p_Thread;
    for (auto it = m_hashThreadSize.begin(); it != m_hashThreadSize.end(); ++it){
        p_Thread = it.key();
        p_Thread->exit();
        p_Thread->wait(3000);
        delete p_Thread;
        p_Thread= NULL;
    }
}

ThreadHandle & ThreadHandle::getClass()
{
    static ThreadHandle o_ThreadHande;
    return o_ThreadHande;
}

QThread *ThreadHandle::GetThread()
{
    if (!b_InitFist){
        InitThreadType(THREADSIZE, 10);
    }
    if (m_Type == THREADSIZE)
        return FindThreadSize();
    else
        return FindHandleSize();
}

void ThreadHandle::RemoveThread(QThread * p_Thread)
{
    auto t = m_hashThreadSize.find(p_Thread);
    if (t != m_hashThreadSize.end()){
        t.value() --;
        if (m_Type == HANDLESIZE && t.value() == 0 && m_hashThreadSize.size() > 1){
            m_hashThreadSize.remove(p_Thread);
            p_Thread->exit();
            p_Thread->wait(3000);
            delete p_Thread;
            p_Thread= NULL;
        }
    }
}

void ThreadHandle::InitThreadType(ThreadType Type, unsigned int un_max)
{
    if (!b_InitFist)
    {
        this->m_Type = Type;
        this->m_unSize = un_max;
        if (this->m_unSize == 0){
            if(Type == THREADSIZE)
                this->m_unSize = 10;
            else
                this->m_unSize = 1000;
        }

        if (Type == THREADSIZE)
            InitThreadSize();
        else{
            QThread * p_Thread = new QThread;
//            p_Thread->setEventDispatcher(new EventDispatcherLibEv());

            m_hashThreadSize.insert(p_Thread,0);
            p_Thread->start();
        }
    }
    b_InitFist = true;
}

//建立好线程并启动，
void ThreadHandle::InitThreadSize()
{
    QThread * p_Thread;
    for (unsigned int i = 0; i < m_unSize;++i){
        p_Thread = new QThread;
//        p_Thread->setEventDispatcher(new EventDispatcherLibEv());

        m_hashThreadSize.insert(p_Thread,0);
        p_Thread->start();
    }
}

//查找到线程里的连接数小于最大值就返回查找到的，找不到就新建一个线程
QThread * ThreadHandle::FindHandleSize()
{
    for(auto it  = m_hashThreadSize.begin();it != m_hashThreadSize.end() ;++it){
        if (it.value() < m_unSize){
            it.value() ++;
            return it.key();
        }
    }
    QThread * p_Thread = new QThread;
//    p_Thread->setEventDispatcher(new EventDispatcherLibEv());

    m_hashThreadSize.insert(p_Thread,1);
    p_Thread->start();
    return p_Thread;
}

//遍历查找所有线程中连接数最小的那个，返回
QThread *ThreadHandle::FindThreadSize()
{
    auto it = m_hashThreadSize.begin();
    auto ite = m_hashThreadSize.begin();
    for(++it ; it != m_hashThreadSize.end(); ++it){
        if (it.value() < ite.value()){
            ite = it;
        }
    }
    ite.value() ++;
    return ite.key();
}

//仅仅清空计数，线程不释放
void ThreadHandle::Clear()
{
    for(auto it  = m_hashThreadSize.begin();it != m_hashThreadSize.end() ;++it){
        it.value()  = 0;
    }
}
