#ifndef DEVICEOBSERVER_H
#define DEVICEOBSERVER_H
#include "Firmware/firmware.h"
#include "Firmware/comdiscoverd.h"
#include "UsbControl/usbcontrol.h"
#include "fwuploaddata.h"
#include "upperdefine.h"
#include "Box/box.h"
#include "Robot/catchrobot.h"
#include "Robot/supplementroobot.h"
#include "tcp/tcpserverinstancegetter.h"

class DeviceObserver : public QObject
{
    Q_OBJECT
    friend class DeviceObserverInstanceGetter;

private:
    explicit DeviceObserver(QObject *parent = NULL);

public:
    ~DeviceObserver();
    //设置固件com配置参数
    bool SetNeedFWConfig(const QList<STRUCT_COMINFO> &list_FWComInfo);
    //设置固件打开方式 com名称
    bool SetOpenFWModelByCom();
    //设置固件打开方式 PIDVID
    bool SetOpenFWModelByPidVid();

//    bool SetManuallyCreatUsbControl();
    //获取固件对象列表
    bool GetFWObjectPointer(QList<Firmware *> &list_FWObjectPointer);
    //获取UsbControl对象指针
    bool GetUsbControlObjectPointer(UsbControl * &p_UsbControl);
    //设置UsbControl对象指针
    bool SetUsbControlObjectPointer(UsbControl * &p_UsbControl);
    //移除UsbControl对象指针
    bool RemoveUsbControlObjectPoint();
    //设置本地TCP服务器
    bool SetLocalTcpServer(const QString &str_IP,
                           const ushort &us_Port);
    //设置箱子IP
    bool SetBoxIP(const QList<QString> &list_BoxIP);
    //设置抓取机器人IP
    bool SetCatchRobotIP(const QString &str_CatchRobotIP);
    //设置分料机器人IP
    bool SetSupplementRobotIP(const QString &str_SupplementRobotIP);
    //获取箱子对象指针
    bool GetBoxObjectPoint(QList<Box *> &list_Box);
    //获取抓取机器人对象指针
    bool GetCatchRobotObjectPoint(CatchRobot * &p_CatchRobot);
    //获取分料机器人对象指针
    bool GetSupplementRobotObjectPoint(SupplementRoobot * &p_SupplementRoobot);

    bool UpdateFW_OpenByCom();

public:
    //清除枚举结果
    bool ClearEnumResult();
    //清除功率测试结果
    bool ClearPowerSendResult();
    //获取所有固件工作序号
    bool GetAllFWSequenceNumber(QList<ushort> &list_SequenceNumber);
    //获取所有箱子工作序号
    bool GetAllBoxSequenceNumber(QList<ushort> &list_SequenceNumber);
    //获取抓取机器人连接情况
    bool GetCatchRobotConnectState();
    //获取分料机器人连接情况
    bool GetSupplementRobotConnectState();

public:
    //获取箱子操作动作
    bool GetBoxOperator(const ushort &us_FWStation,
                        BOX_OPERATOR &box_Operator);

    //获取抓取机器人动作
    bool GetCatchRobotGetAction(const ushort &us_FWStation,
                                QString &str_Action);
    //获取分料机器人分料动作 
    bool GetSupplementRobotGetRequest(const ushort &us_FWStation,
                                      QString &str_Request);


public:
    //获取枚举完成数据
    void GetEnumUsbCompleteData(EMUN_TESTDATAPOINT Point,
                                QMap<int,bool> &map_EnumResult);
    //获取功率测试完成数据
    void GetSendPowerTestCompleteData(EMUN_TESTDATAPOINT Point,
                                      QMap<int,bool> &map_EnumResult,
                                      QMap<int,bool> &map_OpenDeviceResult,
                                      QMap<int, bool> &map_SendCmdResult);
    //获取固件发送数据 cmd + 数据
    void GetReceiveCommandAndData(const ushort &us_SequenceNumber,
                                  uchar uc_Command,
                                  STRUCT_COMMANDANDDATA &struct_CommandAndData);
    //获取pc发送数据 cmd + 数据
    void GetWriteCommandAndData(const ushort &us_SequenceNumber,
                                uchar c_Command,
                                STRUCT_COMMANDANDDATA &struct_CommandAndData);
    //获取固件信息
    void GetGetFWInfoData(EMUN_TESTDATAPOINT Point,
                          const ushort &us_SequenceNumber,
                          STRUCT_FWINFO &struct_FWInfo);
    //获取测试超时时间
    void GetTestTimeOutData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            int &n_TestTimeOut);
    //获取固件测试状态
    void GetMachineStateData(EMUN_TESTDATAPOINT Point,
                             const ushort &us_SequenceNumber,
                             ENUM_MACHINEDUTSTATE &DUTState,
                             ENUM_MACHINETESTSTATE &TestState);
    //获取功率校准数据
    void GetAdjustPowerData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            short &s_Vslop,
                            short &s_VslopAdc,
                            short &s_Intercept);
    //获取读取功率测试数据
    void GetReadPowerData(EMUN_TESTDATAPOINT Point,
                          const ushort &us_SequenceNumber,
                          QList<short> &list_Vslop,
                          QList<short> &list_VslopAdc,
                          QList<short> &list_Intercept,
                          QList<char> &list_db_Compensation,
                          short &s_ReferenceVoltage_1,
                          short &s_ReferenceVoltage_2);
    //获取功率自检测试数据
    void GetPowerSelfTestData(EMUN_TESTDATAPOINT Point,
                              const ushort &us_SequenceNumber,
                              QList<short> &list_SelfVoltage);
    //获取固件boot数据
    void GetFWBootData(EMUN_TESTDATAPOINT Point,
                       const ushort &us_SequenceNumber,
                       STRUCT_FWINFO &struct_FWInfo);
    //获取固件设备状态
    void GetFWIntactData(EMUN_TESTDATAPOINT Point,
                         const ushort &us_SequenceNumber,
                         ENUM_MSGDEFINE &FWIntact);
    //获取心跳包数据
    void GetHandDataBagData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            STRUCT_HANDBAG &struct_HandBag);
    //获取开始测试通知数据
    void GetStartTestNoticeData(EMUN_TESTDATAPOINT Point,
                                const ushort &us_SequenceNumber,
                                char &c_TestType,
                                char &c_TotalGroup,
                                char &c_AmoutOfAGroup,
                                QList<char> &list_UsefulDUT);
    //获取完成测试数据
    void GetCompleteTestData(EMUN_TESTDATAPOINT Point,
                             const ushort &us_SequenceNumber,
                             QList<char> &list_UsefulResult);
    //获取开始一组功率测试数据
    void GetStartOneGroupPowerTestData(EMUN_TESTDATAPOINT Point,
                                       const ushort &us_SequenceNumber,
                                       char &c_SurplusGroup,
                                       int &n_TestDUTMask,
                                       QList<int> &list_MaskCode);
    //获取开始一组枚举测试数据
    void GetStartOneGroupEnumTestData(EMUN_TESTDATAPOINT Point,
                                      const ushort &us_SequenceNumber,
                                      char &c_SurplusGroup,
                                      int &n_TestDUTMask,
                                      QList<int> &list_MaskCode);
    //获取上传RF功率结果数据
    void GetUploadRFPowerResultData(EMUN_TESTDATAPOINT Point,
                                    const ushort &us_SequenceNumber,
                                    QList<short> &list_Power_db);
    //清除固件测试数据
    void ClearFWTestData(EMUN_TESTDATAPOINT Point,
                         const ushort &us_SequenceNumber);


private:
    //初始化com发现者
    void InitComDiscover();
    //初始化本地TCP服务器
    void InitTcpServer();
    //延时时间
    void WorkSleep(ushort un_Msec);
    //获取固件当前测试数据
    bool GetFWDataPointer(const ushort &us_SequenceNumber,
                          FWUploadData * &p_FWUploadData);
    //获取固件上次测试数据
    bool GetLastFWDataPointer(const ushort &us_SequenceNumber,
                                FWUploadData * &p_FWUploadData);
    //固件确认
    bool FWConfirm(STRUCT_COMINFO &struct_ComInfo);
    //匹配固件，添加新固件到固件列表并初始化
    bool AddFW(const QString &str_PortName,
               const uint &un_Pid,
               const uint &un_Vid);
    //添加固件数据缓存对象
    bool AddFWUploadDataObject(const ushort &us_SequenceNumber,
                               const QString &str_PortName);
    //移除固件
    bool RemoveFW(const QString &str_PortName,
                  const uint &un_Pid,
                  const uint &un_Vid);
    //移除固件数据缓存对象
    bool RemoveFWUploadDataObject(const ushort &us_SequenceNumber,
                                  const QString &str_PortName);
    //移除所有固件和固件数据缓存对象
    bool RemoveAllFWAndFWData();
    //更新上次固件测试缓存数据
    bool UpdataLastFWTestData(const ushort &us_SequenceNumber);
    //转换测试掩码 bit to list<int>
    bool TransformMaskCode(const ushort &us_SequenceNumber,
                           const int &n_MaskData,
                           QList<int> &list_MaskCode);
    //赋值 更新 usb测试数据 
    bool ReplaceUsbControlResult(const ushort &us_SequenceNumber,
                                 QMap<int,bool> &map_FromResultData,
                                 QMap<int,bool> &map_ToResultData);
    //添加box对象
    bool AddBox(const QString &str_IP);
    //移除box对象
    bool RemoveBox(const QString &str_IP);
    //添加抓取机器人对象
    bool AddCatchRobot(const QString &str_IP);
    //移除抓取机器人对象
    bool RemoveCatchRobot(const QString &str_IP);
    //添加分料机器人对象 
    bool AddSupplementRobot(const QString &str_IP);
    //移除分料机器人对象
    bool RemoveSupplementRobot(const QString &str_IP);

signals:
    //发送 添加匹配新固件
    void sig_FirmwareDiscoverd();
    //发送 移除已匹配固件
    void sig_FirmwareRemove();

signals:
    //发送 添加匹配新箱子
    void sig_BoxDiscoverd();
    //发送 添加抓取机器人
    void sig_CatchRobotDiscoverd();
    //发送 添加分料机器人 
    void sig_SupplementRobotDiscoverd();
    //发送 移除已匹配箱子 
    void sig_BoxRemove();
    //发送 移除抓取机器人
    void sig_CatchRobotRemove();
    //发送 移除分料机器人
    void sig_SupplementRobotRemove();

signals:
    //发送 箱子动作更新 
    void sig_BoxOperatorUpdata(ushort us_SequenceNumber);

signals:
    //发送 取料机器人动作更新
    void sig_CatchRobotGetActionUpdata(ushort us_SequenceNumber);

signals:
    //发送 分料机器人请求更新
    void sig_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber);

signals:
    //发送 枚举usb完成
    void sig_EnumUsbComplete();
    //发送 载波cmd发送完成
    void sig_SendPowerTestComplete();

signals:
    //发送 接收cmd数据
    void sig_ReceiveCmd(ushort us_SequenceNumber,
                        uchar uc_Command);
    //发送 写入cmd数据
    void sig_WriteCmd(ushort us_SequenceNumber,
                      uchar uc_Command);
    //发送 获取固件信息
    void sig_GetFWInfo(ushort us_SequenceNumber);
    //发送 测试时间
    void sig_TestTimeOut(ushort us_SequenceNumber);
    //发送 固件工作状态
    void sig_MachineState(ushort us_SequenceNumber);
    //发送 校准功率
    void sig_AdjustPower(ushort us_SequenceNumber);
    //发送 读取校准功率
    void sig_ReadPower(ushort us_SequenceNumber);
    //发送 功率自检测试
    void sig_PowerSelfTest(ushort us_SequenceNumber);
    //发送 固件boot信息
    void sig_FWBoot(ushort us_SequenceNumber);
    //发送 固件设备状态
    void sig_FWIntact(ushort us_SequenceNumber);
    //发送 握手包信息
    void sig_HandDataBag(ushort us_SequenceNumber);
    //发送 开始测试通知
    void sig_StartTestNotice(ushort us_SequenceNumber);
    //发送 完成测试
    void sig_CompleteTest(ushort us_SequenceNumber);
    //发送 开始一组功率测试
    void sig_StartOneGroupPowerTest(ushort us_SequenceNumber);
    //发送 开始一组枚举测试
    void sig_StartOneGroupEnumTest(ushort us_SequenceNumber);
    //发送 上传RF功率结果
    void sig_UploadRFPowerResult(ushort us_SequenceNumber);

public slots:
    //发现新com 处理
    void slot_FirmwareDiscoverd(QString str_Port,
                                uint un_Pid,
                                uint un_Vid);
    //发现com断开 处理
    void slot_FirmwareRemove(QString str_Port,
                             uint un_Pid,
                             uint un_Vid);
public slots:
    //新用户连接信息
    void slot_ConnectClient(int n_ID,
                           QString str_IP,
                           quint16 us_Port);
    //断开连接的用户信息
    void slot_DisConnectClient(int n_ID,
                               QString str_IP,
                               quint16 us_Port);

// us_FWStation == us_SequenceNumber
public slots:
    //收到 箱子动作
    void slot_BoxOperator(ushort us_FWStation,
                          BOX_OPERATOR box_Operator);

public slots:
    //收到 取料机器人动作
    void slot_CatchRobotGetAction(ushort us_FWStation,
                                  QString str_Action);

public slots:
    //收到 分料机器人请求信号
    void slot_SupplementRobotGetRequest(ushort us_FWStation,
                                        QString str_Request);
public slots:
    //收到 枚举usb完成
    void slot_EnumUsbComplete();
    //收到 载波cmd发送完成
    void slot_SendPowerTestComplete();

public slots:
    //收到 接收cmd数据
    void slot_ReceiveCommand(ushort us_SequenceNumber,
                             uchar uc_Command,
                             QByteArray byte_Data,
                             uint un_DataLength);
    //收到 写入cmd数据
    void slot_WriteCommand(ushort us_SequenceNumber,
                           uchar uc_Command,
                           QByteArray byte_Data,
                           uint un_DataLength);
    //收到 获取固件信息
    void slot_GetFWInfo(ushort us_SequenceNumber,
                        STRUCT_FWINFO struct_FWInfo);
    //收到 测试时间
    void slot_TestTimeOut(ushort us_SequenceNumber,
                          int n_TestTimeOut);
    //收到 固件工作状态
    void slot_MachineState(ushort us_SequenceNumber,
                           ENUM_MACHINEDUTSTATE DUTState,
                           ENUM_MACHINETESTSTATE TestState);
    //收到 校准功率
    void slot_AdjustPower(ushort us_SequenceNumber,
                          short s_Vslop,
                          short s_VslopAdc,
                          short s_Intercept);
    //收到 读取校准功率
    void slot_ReadPower(ushort us_SequenceNumber,
                        QList<short> list_Vslop,
                        QList<short> list_VslopAdc,
                        QList<short> list_Intercept,
                        QList<char> list_db_Compensation,
                        short s_ReferenceVoltage_1,
                        short s_ReferenceVoltage_2);
    //收到 功率自检测试
    void slot_PowerSelfTest(ushort us_SequenceNumber,
                            QList<short> list_SelfVoltage);
    //收到 固件boot信息
    void slot_FWBoot(ushort us_SequenceNumber,
                     STRUCT_FWINFO struct_FWInfo);
    //收到 固件设备状态
    void slot_FWIntact(ushort us_SequenceNumber,
                       ENUM_MSGDEFINE FWIntact);
    //收到 握手包信息（心跳包）
    void slot_HandDataBag(ushort us_SequenceNumber,
                          STRUCT_HANDBAG struct_HandBag);
    //收到 开始测试通知
    void slot_StartTestNotice(ushort us_SequenceNumber,
                              char c_TestType,
                              char c_TotalGroup,
                              char c_AmoutOfAGroup,
                              QList<char> list_UsefulDUT);
    //收到 完成测试
    void slot_CompleteTest(ushort us_SequenceNumber,
                           QList<char> list_UsefulResult);
    //收到 开始一组功率测试
    void slot_StartOneGroupPowerTest(ushort us_SequenceNumber,
                                     char c_SurplusGroup,
                                     int n_TestDUTMask);
    //收到 开始一组枚举测试
    void slot_StartOneGroupEnumTest(ushort us_SequenceNumber,
                                    char c_SurplusGroup,
                                    int n_TestDUTMask);
    //收到 上传RF功率结果
    void slot_UploadRFPowerResult(ushort us_SequenceNumber,
                                  QList<short> list_Power_db);


private slots:

private:
    ComDiscoverd *m_pComDiscoverd;          //com发现者对象
    QList<Firmware *> m_listFirmware;       //固件列表
    UsbControl *m_pUsbControl;              //usb控制对象
    QThread *m_pQThread;                    //线程对象

    TcpServerInstanceGetter m_oTcpServerInstanceGetter;     //本地TCP服务器类管理类
    TcpServer *m_pTcpServer;    //本地服务器对象

    QList<STRUCT_COMINFO> m_listNeedFW;     //固件com确认配置
    QList<STRUCT_COMINFO> m_listFindCom;    //固件com已匹配的配置

    QList<FWUploadData *> m_listFWUploadData;   //固件当前测试 上传数据缓存
    QMap<int,bool> m_mapEnumResult;             //当前枚举结果
    QMap<int,bool> m_mapOpenDeviceResult;       //当前打开设备结果
    QMap<int,bool> m_mapSendCmdResult;          //当前发送载波cmd结果

    QList<FWUploadData *> m_listLastFWUploadData;   //固件上次测试 上传数据缓存
    QMap<int,bool> m_mapLastEnumResult;             //上次枚举结果
    QMap<int,bool> m_mapLastOpenDeviceResult;       //上次打开设备结果
    QMap<int,bool> m_mapLastSendCmdResult;          //上次发送载波cmd结果

    ENUM_OPENMODEL m_OpenFWModel;           //固件com打开方式

    bool m_bManuallyCreatUsbControl;        //手动new CreatUsbControl对象

    QList<STRUCT_COMMANDANDDATA> m_listReceiveCommandAndData;   //接收 cmd和数据
    QList<STRUCT_COMMANDANDDATA> m_listWriteCommandAndData;     //写入 cmd和 数据

    QString m_strLocalTcpServerIP;      //本地tcp IP
    ushort m_usLocalTcpServerPort;      //本地tcp port

    QList<QString> m_listBoxIP;         //箱子  ip
    QList<Box *> m_listBox;             //箱子 对象列表

    QString m_strCatchRobotIP;          //抓取机器人 ip
    CatchRobot *m_pCatchRobot;          //抓取机器人对象

    QString m_strSupplementRobotIP;         //分料机器人 ip
    SupplementRoobot *m_pSupplementRoobot;  //分料机器人 对象

    QMap<ushort, BOX_OPERATOR> m_mapBoxOperator;    //box 动作 <工作序号, 动作>
    QMap<ushort, QString> m_mapCatchRobotAction;    //抓取机器人 <工作位置, 动作>
    QMap<ushort, QString> m_mapSupplementRobotRequest;  //分料机器人 <工作位置, 请求cmd>

    QList<ushort> m_listFWSequenceNumber;       //固件工作序号 列表
    QList<ushort> m_listBoxSequenceNumber;      //箱子工作序号 列表

    bool m_bCatchRobotConnect;                  //抓取机器人连接状态
    bool m_bSupplementRobotConnect;             //分料机器人连接状态
};

#endif // DEVICEOBSERVER_H
