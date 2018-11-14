#ifndef ROBOT_DEFINE_H
#define ROBOT_DEFINE_H
#include <QString>

const QByteArray CatchRobot_ACK = "A";

const QString CatchRobot_Put = "A";         //control
const QString CatchRobot_Put_Ok = "B";      //robot
const QString CatchRobot_Get = "C";         //control
const QString CatchRobot_Get_OK = "D";      //robot

const QString SupplementRobot_Request = "R";

enum ENUM_FWSTATION{
    Box_1,
    Box_2
};



#endif // ROBOT_DEFINE_H
