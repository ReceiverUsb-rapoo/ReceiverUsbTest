#ifndef ROBOT_DEFINE_H
#define ROBOT_DEFINE_H
#include <QString>

//机器人命令 
const QByteArray CatchRobot_ACK = "A";

const QString CatchRobot_Put = "A";         //control   放
const QString CatchRobot_Put_Ok = "B";      //robot     放OK
const QString CatchRobot_Get = "C";         //control   取
const QString CatchRobot_Get_OK = "D";      //robot     取ok
const QString CatchRobot_Working = "W";
const QString CatchRobot_Leaving = "L";

const QString SupplementRobot_Request = "R";    //请求 结果

//箱子位置    
enum ENUM_FWSTATION{
    Box_1,
    Box_2
};

#endif // ROBOT_DEFINE_H
