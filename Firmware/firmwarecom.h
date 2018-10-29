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

    bool InitFirmwareCom();
    bool ExitFirmwareCom();

    bool SetFirmwareComConfig(const QString &str_Com,
                              const uint &un_Pid,
                              const uint &un_Vid);

    bool GetFirmwareComComfig(QString &str_Com,
                              uint &un_Pid,
                              uint &un_Vid);

    bool SetOpenComModelByCom();
    bool SetOpenComModelByPidVid();

    bool OpenFirmwareCom();
    bool CloseFirmwareCom();

    bool WriteCommandData(char *p_cCommand,
                          char *p_cData,
                          uint &un_DataLength);

private:
    bool OpenCom(const QString &str_ComName);

    bool ByteArrayTrasfer(QByteArray byte_Data, char *p_cData);

    bool CheckSum_CRC(char *p_cData,
                      uint16_t &n_DataLenght,
                      uint16_t &us_CRC);

    bool HandleBagData(QByteArray &byte_BagData);
    bool HandleCacheData(QByteArray &byte_Cache);

    bool SendCommand(char c_Command,
                     char *p_cData,
                     uint &un_DataLength);

    void WorkSleep(ushort un_Msec);

signals:
    void sig_ReceiveCommand(char c_Command,
                            QByteArray byte_Data,
                            uint un_DataLength);

    void sig_WriteCommand(char c_Command,
                          QByteArray byte_Data,
                          uint un_DataLength);

public slots:

private slots:
    void slot_ReadData();

private:
    QSerialPort *m_pQSerialPort;
    QThread *m_pQThread;

    ENUM_OPENMODEL m_OpenModel;
    QString m_strCom;
    uint m_unPid;
    uint m_unVid;

    QByteArray m_byteCache;
};

#endif // FIRMWARECOM_H
