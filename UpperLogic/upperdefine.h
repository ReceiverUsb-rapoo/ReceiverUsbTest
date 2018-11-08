#ifndef UPPERDEFINE_H
#define UPPERDEFINE_H
#include <QString>

struct STRUCT_COMINFO{
    QString str_PortName;
    uint un_Pid;
    uint un_Vid;

    STRUCT_COMINFO(){
        str_PortName = "";
        un_Pid = 0;
        un_Vid = 0;
    }
};

enum EMUN_TESTDATAPOINT{
    LAST = 0,
    CURRENT,
};

struct STRUCT_COMMANDANDDATA{
    ushort us_SequenceNumber;
    uchar uc_Command;
    char DATA[254];
    uint un_DataLength;

    STRUCT_COMMANDANDDATA(){
        us_SequenceNumber = 0;
        uc_Command = 0;
        un_DataLength = 0;

        memset(DATA, 0, 254);
    }
};

enum ENUM_RESULTTYPE{
    ENUM_RESULT = 0x00,
    OPENDEVICE_RESULT,
    SENDCMD_RESULT,
    POWERTEST_REULST
};

struct STRUCT_RESULTCROUT{
    uint un_Sum;
    uint un_RetestCount;

    STRUCT_RESULTCROUT(){
        un_Sum  =  0;
        un_RetestCount = 0;
    }
};


#endif // UPPERDEFINE_H
