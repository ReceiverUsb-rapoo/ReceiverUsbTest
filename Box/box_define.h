#ifndef BOX_DEFINE_H
#define BOX_DEFINE_H
#include  <QString>

enum BOX_OPERATOR{
    OPENBOX,
    OPENBOX_OK,
    CLOSEBOX,
    CLOSEBOX_OK,
    ERRORBOX
};

const QByteArray OpenCmd = "open";
const QByteArray CloseCmd = "close";

const QByteArray Open_ACK_Ok = "open/ok";
const QByteArray Close_ACK_OK = "close/ok";

const QByteArray Open_ACK_Faile = "open/ng";
const QByteArray Close_ACK_Faile = "close/ng";


#endif // BOX_DEFINE_H
