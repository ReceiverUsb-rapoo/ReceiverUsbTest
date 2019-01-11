#ifndef FIRMWARE_H
#define FIRMWARE_H
#include <QTimer>
#include <QMap>
#include <QTime>
#include "firmwarecom.h"
#include "firmware_define.h"

class Firmware :public QObject
{
    Q_OBJECT
public:
    explicit Firmware(QObject *parent = 0);
    ~Firmware();

    //初始化固件
    bool InitFirmware();
    //退出固件
    bool ExitFirmware();
    //获取固件序号
    bool GetSequenceNumber(ushort &us_SequenceNumber);
    //设置固件配置
    bool SetFirmwareConfig(const QString &str_Com,
                           const uint &un_Pid,
                           const uint &un_Vid);
    //获取固件配置
    bool GetFirmwareConfig(QString &str_Com,
                           uint &un_Pid,
                           uint &un_Vid);
    //设置打开固件方式为com名称 
    bool SetOpenComModelByCom();
    //设置打开固件方式未PIDVID
    bool SetOpenComModelByPidVid();
    //设置PC和固件配置参数
    bool SetPCConfig(STRUCT_PCTESTCONFIG &struct_PCConfig);
    //打开固件
    bool OpenFirmware();
    //关闭固件
    bool CloseFirmware();

public:
    //pc send
    //获取固件信息
    bool PC_GetFWInfo();
    //复位固件
    bool PC_RestartFW();
    //初始化固件
    bool PC_InitDUTTest();
    //退出/停止 DUT测试
    bool PC_ExitDUTTest();
    //开始一次DUT测试
    bool PC_StartOneTest();
    //查询固件状态
    bool PC_InquireMachineState();
    //完成枚举通知
    bool PC_CompleteEnumTest();
    //进入调试模式
    bool PC_EnterDebugModel();
    //退出调试模式
    bool PC_ExitDebugModel();
    //测试整个DUT流程
    bool PC_StartWholeDUTTest();
    //校准功率
    bool PC_AdjustPower(char c_Funtion = 0x00,
                        short s_ReferenceVoltage_1 = 0x09c4,
                        short s_ReferenceVoltage_2 = 0x09c4,
                        uchar uc_Station = 0x01,
                        char c_AdjustPoint = 0x32,
                        char c_dp_Compensation = 0x00);
    //读取校准功率
    bool PC_ReadPowerData();
    //清除校准功率
    bool PC_ClearPowerConfig(const int &n_Station);
    //测试功率自检
    bool PC_TestPowerSelfTest();
    //测试枚举
    bool PC_TestEnum(const ENUM_POWERONGROUP &Group,
                     const short &s_Time);
    //控制上电 
    bool PC_PowerOnSwitch(const ENUM_POWERONSWITCH &Switch,
                          const ENUM_POWERONGROUP &Group);

private:
    //延时函数
    void WorkSleep(ushort un_Msec);
    //短超时发送cmd
    bool SendCommandData_ShortTimeOut(ENUM_PCCOMMAND PCCommand,
                                      char *p_cData,
                                      uint un_DataLength);
    //长超时发送cmd
    bool SendCommandData_LongtTimeOut(ENUM_PCCOMMAND PCCommand,
                                      char *p_cData,
                                      uint un_DataLength);
    //发送cmd +  data
    bool SendCommandData(ENUM_PCCOMMAND PCCommand,
                         char *p_cData,
                         uint un_DataLength);
    //移除超时发送列表中的cmd 通过固件ACK移除
    bool FWACKRemoveTimeOut(ENUM_FIRMWARECOMMAND FWCommand);

//    bool AnalyzeDataArray_1_Byte(ENUM_FIRMWARECOMMAND FWCommand,
//                            char *p_cData,
//                            uint &un_DataLength);

//    bool AnalyzeDataArray_2_Byte(ENUM_FIRMWARECOMMAND FWCommand,
//                            char *p_cData,
//                            uint &un_DataLength);
    //固件cmd ack回复
    bool FWCMDControl(uchar &uc_Command,
                      QByteArray &byte_Data,
                      uint &un_DataLength);
    bool TntervalTimeReceive(ENUM_FIRMWARECOMMAND FWCommad);

private:
    //pc ack send to fw
    //pc-ack boot回复
    bool PCACK_boot();
    //pc-ack 固件状态回复 
    bool PCACK_FirmwareIntact(ENUM_MSGDEFINE FWState);
    //pc-ack 固件选择回复
    bool PCACK_SelectFirmware();
    //pc-ack 开始测试回复
    bool PCACK_StartTest();
    //pc-ack 完成测试回复
    bool PCACK_CompleteTest();

public:
    //pc-ack 开始一组功率测试回复
    bool PCACK_StartOneGroupPowerTest();

private:
    //pc-ack 开始一组枚举测试回复
    bool PCACK_StartOneGroupEnumTesT();
    //pc-ack 上传RF功率回复
    bool PCACK_UploadRFPowerResult();

    //pc receive from fw cmd
    //boot 信息 
    bool FW_boot(char *p_cData,
                 const uint &un_DataLength);
    //固件状态
    bool FW_FirmwareIntact(char *p_cData,
                           const uint &un_DataLength);
    //选择固件
    bool FW_SelectFirmware();
    //握手协议
    bool FW_HandShake(char *p_cData,
                      const uint &un_DataLength);
    //开始测试通知
    bool FW_StartTest(char *p_cData,
                      const uint &un_DataLength);
    //完成测试通知
    bool FW_CompleteTest(char *p_cData,
                         const uint &un_DataLength);
    //开始一组功率测试 
    bool FW_StartOneGroupPowerTest(char *p_cData,
                                   const uint &un_DataLength);
    //开始以组枚举测试
    bool FW_StartOneGroupEnumTesT(char *p_cData,
                                  const uint &un_DataLength);
    //上传RF功率测试
    bool FW_UploadRFPowerResult(char *p_cData,
                                const uint &un_DataLength);

    //pc receive from fw ack
    //fw-ack 获取固件信息
    bool FWACK_GetFWInfo(char *p_cData,
                         const uint &un_DataLength);
    //fw-ack 复位固件
    bool FWACK_RestartFW();
    //fw-ack 初始化固件
    bool FWACK_InitDUTTest();
    //fw-ack 退出/停止 DUT测试
    bool FWACK_ExitDUTTest();
    //fw-ack 开始一次DUT测试
    bool FWACK_StartOneTest(char *p_cData,
                            const uint &un_DataLength);
    //fw-ack 查询固件状态
    bool FWACK_InquireMachineState(char *p_cData,
                                   const uint &un_DataLength);
    //fw-ack 完成枚举通知
    bool FWACK_CompleteEnumTest();
    //fw-ack 进入调试模式
    bool FWACK_EnterDebugModel();
    //fw-ack 退出调试模式
    bool FWACK_ExitDebugModel();
    //fw-ack 测试整个DUT流程
    bool FWACK_StartWholeDUTTest();
    //fw-ack 校准功率
    bool FWACK_AdjustPower(char *p_cData,
                           const uint &un_DataLength);
    //fw-ack 读取校准功率
    bool FWACK_ReadPowerData(char *p_cData,
                             const uint &un_DataLength);
    //fw-ack 清除校准功率
    bool FWACK_ClearPowerConfig();
    //fw-ack 测试功率自检
    bool FWACK_TestPowerSelfTest(char *p_cData,
                                 const uint &un_DataLength);
    //fw-ack 测试枚举
    bool FWACK_TestEnum();
    //fw-ack 控制上电 
    bool FWACK_PowerOnSwitch();

signals:
    //发送 接收cmd数据
    void sig_ReceiveCommand(ushort us_SequenceNumber,
                            uchar uc_Command,
                            QByteArray byte_Data,
                            uint un_DataLength);
    //发送 写入cmd数据
    void sig_WriteCommand(ushort us_SequenceNumber,
                          uchar uc_Command,
                          QByteArray byte_Data,
                          uint un_DataLength);
    //发送 获取固件信息
    void sig_GetFWInfo(ushort us_SequenceNumber,
                       STRUCT_FWINFO struct_FWInfo);
    //发送 测试时间
    void sig_TestTimeOut(ushort us_SequenceNumber,
                         int n_TestTimeOut);
    //发送 固件工作状态
    void sig_MachineState(ushort us_SequenceNumber,
                          ENUM_MACHINEDUTSTATE,
                          ENUM_MACHINETESTSTATE);
    //发送 校准功率
    void sig_AdjustPower(ushort us_SequenceNumber,
                         short s_Vslop,
                         short s_VslopAdc,
                         short s_Intercept);
    //发送 读取校准功率
    void sig_ReadPower(ushort us_SequenceNumber,
                       QList<short> list_Vslop,
                       QList<short> list_VslopAdc,
                       QList<short> list_Intercept,
                       QList<char> list_db_Compensation,
                       short s_ReferenceVoltage_1,
                       short s_ReferenceVoltage_2);
    //发送 功率自检测试 
    void sig_PowerSelfTest(ushort us_SequenceNumber,
                           QList<short> list_SelfVoltage);
    //发送 固件boot信息
    void sig_FWBoot(ushort us_SequenceNumber,
                    STRUCT_FWINFO struct_FWInfo);
    //发送 固件设备状态
    void sig_FWIntact(ushort us_SequenceNumber,
                      ENUM_MSGDEFINE FWIntact);
    //发送 握手包信息
    void sig_HandDataBag(ushort us_SequenceNumber,
                         STRUCT_HANDBAG struct_HandBag);
    //发送 开始测试通知 
    void sig_StartTestNotice(ushort us_SequenceNumber,
                             char c_TestType,
                             char c_TotalGroup,
                             char c_AmoutOfAGroup,
                             QList<char> list_UsefulDUT);
    //发送 完成测试
    void sig_CompleteTest(ushort us_SequenceNumber,
                          QList<char> list_UsefulResult);
    //发送 开始一组功率测试
    void sig_StartOneGroupPowerTest(ushort us_SequenceNumber,
                                    char c_SurplusGroup,
                                    int n_TestDUTMask);
    //发送 开始一组枚举测试
    void sig_StartOneGroupEnumTest(ushort us_SequenceNumber,
                                   char c_SurplusGroup,
                                   int n_TestDUTMask);
    //发送 上传RF功率结果 
    void sig_UploadRFPowerResult(ushort us_SequenceNumber,
                                 QList<short> list_Power_db);

public slots:
    //接收 接收cmd数据
    void slot_ReceiveCommand(char c_Command,
                             QByteArray byte_Data,
                             uint un_DataLength);
    //接收  写入cmd数据
    void slot_WriteCommand(char c_Command,
                          QByteArray byte_Data,
                          uint un_DataLength);

private slots:
    //短超时cmd处理 轮询处理
    void slot_ShortTimeOut();
    //长超时cmd处理 轮询处理
    void slot_LongTimeOut();

private:
    FirmwareCom *m_pFirmwareCom;
    STRUCT_PCTESTCONFIG m_structPCConfig;   //固件配置参数
    ushort m_usSequenceNumber;              //固件序号 

    QTimer *m_pLongQTimer;
    QTimer *m_pShortQTimer;

    QThread *m_pQThread;

    QMap<ENUM_FIRMWARECOMMAND, QString> m_mapFWCommand_Interval;    //接收时间 map <cmd,上次接收时间>

    QMap<ENUM_PCCOMMAND, QTime> m_mapPCCommand_ShortTimeOut;    //短超时cmd map <cmd,时间>
    QMap<ENUM_PCCOMMAND, QTime> m_mapPCCommand_LongTimeOut;     //长超时cmd map <cmd,时间>

    QMap<ENUM_PCCOMMAND, STRUCT_PCCOMMANDDATA> m_mapPCCommandData;  //<cmd, 附带的数据>
};

#endif // FIRMWARE_H
