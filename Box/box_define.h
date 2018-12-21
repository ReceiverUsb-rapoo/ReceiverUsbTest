#ifndef BOX_DEFINE_H
#define BOX_DEFINE_H
#include  <QString>

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

const QByteArray Open_ACK_Ok = "open/ok";
const QByteArray Close_ACK_OK = "close/ok";

const QByteArray Open_ACK_Faile = "open/ng";
const QByteArray Close_ACK_Faile = "close/ng";


#endif // BOX_DEFINE_H
