#ifndef STARTTEST_MSGQUEUE_H
#define STARTTEST_MSGQUEUE_H
#include <QObject>
#include <QThread>
#include "UpperLogic/deviceoperatorinstancegetter.h"

/*
* 消息队列模式，DUT测试只能一次一次测试。测试推入队列
*/

class StartTest_MsgQueue/* : QThread*/
{
public:
    StartTest_MsgQueue();
    //工作序号-DUT测试，推入列表后端
    void PushBack(const ushort &us_SequenceNumber);
    //工作序号-DUT测试，推入列表前端
    void PushFront(const ushort &us_SequenceNumber);
    //完成一次DUT测试
    void CompleteOneTest();
    //获取消息队列里的任务 (>0 true, =0 false)
    bool GetTaskState();

private:
    //检查队列状态
    bool CheckQueueWorkState();
    //开始一次测试
    void StartOneTest();

//protected:
//    void run();

private:
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;   //操作者类管理类
    DeviceOperator *m_pDeviceOperator;

    QQueue<ushort> m_Queue;     //消息队列
    bool m_bTesting;            //测试任务状态
};

#endif // STARTTEST_MSGQUEUE_H
