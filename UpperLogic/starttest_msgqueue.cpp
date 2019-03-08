#include "starttest_msgqueue.h"
#include <QDebug>

StartTest_MsgQueue::StartTest_MsgQueue(QObject *parent)
    :QObject(parent)
{
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();
    m_bTesting = false;

    m_pQTimer = new QTimer(this);
    connect(m_pQTimer, SIGNAL(timeout()),
            this, SLOT(slot_TimeOut()));
    m_pQTimer->start(500);
}

StartTest_MsgQueue::~StartTest_MsgQueue()
{

}

//bool StartTest_MsgQueue::SetFunPoint(const bool &(*p_Fun)())
//{
//    m_pFun = p_Fun;
//}

void StartTest_MsgQueue::PushBack(const ushort &us_SequenceNumber)
{
    m_Queue.push_back(us_SequenceNumber);
}

void StartTest_MsgQueue::PushFront(const ushort &us_SequenceNumber)
{
    m_Queue.push_front(us_SequenceNumber);
}

void StartTest_MsgQueue::CompleteOneTest()
{
    m_bTesting = false;
}

bool StartTest_MsgQueue::GetTaskState()
{
    if(m_Queue.count() != 0)
        return true;
    else
        return false;
}

bool StartTest_MsgQueue::ClearAndResetting()
{
    m_bTesting = false;
    m_Queue.clear();
    return true;
}

bool StartTest_MsgQueue::CheckQueueWorkState()
{
    return m_bTesting;
}

void StartTest_MsgQueue::StartOneTest()
{
    if(m_Queue.isEmpty()){
        return;
    }

    if(m_bTesting){
        return;
    }

    m_pDeviceOperator->StartOneTest(m_Queue.takeFirst());
    m_bTesting = true;
}

void StartTest_MsgQueue::slot_TimeOut()
{
    StartOneTest();
}
