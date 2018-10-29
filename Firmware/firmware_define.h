#ifndef FIRMWARE_DEFINE_H
#define FIRMWARE_DEFINE_H

#define UNUSED_PARAMETER

#include <QString>

const int MAX_LENGHT            = 2048;
const int UP_HEADER_LENGHT      = 4;
const int DOWN_HEADER_LENGHT    = 4;
const int FRAME_LENGHT          = 2;
const int COMMAND_LENGHT        = 1;
const int CHECKSUM_LENGHT       = 2;
const int FRAMETAIL_LENGHT      = 2;
const int BAGDATA_SHORTEST      = 11;
const int DATA_MAX_LENGHT       = 241;
const char UP_HEADER[5]         = {0x55, 0x50, 0x4C, 0x3A};
const char DOWN_HEADER[5]       = {0x44, 0x4E, 0x4C, 0x3A};
const char FRAMETAIL[3]         = {0x0D, 0x0A};

const int PositionTotal         = 32;
const int ShortTimeOut          = 500;
const int LongTimeOut           = 5000;

enum ENUM_OPENMODEL{
    BYCOM = 0,
    BYPIDVID
};

enum ENUM_TESTTYPE{
    _24GMOUSE       = 0x00,     //2.4G鼠标
    _24GKEYBOARD    = 0x01,     //2.4G键盘
    _24GRECEIVER    = 0x02,     //2.4G接收器

    _WIREDMOUSE     = 0x03,     //有线鼠标
    _WIREDKEYBOARD  = 0x04,     //有线键盘

    _58GMOUSE       = 0x06,     //5.8G鼠标
    _58GKEYBOARD    = 0x07,     //5.8G键盘
    _58GRECEIVER    = 0x08,     //5.8G接收器

    _3MMOUSE        = 0x09,     //三模鼠标
    _3MKEYBOARD     = 0x0a,     //三模键盘
    _3MRECEIVER     = 0x0b,     //三模接收器

    _INTACTRECEIVER = 0x0c,     //成品接收器
};

struct STRUCT_FWCONGIF{
    ushort us_WorkCurrentTime;        //工作电流测试时间(ms), 默认=0
    ushort us_SleepCurrentTime;       //睡眠电流测试时间(ms), 默认=0
    ushort us_LEDTestTime;            //LED测试时间(ms),默认=0
    ushort us_LED2TestTime;           //LED2测试时间(ms),默认=0
    ushort us_IRTestTime;             //红外测试时间(ms),默认=0
    ushort us_LaserTestTime;          //激光测试时间(ms),默认=0
    ushort us_BurnEEPROMTime;         //烧录EEPROM时间(ms)，默认=0
    ushort us_ChargeTextTime;         //充电测试时间(ms), 默认=0
    ushort us_SendTime;               //发射时间(n*500ms),默认=0
    ushort us_ReceiveTime;            //接收时间(n*1000ms),默认=0
    ushort us_PoweredDelay;           //上电延时(ms),默认=1200
    ushort us_ReadRFDelay;            //读取RF延时(ms),默认=5000
    ushort us_SleepCurrentDelay;      //检测睡眠电流延时(ms),默认=4000
    ushort us_DischargeTimeSelect;    //放电时间选择(ms),默认=2000
    ushort us_DUTVoltageSelect;       //DUT供电电压选择(mV),默认=1500
    ushort us_CloseCurrentSet;        //关断电流设置(mA)，默认=0
    uchar uc_IRTestSwitch;            //红外测试开关,默认=0,bit0=1:测红外开；bit1=1测红外关
    uchar uc_LEDTestSwitch;           //LED测试开关,默认=0bit0=1: 测LED; bit1=1：测LED2;
    uchar uc_LaserTestSwitch;         //激光测试开关,默认=0
    uchar uc_FunctionTextSwitch;      //功能测试开关，默认=0,bit=1 表示测试；bit0：晶体频偏；bit1：烧录EEPROM；bit2: 测试充电；
    uchar uc_SelfTestCode;            //自检测结果码(正确0Xaa),默认=0xaa
    uchar uc_RetestTimes;             //重测次数,默认=0
    uchar uc_IRCollectAvgTimes;       //红外采集平均次数(1~32),默认=0
    uchar uc_LEDCollectAvgTimes;      //LED采集平均次数(1~32),默认=0
    uchar uc_LED2CollectAvgTimes;     //LED2 采集平均次数（1~32），默认=0
    uchar uc_CurrentCollectAvgTimes;  //电流采集平均次数(1~32),默认=0
    uchar uc_CrytalCollectAvgTimes;   //测试晶体频偏平均次数(1~20),默认=0
    uchar uc_ReceiveRSSILimit;        //接收RSSI下限,默认=0
    uchar uc_SingleTestCount;         //分组测试单次测量的DUT数量N,默认=4

    //... ADD IN STRUCT_PCCONFIG -- str_DUTFWPositions other ..

    ushort us_SinglePowerDelay;       //功率检测从’测试DUT功率’命令发出后延时检测时间,单位ms,
    ushort us_SinglePowerTime;        //功率检测时单个DUT测试时间,单位ms
    uchar uc_PowerAvgTimes;           //功率检测时检测的ADC平均次数
    uchar uc_PowerChannel;            //功率检测时单个DUT所用的发射频率
    uchar uc_PowerTestSwitch;         //功率检测开关，1：打开功率检测；0：不检测功率

    //... ADD IN STRUCT_PCCONFIG -- str_PowerDUTPositions str_PowerTestGroups..

    ushort us_FirstPowerTestDelay;
    ushort us_EnumTime;               //默认=8000
    uchar uc_EnumTestSwitch;
    uchar uc_TotalByte;
};

struct STRUCT_PCTESTCONFIG{
    ENUM_TESTTYPE TestType;
    uint un_FWPosition;
    ushort us_FWPid;
    ushort us_FWVid;
    ushort us_PoweredDelay;           //上电延时(ms),默认=1200
    ushort us_ReadRFDelay;            //读取RF延时(ms),默认=5000
    ushort us_DischargeTimeSelect;    //放电时间选择(ms),默认=2000
    ushort us_DUTVoltageSelect;       //DUT供电电压选择(mV),默认=1500

    uchar uc_RetestTimes;             //重测次数,默认=0

    uchar uc_SingleTestCount;         //分组测试单次测量的DUT数量N,默认=4
    ushort us_SinglePowerDelay;       //功率检测从’测试DUT功率’命令发出后延时检测时间,单位ms,
    ushort us_SinglePowerTime;        //功率检测时单个DUT测试时间,单位ms
    uchar uc_PowerAvgTimes;           //功率检测时检测的ADC平均次数
    uchar uc_PowerChannel;            //功率检测时单个DUT所用的发射频率
    uchar uc_PowerTestSwitch;         //功率检测开关，1：打开功率检测；0：不检测功率

    ushort us_FirstPowerTestDelay;
    ushort us_EnumTime;               //默认=8000
    uchar uc_EnumTestSwitch;

//    QString str_EnumPositionPort;
    QString str_DUTFWPositions;         //DUT测试的位置(1~32)
    QString str_PowerDUTPositions;      //功率测试的位置(1~32)
    QString str_PowerTestGroups;        //功率测试的组(1~32)
    QString str_RFPowerDBUponLimit;     //RF功率上限(1~32)
    QString str_RFPowerDBLowerLimit;    //RF功率下限(1~32)

    QString str_ProjectName;
    QString str_ProjectVersions;
};

struct STRUCT_FWINFO{
    ushort us_Vid;
    ushort us_Pid;
    uchar uc_Year;
    uchar uc_Month;
    uchar uc_Day;
    uchar uc_Version;
    uchar uc_UUID_1;
    uchar uc_UUID_2;
    uchar uc_UUID_3;
    uchar uc_UUID_4;
    uchar uc_UUID_5;
    uchar uc_UUID_6;
    uchar uc_UUID_7;
    uchar uc_UUID_8;
    uchar uc_UUID_9;
    uchar uc_UUID_10;
    uchar uc_UUID_11;
    uchar uc_UUID_12;
    uchar uc_FixtureNumber;
    uchar uc_InstallStation;

    STRUCT_FWINFO(){
        us_Vid = 0;
        us_Pid = 0;
        uc_Year = 0;
        uc_Month = 0;
        uc_Day = 0;
        uc_Version = 0;
        uc_UUID_1 = 0;
        uc_UUID_2 = 0;
        uc_UUID_3 = 0;
        uc_UUID_4 = 0;
        uc_UUID_5 = 0;
        uc_UUID_6 = 0;
        uc_UUID_7 = 0;
        uc_UUID_8 = 0;
        uc_UUID_9 = 0;
        uc_UUID_10 = 0;
        uc_UUID_11 = 0;
        uc_UUID_12 = 0;
        uc_FixtureNumber = 0;
        uc_InstallStation = 0;
    }

};

struct STRUCT_HANDBAG{
    char c_DUTState;
    char c_TestState;
    char c_TestType;
    char c_SurplusGroup;
    char c_TestingProgress;
};


enum ENUM_PCCOMMAND{
    //comand
    PC_GETFWINFO            = 0x01,
    PC_RESTARTFW            = 0x02,

    PC_INITRDUTTEST         = 0x30,
    PC_EXITDUTTEST          = 0x31,
    PC_STARTONETEST         = 0x32,
    PC_INQUIREMACHINESTATE  = 0x33,

    PC_COMPLETEENUM         = 0x35,
    PC_ENTERDEBUGMODEL      = 0x50,
    PC_EXITDEBUGMODLE       = 0x51,

    PC_STARTWHOLEDUTTEST    = 0x5a,
    PC_ADJUSTPOWER          = 0x5e,
    PC_READPOWERDATA        = 0x5f,
    PC_CLEARPOWERCONFIG     = 0x60,

    PC_TESTPOWERSELFTEST    = 0x61,
    PC_TESTENUM             = 0x69,

    //ACK
    PCACK_BOOT                  = 0x80,
    PCACK_FIRMWAREINTACT        = 0x81,
    PCACK_SELECTFIRMWARE        = 0x82,

    PCACK_STARTTEST             = 0x92,
    PCACK_COMPLETETEST          = 0x93,
    PCACK_POWTEST               = 0x94,
    PCACK_STARTONEGROUNDTEST    = 0x95,
    PCACK_UPLOADRFPOWERRESYLT   = 0xa0
};

enum ENUM_FIRMWARECOMMAND{
    //command
    FW_BOOT                     = 0x00,
    FW_FIRMWAREINTACT           = 0x01,
    FW_SELECTFIRMWARE           = 0x02,
    FW_HANDSHAKE                = 0x03,

    FW_STARTTEST                = 0x12,
    FW_COMPLETETEST             = 0x13,
    FW_STARTONEGROUPPOWTEST     = 0x14,
    FW_STARTONEGROUPENUMTEST    = 0x15,
    FW_UPLOADRFPOWERRESYLT      = 0x20,

    //ACK
    FWACK_GETFWINFO         = 0x81,
    FWACK_RESTARTFW         = 0x82,

    FWACK_INITDUTTEST           = 0xb0,
    FWACK_EXITDUTTEST           = 0xb1,
    FWACK_STARTONETEST          = 0xb2,
    FWACK_INQUIREMACHINESTATE   = 0xb3,

    FWACK_COMPLETEENUM      = 0xb5,
    FWACK_ENTERDEBUGMODEL   = 0xd0,
    FWACK_EXITDEBUGMODLE    = 0xd1,

    FWACK_STARTWHOLEDUTTEST = 0xda,
    FWACK_ADJUSTPOWER       = 0xde,
    FWACK_READPOWERDATA     = 0xdf,
    FWACK_CLEARPOWERCONFIG  = 0xe0,

    FWACK_TESTPOWERSELFTEST = 0xe1,
    FWACK_TESTENUM          = 0xe9,
};

enum ENUM_MSGDEFINE{
    NORMAL = 0x00,
    NORMAL_OUTUPGRADEMODEL = 0x01,
    UPGRADEDATABAGERROR = 0x02,

    NOFW = 0x0b,
    FWDAMAGED = 0x0c,

    OTHERERROR = 0x7f,

    ABNORMALITY = 0xff,   //use for autor

    //......and so on and no use again
};

struct STRUCT_PCCOMMANDDATA{
    ENUM_PCCOMMAND PCCommand;
    char DATA[254];
    uint un_DataLength;
    uint un_SendTime;
};

enum ENUM_MACHINEDUTSTATE{
    DUTOUTFIXTURE = 0,
    DUTINFIXTURE = 1
};

enum ENUM_MACHINETESTSTATE{
    UNSTART = 0,
    WAIT,
    TESTING,
    COMPLETE
};

#endif // FIRMWARE_DEFINE_H
