#ifndef STARTTEST_MSGQUEUE_H
#define STARTTEST_MSGQUEUE_H
#include <QObject>
#include <QThread>
#include "UpperLogic/deviceoperatorinstancegetter.h"

class StartTest_MsgQueue/* : QThread*/
{
public:
    StartTest_MsgQueue();

    void PushBack(const ushort &us_SequenceNumber);

    void PushFront(const ushort &us_SequenceNumber);

    void CompleteOneTest();

    bool GetTaskState();

private:
    bool CheckQueueWorkState();

    void StartOneTest();

//protected:
//    void run();

private:
    DeviceOperatorInstanceGetter m_oDeviceOperatorInstanceGetter;
    DeviceOperator *m_pDeviceOperator;

    QQueue<ushort> m_Queue;
    bool m_bTesting;
};

#endif // STARTTEST_MSGQUEUE_H
