#ifndef FIRMWARECOM_H
#define FIRMWARECOM_H
#include <QObject>
#include <QSerialPort>
#include <QThread>
#include "firmware_define.h"

class FirmwareCom : public QObject
{
    Q_OBJECT
public:
    explicit FirmwareCom(QObject *parent = 0);
    ~FirmwareCom();

    //初始化固件com
    bool InitFirmwareCom();
    //退出固件com
    bool ExitFirmwareCom();
    //设置固件com配置
    bool SetFirmwareComConfig(const QString &str_Com,
                              const uint &un_Pid,
                              const uint &un_Vid);
    //获取固件com配置
    bool GetFirmwareComComfig(QString &str_Com,
                              uint &un_Pid,
                              uint &un_Vid);
    //设置com打开方式为com名称
    bool SetOpenComModelByCom();
    //设置com打开方式为pidvid
    bool SetOpenComModelByPidVid();
    //打开固件com
    bool OpenFirmwareCom();
    //关闭固件com
    bool CloseFirmwareCom();
    //写入数据 cmd + 数据 + 长度 
    bool WriteCommandData(char *p_cCommand,
                          char *p_cData,
                          uint &un_DataLength);

private:
    //打开com
    bool OpenCom(const QString &str_ComName);
    //数据转换 ByteArray 转 char *
    bool ByteArrayTrasfer(QByteArray byte_Data, char *p_cData);
    //CRC校验
    bool CheckSum_CRC(char *p_cData,
                      uint16_t &n_DataLenght,
                      uint16_t &us_CRC);
    //处理整包数据 
    bool HandleBagData(QByteArray &byte_BagData);
    //处理缓存数据
    bool HandleCacheData(QByteArray &byte_Cache);
    //发送数据 cmd + 数据 + 长度
    bool SendCommand(char c_Command,
                     char *p_cData,
                     uint &un_DataLength);
    //延时时间
    void WorkSleep(ushort un_Msec);

signals:
    //发送 接收cmd数据
    void sig_ReceiveCommand(char c_Command,
                            QByteArray byte_Data,
                            uint un_DataLength);
    //发送 写入cmd数据
    void sig_WriteCommand(char c_Command,
                          QByteArray byte_Data,
                          uint un_DataLength);

public slots:

private slots:
    //com 数据处理接口 
    void slot_ReadData();

private:
    QSerialPort *m_pQSerialPort;
    QThread *m_pQThread;

    ENUM_OPENMODEL m_OpenModel;     //打开com方式
    QString m_strCom;               //com 名称
    uint m_unPid;                   //com PID
    uint m_unVid;                   //com VID

    QByteArray m_byteCache;         //缓存数据
};

#endif // FIRMWARECOM_H
