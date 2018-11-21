#ifndef UI_DEFINE_H
#define UI_DEFINE_H
#include <QString>

const QString ReadyImagePath = ":/Image/Receiver_Ready.png";
const QString RunImagePath = ":/Image/Receiver_Run.png";
const QString FaileImagePath = ":/Image/Receiver_Faile.png";
const QString PassImagePath = ":/Image/Receiver_Pass.png";

enum ENUM_TESTSTATE{
    Ready_Test,
    Run_Test,
    Faile_Test,
    Pass_Test
};


#endif // UI_DEFINE_H
