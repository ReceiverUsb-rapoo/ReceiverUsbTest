#ifndef FIRMWARE_DEFINE_H
#define FIRMWARE_DEFINE_H

#define UNUSED_PARAMETER

#include <QString>

const int MAX_LENGHT            = 2048;                         //数据最大长度 
const int UP_HEADER_LENGHT      = 4;                            //数据包头长度 上行帧
const int DOWN_HEADER_LENGHT    = 4;                            //数据包头长度 下行帧
const int FRAME_LENGHT          = 2;                            //数据包帧长
const int COMMAND_LENGHT        = 1;                            //cmd长度 
const int CHECKSUM_LENGHT       = 2;                            //校验长度
const int FRAMETAIL_LENGHT      = 2;                            //数据包尾长度
const int BAGDATA_SHORTEST      = 11;                           //数据包最小长度
const int DATA_MAX_LENGHT       = 241;                          //数据包最大长度
const char UP_HEADER[5]         = {0x55, 0x50, 0x4C, 0x3A};     //包头 上行帧
const char DOWN_HEADER[5]       = {0x44, 0x4E, 0x4C, 0x3A};     //包头 下行帧
const char FRAMETAIL[3]         = {0x0D, 0x0A};                 //包尾        

const int PositionTotal         = 32;                           //夹具内部容量
const int ShortTimeOut          = 500;                          //断超时 
const int LongTimeOut           = 5000;                         //长超时 

//打开com方式
enum ENUM_OPENMODEL{
    BYCOM = 0,  
    BYPIDVID
};

//控制上电开关
enum ENUM_POWERONSWITCH{
    POWERON     = 0x00,
    POWEROFF    = 0x01
};

//上电 组别 
enum ENUM_POWERONGROUP{
    FIRSTGROUP      = 0x01,
    SECONDGROUP     = 0x02
};

//测试类型
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

//cmd pc端
enum ENUM_PCCOMMAND{
    //comand
    PC_GETFWINFO            = 0x01,         //获取固件信息
    PC_RESTARTFW            = 0x02,         //复位固件

    PC_INITRDUTTEST         = 0x30,         //初始化固件
    PC_EXITDUTTEST          = 0x31,         //退出/停止 DUT测试
    PC_STARTONETEST         = 0x32,         //开始一次DUT测试
    PC_INQUIREMACHINESTATE  = 0x33,         //查询固件状态

    PC_COMPLETEENUM         = 0x35,         //完成枚举通知
    PC_ENTERDEBUGMODEL      = 0x50,         //进入调试模式
    PC_EXITDEBUGMODLE       = 0x51,         //退出调试模式

    PC_STARTWHOLEDUTTEST    = 0x5a,         //测试整个DUT流程
    PC_ADJUSTPOWER          = 0x5e,         //校准功率
    PC_READPOWERDATA        = 0x5f,         //读取校准功率
    PC_CLEARPOWERCONFIG     = 0x60,         //清除校准功率

    PC_TESTPOWERSELFTEST    = 0x61,         //测试功率自检
    PC_TESTENUM             = 0x69,         //测试枚举
    PC_POWERONSWITCH        = 0x6a,         //控制上电 

    //ACK
    PCACK_BOOT                  = 0x80,     //pc-ack boot 信息 
    PCACK_FIRMWAREINTACT        = 0x81,     //pc-ack 固件状态
    PCACK_SELECTFIRMWARE        = 0x82,     //pc-ack 选择固件

    PCACK_STARTTEST             = 0x92,     //pc-ack 开始测试通知
    PCACK_COMPLETETEST          = 0x93,     //pc-ack 完成测试通知
    PCACK_POWTEST               = 0x94,     //pc-ack 开始一组功率测试 
    PCACK_STARTONEGROUNDTEST    = 0x95,     //pc-ack 开始以组枚举测试
    PCACK_UPLOADRFPOWERRESYLT   = 0xa6,     //pc-ack 上传RF功率测试
};

enum ENUM_FIRMWARECOMMAND{
    //command
    FW_BOOT                     = 0x00,     //boot 信息 
    FW_FIRMWAREINTACT           = 0x01,     //固件状态
    FW_SELECTFIRMWARE           = 0x02,     //选择固件
    FW_HANDSHAKE                = 0x03,     //握手协议

    FW_STARTTEST                = 0x12,     //开始测试通知
    FW_COMPLETETEST             = 0x13,     //完成测试通知
    FW_STARTONEGROUPPOWTEST     = 0x14,     //开始一组功率测试 
    FW_STARTONEGROUPENUMTEST    = 0x15,     //开始以组枚举测试
    FW_UPLOADRFPOWERRESYLT      = 0x26,     //上传RF功率测试

    //ACK
    FWACK_GETFWINFO         = 0x81,         //fw-ack 获取固件信息
    FWACK_RESTARTFW         = 0x82,         //fw-ack 复位固件

    FWACK_INITDUTTEST           = 0xb0,     //fw-ack 初始化固件
    FWACK_EXITDUTTEST           = 0xb1,     //fw-ack 退出/停止 DUT测试
    FWACK_STARTONETEST          = 0xb2,     //fw-ack 开始一次DUT测试
    FWACK_INQUIREMACHINESTATE   = 0xb3,     //fw-ack 查询固件状态

    FWACK_COMPLETEENUM      = 0xb5,         //fw-ack 完成枚举通知
    FWACK_ENTERDEBUGMODEL   = 0xd0,         //fw-ack 进入调试模式
    FWACK_EXITDEBUGMODLE    = 0xd1,         //fw-ack 退出调试模式

    FWACK_STARTWHOLEDUTTEST = 0xda,         //fw-ack 测试整个DUT流程
    FWACK_ADJUSTPOWER       = 0xde,         //fw-ack 校准功率
    FWACK_READPOWERDATA     = 0xdf,         //fw-ack 读取校准功率
    FWACK_CLEARPOWERCONFIG  = 0xe0,         //fw-ack 清除校准功率

    FWACK_TESTPOWERSELFTEST = 0xe1,         //fw-ack 测试功率自检
    FWACK_TESTENUM          = 0xe9,         //fw-ack 测试枚举
    FWACK_POWERONSWITCH     = 0xea,         //fw-ack 控制上电 
};

enum ENUM_MSGDEFINE{
    NORMAL = 0x00,                          //执行正常
    NORMAL_OUTUPGRADEMODEL = 0x01,          //不在固件升级模式
    UPGRADEDATABAGERROR = 0x02,             //固件升级数据包序号错误

    NOFW = 0x0b,                            //无固件
    FWDAMAGED = 0x0c,                       //固件损坏

    OTHERERROR = 0x7f,                      //未知错误 

    ABNORMALITY = 0xff,   //use for autor   //自定 错误类型

    //......and so on and no use again
};

//固件状态 
enum ENUM_MACHINEDUTSTATE{
    DUTOUTFIXTURE = 0,      //在线
    DUTINFIXTURE = 1        //离线
};  

//测试状态
enum ENUM_MACHINETESTSTATE{
    UNSTART = 0,        //未开始
    WAIT,               //等待 
    TESTING,            //测试中
    COMPLETE            //完成
};

//#pragma pack(push)  //将当前pack设置压栈保存
#pragma pack(1)     //必须在结构体定义之前使用

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

    ushort us_FirstPowerTestDelay;    //第一次功率测试延时
    ushort us_EnumTime;               //默认=8000
    uchar uc_EnumTestSwitch;          //枚举测试开关 
    uchar uc_TotalByte;               //字节总数

    //初始化数据
    STRUCT_FWCONGIF(){
        us_WorkCurrentTime = 0;
        us_SleepCurrentTime = 0;
        us_LEDTestTime = 0;
        us_LED2TestTime = 0;
        us_IRTestTime = 0;
        us_LaserTestTime = 0;
        us_BurnEEPROMTime = 0;
        us_ChargeTextTime = 0;
        us_SendTime = 0;
        us_ReceiveTime = 0;
        us_PoweredDelay = 1200;
        us_ReadRFDelay = 5000;
        us_SleepCurrentDelay = 4000;
        us_DischargeTimeSelect = 2000;
        us_DUTVoltageSelect = 1500;
        us_CloseCurrentSet = 0;
        uc_IRTestSwitch = 0;

        uc_LEDTestSwitch = 0;
        uc_LaserTestSwitch = 0;
        uc_FunctionTextSwitch = 0;
        uc_SelfTestCode = 0;
        uc_RetestTimes = 0;
        uc_IRCollectAvgTimes = 0;
        uc_LEDCollectAvgTimes = 0;
        uc_LED2CollectAvgTimes = 0;
        uc_CurrentCollectAvgTimes = 0;

        uc_CrytalCollectAvgTimes = 0;
        uc_ReceiveRSSILimit = 0;
        uc_SingleTestCount = 4;
        us_SinglePowerDelay = 0;
        us_SinglePowerTime = 0;
        uc_PowerAvgTimes = 0;
        uc_PowerChannel = 0;
        uc_PowerTestSwitch = 0;
        us_FirstPowerTestDelay = 0;
        us_EnumTime = 8000;

        uc_EnumTestSwitch = 0;
        uc_TotalByte = 0;
    }
};

struct STRUCT_PCTESTCONFIG{
    ENUM_TESTTYPE TestType;           //测试类型
    uint un_FWPosition;               //固件位置
    ushort us_FWPid;                  //固件PID
    ushort us_FWVid;                  //固件VID
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

    QString str_ProjectName;            //测试项目名称 
    QString str_ProjectVersions;        //测试参数版本

    //初始化数据
    STRUCT_PCTESTCONFIG(){
        TestType = _INTACTRECEIVER;
        un_FWPosition = 0;
        us_FWPid = 0;
        us_FWVid = 0;
        us_PoweredDelay = 1200;
        us_ReadRFDelay = 5000;
        us_DischargeTimeSelect = 2000;
        us_DUTVoltageSelect = 1500;
        uc_RetestTimes = 0;
        uc_SingleTestCount = 4;
        us_SinglePowerDelay = 0;
        us_SinglePowerTime = 0;
        uc_PowerAvgTimes = 0;
        uc_PowerChannel = 0;
        uc_PowerTestSwitch = 0;
        us_FirstPowerTestDelay = 0;
        us_EnumTime = 8000;
        uc_EnumTestSwitch = 0;

        str_DUTFWPositions = "";
        str_PowerDUTPositions = "";
        str_PowerTestGroups = "";
        str_RFPowerDBUponLimit = "";
        str_RFPowerDBLowerLimit = "";

        str_ProjectName = "";
        str_ProjectVersions = "";
    }
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

//握手包数据
struct STRUCT_HANDBAG{
    char c_DUTState;            //DUT状态
    char c_TestState;           //测试状态
    char c_TestType;            //测试类型
    char c_SurplusGroup;        //剩余分组
    char c_TestingProgress;     //测试进度

    STRUCT_HANDBAG(){
        c_DUTState = 0;
        c_TestState = 0;
        c_TestType = 0;
        c_SurplusGroup = 0;
        c_TestingProgress = 0;
    }
};

//PC端命令
struct STRUCT_PCCOMMANDDATA{
    ENUM_PCCOMMAND PCCommand;   //cmd
    char DATA[254];             //数据 
    uint un_DataLength;         //数据长度
    uint un_SendTime;           //发送时间

    STRUCT_PCCOMMANDDATA(){
        PCCommand = PC_GETFWINFO;
        memset(DATA, 0, 254);
        un_DataLength = 0;
        un_SendTime = 0;
    }
};

#pragma pack(0) // 恢复先前的pack设置

#endif // FIRMWARE_DEFINE_H
