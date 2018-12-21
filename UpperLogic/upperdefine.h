#ifndef UPPERDEFINE_H
#define UPPERDEFINE_H
#include <QString>

//com端口信息
struct STRUCT_COMINFO{
    QString str_PortName;   //端口名称 
    uint un_Pid;            //PID
    uint un_Vid;            //VID

    STRUCT_COMINFO(){
        str_PortName = "";
        un_Pid = 0;
        un_Vid = 0;
    }
};

//测试数据指向时间状态
enum EMUN_TESTDATAPOINT{
    LAST = 0,   //上次
    CURRENT,    //当前
};

//cmd 和 数据
struct STRUCT_COMMANDANDDATA{
    ushort us_SequenceNumber;   //序号
    uchar uc_Command;           //命令
    char DATA[254];             //数据
    uint un_DataLength;         //数据长度 

    STRUCT_COMMANDANDDATA(){
        us_SequenceNumber = 0;
        uc_Command = 0;
        un_DataLength = 0;

        memset(DATA, 0, 254);
    }
};

//测试结果类型
enum ENUM_RESULTTYPE{
    ENUM_RESULT = 0x00,     //枚举结果
    OPENDEVICE_RESULT,      //打开设备结果 
    SENDCMD_RESULT,         //发送cmd到usb指令
    POWERTEST_REULST        //功率测试结果 
};

//测试结果 计数
struct STRUCT_RESULTCROUT{
    uint un_Sum;            //总数
    uint un_RetestCount;    //重测总数

    STRUCT_RESULTCROUT(){
        un_Sum  =  0;
        un_RetestCount = 0;
    }
};


#endif // UPPERDEFINE_H
