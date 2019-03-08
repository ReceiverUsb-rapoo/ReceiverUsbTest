#ifndef THREADHANDLE_H
#define THREADHANDLE_H
#include <QThread>
#include <QQueue>
#include <QList>
#include <QHash>

//线程管理类，类似于一个线程池，单例类
//两种初始化方式，一种是每个线程处理的连接数，一个是一共多少个线程

enum ThreadType
{
    THREADSIZE,     //固定线程数
    HANDLESIZE      //固定每个线程处理多少连接
};

class ThreadHandle
{
private:
    ThreadHandle();

public:
    ~ThreadHandle();

    //返回对象引用，是单例类
    static ThreadHandle & getClass();
    //取出应该移入的线程
    QThread * GetThread();
    //初始化线程管理的方式
    void InitThreadType(ThreadType Type = HANDLESIZE, unsigned int un_max = 1000);
    //连接断开，线程计数减一
    void RemoveThread(QThread *p_Thread);
    //清空计数
    void Clear();

protected:
    //新建固定线程和启动
    void InitThreadSize();
    //固定线程数的查找
    QThread * FindThreadSize();
    //固定连接数查找
    QThread * FindHandleSize();
private:
    ThreadType m_Type;                                  //线程类型
    unsigned int m_unSize;                              //最大值
    QHash<QThread *, unsigned int> m_hashThreadSize;    //保存每个线程的数目
    bool b_InitFist;                                    //是否是第一次初始化，只允许初始化一次。

};

#endif // THREADHANDLE_H
