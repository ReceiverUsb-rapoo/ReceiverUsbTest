#ifndef DEVICEOBSERVER_H
#define DEVICEOBSERVER_H
#include "Firmware/firmware.h"
#include "Firmware/comdiscoverd.h"
#include "UsbControl/usbcontrol.h"
#include "fwuploaddata.h"
#include "upperdefine.h"
#include "Box/box.h"
#include "Robot/catchrobot.h"
#include"Robot/supplementroobot.h"
#include "tcp/tcpserverinstancegetter.h"

class DeviceObserver : public QObject
{
    Q_OBJECT
    friend class DeviceObserverInstanceGetter;

private:
    explicit DeviceObserver(QObject *parent = NULL);

public:
    ~DeviceObserver();

    bool SetNeedFWConfig(const QList<STRUCT_COMINFO> &list_FWComInfo);

    bool SetOpenFWModelByCom();

    bool SetOpenFWModelByPidVid();

//    bool SetManuallyCreatUsbControl();

    bool GetFWObjectPointer(QList<Firmware *> &list_FWObjectPointer);

    bool GetUsbControlObjectPointer(UsbControl * &p_UsbControl);

    bool SetUsbControlObjectPointer(UsbControl * &p_UsbControl);

    bool RemoveUsbControlObjectPoint();

    bool SetLocalTcpServer(const QString &str_IP,
                           const ushort &us_Port);

    bool SetBoxIP(const QList<QString> &list_BoxIP);

    bool SetCatchRobotIP(const QString &str_CatchRobotIP);

    bool SetSupplementRobotIP(const QString &str_SupplementRobotIP);

    bool GetBoxObjectPoint(QList<Box *> &list_Box);

    bool GetCatchRobotObjectPoint(CatchRobot * &p_CatchRobot);

    bool GetSupplementRobotObjectPoint(SupplementRoobot * &p_SupplementRoobot);

public:
    bool ClearEnumResult();

    bool ClearPowerSendResult();

    bool GetAllSequenceNumber(QList<ushort> &list_SequenceNumber);

public:
    bool GetBoxOperator(const ushort &us_FWStation,
                        BOX_OPERATOR &box_Operator);


    bool GetCatchRobotGetAction(const ushort &us_FWStation,
                                QString &str_Action);

    bool GetSupplementRobotGetRequest(const ushort &us_FWStation,
                                      QString &str_Request);


public:
    void GetEnumUsbCompleteData(EMUN_TESTDATAPOINT Point,
                                QMap<int,bool> &map_EnumResult);

    void GetSendPowerTestCompleteData(EMUN_TESTDATAPOINT Point,
                                      QMap<int,bool> &map_EnumResult,
                                      QMap<int,bool> &map_OpenDeviceResult,
                                      QMap<int, bool> &map_SendCmdResult);

    void GetReceiveCommandAndData(const ushort &us_SequenceNumber,
                                  uchar uc_Command,
                                  STRUCT_COMMANDANDDATA &struct_CommandAndData);

    void GetWriteCommandAndData(const ushort &us_SequenceNumber,
                                uchar c_Command,
                                STRUCT_COMMANDANDDATA &struct_CommandAndData);

    void GetGetFWInfoData(EMUN_TESTDATAPOINT Point,
                          const ushort &us_SequenceNumber,
                          STRUCT_FWINFO &struct_FWInfo);

    void GetTestTimeOutData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            int &n_TestTimeOut);

    void GetMachineStateData(EMUN_TESTDATAPOINT Point,
                             const ushort &us_SequenceNumber,
                             ENUM_MACHINEDUTSTATE &DUTState,
                             ENUM_MACHINETESTSTATE &TestState);

    void GetAdjustPowerData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            short &s_Vslop,
                            short &s_VslopAdc,
                            short &s_Intercept);

    void GetReadPowerData(EMUN_TESTDATAPOINT Point,
                          const ushort &us_SequenceNumber,
                          QList<short> &list_Vslop,
                          QList<short> &list_VslopAdc,
                          QList<short> &list_Intercept,
                          QList<char> &list_db_Compensation,
                          short &s_ReferenceVoltage_1,
                          short &s_ReferenceVoltage_2);

    void GetPowerSelfTestData(EMUN_TESTDATAPOINT Point,
                              const ushort &us_SequenceNumber,
                              QList<short> &list_SelfVoltage);

    void GetFWBootData(EMUN_TESTDATAPOINT Point,
                       const ushort &us_SequenceNumber,
                       STRUCT_FWINFO &struct_FWInfo);

    void GetFWIntactData(EMUN_TESTDATAPOINT Point,
                         const ushort &us_SequenceNumber,
                         ENUM_MSGDEFINE &FWIntact);

    void GetHandDataBagData(EMUN_TESTDATAPOINT Point,
                            const ushort &us_SequenceNumber,
                            STRUCT_HANDBAG &struct_HandBag);

    void GetStartTestNoticeData(EMUN_TESTDATAPOINT Point,
                                const ushort &us_SequenceNumber,
                                char &c_TestType,
                                char &c_TotalGroup,
                                char &c_AmoutOfAGroup,
                                QList<char> &list_UsefulDUT);

    void GetCompleteTestData(EMUN_TESTDATAPOINT Point,
                             const ushort &us_SequenceNumber,
                             QList<char> &list_UsefulResult);

    void GetStartOneGroupPowerTestData(EMUN_TESTDATAPOINT Point,
                                       const ushort &us_SequenceNumber,
                                       char &c_SurplusGroup,
                                       int &n_TestDUTMask,
                                       QList<int> &list_MaskCode);

    void GetStartOneGroupEnumTestData(EMUN_TESTDATAPOINT Point,
                                      const ushort &us_SequenceNumber,
                                      char &c_SurplusGroup,
                                      int &n_TestDUTMask,
                                      QList<int> &list_MaskCode);

    void GetUploadRFPowerResultData(EMUN_TESTDATAPOINT Point,
                                    const ushort &us_SequenceNumber,
                                    QList<short> &list_Power_db);

    void ClearFWTestData(EMUN_TESTDATAPOINT Point,
                         const ushort &us_SequenceNumber);

public:
    void GetBoxOperator(const ushort &us_SequenceNumber,
                        BOX_OPERATOR &box_Operator);


private:
    void InitComDiscover();

    void InitTcpServer();

    void WorkSleep(ushort un_Msec);

    bool GetFWDataPointer(const ushort &us_SequenceNumber,
                          FWUploadData * &p_FWUploadData);

    bool GetLastFWDataPointer(const ushort &us_SequenceNumber,
                                FWUploadData * &p_FWUploadData);

    bool FWConfirm(STRUCT_COMINFO &struct_ComInfo);

    bool AddFW(const QString &str_PortName,
               const uint &un_Pid,
               const uint &un_Vid);

    bool AddFWUploadDataObject(const ushort &us_SequenceNumber,
                               const QString &str_PortName);

    bool RemoveFW(const QString &str_PortName,
                  const uint &un_Pid,
                  const uint &un_Vid);

    bool RemoveFWUploadDataObject(const ushort &us_SequenceNumber,
                                  const QString &str_PortName);
    bool RemoveAllFWAndFWData();

    bool UpdataLastFWTestData(const ushort &us_SequenceNumber);

    bool TransformMaskCode(const ushort &us_SequenceNumber,
                           const int &n_MaskData,
                           QList<int> &list_MaskCode);

    bool ReplaceUsbControlResult(const ushort &us_SequenceNumber,
                                 QMap<int,bool> &map_FromResultData,
                                 QMap<int,bool> &map_ToResultData);

    bool AddBox(const QString &str_IP);

    bool RemoveBox(const QString &str_IP);

    bool AddCatchRobot(const QString &str_IP);

    bool RemoveCatchRobot(const QString &str_IP);

    bool AddSupplementRobot(const QString &str_IP);

    bool RemoveSupplementRobot(const QString &str_IP);

signals:
    void sig_FirmwareDiscoverd();

    void sig_FirmwareRemove();

signals:
    void sig_BoxDiscoverd();

    void sig_CatchRobotDiscoverd();

    void sig_SupplementRobotDiscoverd();

    void sig_BoxRemove();

    void sig_CatchRobotRemove();

    void sig_SupplementRobotRemove();

signals:
    void sig_BoxOperatorUpdata(ushort us_SequenceNumber);

signals:
    void sig_CatchRobotGetActionUpdata(ushort us_SequenceNumber);

signals:
    void sig_SupplementRobotGetRequestUpdata(ushort us_SequenceNumber);

signals:
    void sig_EnumUsbComplete();

    void sig_SendPowerTestComplete();

signals:
    void sig_ReceiveCmd(ushort us_SequenceNumber,
                        uchar uc_Command);

    void sig_WriteCmd(ushort us_SequenceNumber,
                      uchar uc_Command);

    void sig_GetFWInfo(ushort us_SequenceNumber);

    void sig_TestTimeOut(ushort us_SequenceNumber);

    void sig_MachineState(ushort us_SequenceNumber);

    void sig_AdjustPower(ushort us_SequenceNumber);

    void sig_ReadPower(ushort us_SequenceNumber);

    void sig_PowerSelfTest(ushort us_SequenceNumber);

    void sig_FWBoot(ushort us_SequenceNumber);

    void sig_FWIntact(ushort us_SequenceNumber);

    void sig_HandDataBag(ushort us_SequenceNumber);

    void sig_StartTestNotice(ushort us_SequenceNumber);

    void sig_CompleteTest(ushort us_SequenceNumber);

    void sig_StartOneGroupPowerTest(ushort us_SequenceNumber);

    void sig_StartOneGroupEnumTest(ushort us_SequenceNumber);

    void sig_UploadRFPowerResult(ushort us_SequenceNumber);

public slots:
    void slot_FirmwareDiscoverd(QString str_Port,
                                uint un_Pid,
                                uint un_Vid);

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
    void slot_BoxOperator(ushort us_FWStation,
                          BOX_OPERATOR box_Operator);

public slots:
    void slot_CatchRobotGetAction(ushort us_FWStation,
                                  QString str_Action);

public slots:
    void slot_SupplementRobotGetRequest(ushort us_FWStation,
                                        QString str_Request);

public slots:
    void slot_EnumUsbComplete();

    void slot_SendPowerTestComplete();

public slots:
    void slot_ReceiveCommand(ushort us_SequenceNumber,
                             uchar uc_Command,
                             QByteArray byte_Data,
                             uint un_DataLength);

    void slot_WriteCommand(ushort us_SequenceNumber,
                           uchar uc_Command,
                           QByteArray byte_Data,
                           uint un_DataLength);

    void slot_GetFWInfo(ushort us_SequenceNumber,
                        STRUCT_FWINFO struct_FWInfo);

    void slot_TestTimeOut(ushort us_SequenceNumber,
                          int n_TestTimeOut);

    void slot_MachineState(ushort us_SequenceNumber,
                           ENUM_MACHINEDUTSTATE DUTState,
                           ENUM_MACHINETESTSTATE TestState);

    void slot_AdjustPower(ushort us_SequenceNumber,
                          short s_Vslop,
                          short s_VslopAdc,
                          short s_Intercept);

    void slot_ReadPower(ushort us_SequenceNumber,
                        QList<short> list_Vslop,
                        QList<short> list_VslopAdc,
                        QList<short> list_Intercept,
                        QList<char> list_db_Compensation,
                        short s_ReferenceVoltage_1,
                        short s_ReferenceVoltage_2);

    void slot_PowerSelfTest(ushort us_SequenceNumber,
                            QList<short> list_SelfVoltage);

    void slot_FWBoot(ushort us_SequenceNumber,
                     STRUCT_FWINFO struct_FWInfo);

    void slot_FWIntact(ushort us_SequenceNumber,
                       ENUM_MSGDEFINE FWIntact);

    void slot_HandDataBag(ushort us_SequenceNumber,
                          STRUCT_HANDBAG struct_HandBag);

    void slot_StartTestNotice(ushort us_SequenceNumber,
                              char c_TestType,
                              char c_TotalGroup,
                              char c_AmoutOfAGroup,
                              QList<char> list_UsefulDUT);

    void slot_CompleteTest(ushort us_SequenceNumber,
                           QList<char> list_UsefulResult);

    void slot_StartOneGroupPowerTest(ushort us_SequenceNumber,
                                     char c_SurplusGroup,
                                     int n_TestDUTMask);

    void slot_StartOneGroupEnumTest(ushort us_SequenceNumber,
                                    char c_SurplusGroup,
                                    int n_TestDUTMask);

    void slot_UploadRFPowerResult(ushort us_SequenceNumber,
                                  QList<short> list_Power_db);




private slots:

private:
    ComDiscoverd *m_pComDiscoverd;
    QList<Firmware *> m_listFirmware;
    UsbControl *m_pUsbControl;
    QThread *m_pQThread;

    TcpServerInstanceGetter m_oTcpServerInstanceGetter;
    TcpServer *m_pTcpServer;

    QList<STRUCT_COMINFO> m_listNeedFW;
    QList<STRUCT_COMINFO> m_listFindCom;

    QList<FWUploadData *> m_listFWUploadData;
    QMap<int,bool> m_mapEnumResult;
    QMap<int,bool> m_mapOpenDeviceResult;
    QMap<int,bool> m_mapSendCmdResult;

    QList<FWUploadData *> m_listLastFWUploadData;
    QMap<int,bool> m_mapLastEnumResult;
    QMap<int,bool> m_mapLastOpenDeviceResult;
    QMap<int,bool> m_mapLastSendCmdResult;

    ENUM_OPENMODEL m_OpenFWModel;

    bool m_bManuallyCreatUsbControl;

    QList<STRUCT_COMMANDANDDATA> m_listReceiveCommandAndData;
    QList<STRUCT_COMMANDANDDATA> m_listWriteCommandAndData;

    QString m_strLocalTcpServerIP;
    ushort m_usLocalTcpServerPort;

    QList<QString> m_listBoxIP;
    QList<Box *> m_listBox;

    QString m_strCatchRobotIP;
    CatchRobot *m_pCatchRobot;

    QString m_strSupplementRobotIP;
    SupplementRoobot *m_pSupplementRoobot;

    QMap<ushort, BOX_OPERATOR> m_mapBoxOperator;
    QMap<ushort, QString> m_mapCatchRobotAction;
    QMap<ushort, QString> m_mapSupplementRobotRequest;

    QList<ushort> m_listSequenceNumber;
};

#endif // DEVICEOBSERVER_H
