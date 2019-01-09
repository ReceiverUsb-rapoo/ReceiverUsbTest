#ifndef BOX_DEFINE_H
#define BOX_DEFINE_H
#include <QObject>

//箱子动作集和
enum BOX_OPERATOR{
    OPENBOX,
    OPENBOX_OK,
    CLOSEBOX,
    CLOSEBOX_OK,
    ERRORBOX
};

//箱子动作 cmd
const QByteArray OpenCmd = "open";
const QByteArray CloseCmd = "close";

const QByteArray Open_ACK_Ok = "OK\r\n";
const QByteArray Close_ACK_OK = "READY\r\n";

const QByteArray Open_ACK_Faile = "OPEN/NG\r\n";
const QByteArray Close_ACK_Faile = "CLOSE/NG\r\n";


#endif // BOX_DEFINE_H
