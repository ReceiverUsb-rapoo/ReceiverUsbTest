#include "starttest_msgqueue.h"

StartTest_MsgQueue::StartTest_MsgQueue()
{
    m_pDeviceOperator = m_oDeviceOperatorInstanceGetter.GetInstance();
}

void StartTest_MsgQueue::PushBack(const ushort &us_SequenceNumber)
{
    m_Queue.push_back(us_SequenceNumber);

    if(!CheckQueueWorkState()){
        StartOneTest();
    }
}

void StartTest_MsgQueue::PushFront(const ushort &us_SequenceNumber)
{
    m_Queue.push_front(us_SequenceNumber);

    if(!CheckQueueWorkState()){
        StartOneTest();
    }
}

void StartTest_MsgQueue::CompleteOneTest()
{
    m_bTesting = false;

    StartOneTest();
}

bool StartTest_MsgQueue::GetTaskState()
{
    if(m_Queue.count() != 0)
        return true;
    else
        return false;
}

bool StartTest_MsgQueue::CheckQueueWorkState()
{
    if(m_bTesting)
        return true;
    else
        return false;
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
