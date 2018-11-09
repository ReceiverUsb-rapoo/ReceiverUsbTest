#include "firmwarecom.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QThread>
#include <QSerialPortInfo>
#include <QDebug>


#define USEDATAPOINT 1  //select char of point or char of array

FirmwareCom::FirmwareCom(QObject *parent)
    :QObject(parent)
{
    m_OpenModel = BYCOM;
    m_strCom = "";
    m_unPid = 0;
    m_unVid = 0;

    m_pQSerialPort = NULL;
    m_pQThread = NULL;

//    m_pQThread = new QThread;
//    this->moveToThread(m_pQThread);
//    m_pQThread->start();
}

FirmwareCom::~FirmwareCom()
{
    if(m_pQThread != NULL){
        m_pQThread->deleteLater();
        m_pQThread = NULL;
    }

    ExitFirmwareCom();
}

bool FirmwareCom::InitFirmwareCom()
{
    m_pQSerialPort = new QSerialPort;
    m_pQSerialPort->setBaudRate(QSerialPort::Baud115200);
    m_pQSerialPort->setDataBits(QSerialPort::Data8);
    m_pQSerialPort->setParity(QSerialPort::NoParity);
    m_pQSerialPort->setStopBits(QSerialPort::OneStop);
    m_pQSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(m_pQSerialPort, SIGNAL(readyRead()),
            this, SLOT(slot_ReadData()));

    return true;
}

bool FirmwareCom::ExitFirmwareCom()
{
    if(m_pQSerialPort != NULL){
        if(m_pQSerialPort->isOpen()){
            m_pQSerialPort->close();
        }
        delete m_pQSerialPort;
        m_pQSerialPort = NULL;
    }
    return true;
}

bool FirmwareCom::SetFirmwareComConfig(const QString &str_Com,
                                       const uint &un_Pid,
                                       const uint &un_Vid)
{
    m_strCom = str_Com;
    m_unPid = un_Pid;
    m_unVid = un_Vid;
    return true;
}

bool FirmwareCom::GetFirmwareComComfig(QString &str_Com, uint &un_Pid, uint &un_Vid)
{
    str_Com = m_strCom;
    un_Pid = m_unPid;
    un_Vid = m_unVid;
    return true;
}

bool FirmwareCom::SetOpenComModelByCom()
{
   m_OpenModel = BYCOM;
   return true;
}

bool FirmwareCom::SetOpenComModelByPidVid()
{
   m_OpenModel = BYPIDVID;
   return true;
}

bool FirmwareCom::OpenFirmwareCom()
{
//    QList<QSerialPortInfo> list_PortInfo = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo PortInfo, QSerialPortInfo::availablePorts()){
        if(m_OpenModel == BYCOM){
            if(PortInfo.portName() == m_strCom){
                return OpenCom(PortInfo.portName());
            }
        }
        else if(m_OpenModel == BYPIDVID){
            if(PortInfo.productIdentifier() == (ushort)m_unPid &&
                    PortInfo.vendorIdentifier() == (ushort)m_unVid){
                return OpenCom(PortInfo.portName());
            }
        }
    }
    return false;
}

bool FirmwareCom::CloseFirmwareCom()
{
    if(m_pQSerialPort->isOpen()){
        m_pQSerialPort->close();
    }
    return true;
}

bool FirmwareCom::WriteCommandData(char *p_cCommand,
                                   char *p_cData,
                                   uint &un_DataLength)
{
    char cWRITEDATA[255] = {};
    for(int i = 0; i < DOWN_HEADER_LENGHT; i++){
        cWRITEDATA[i] = DOWN_HEADER[i];
    }

    uint16_t us_FrameLength = COMMAND_LENGHT + un_DataLength + CHECKSUM_LENGHT;
    memcpy(cWRITEDATA + 4, &us_FrameLength, 2);
    memcpy(cWRITEDATA + 6, p_cCommand, 1);
    memcpy(cWRITEDATA + 7, p_cData, un_DataLength);

    char CHECKSUMDATA[243] = {};
    uint16_t n_CheckLength = 2 + 1 + un_DataLength;
    uint16_t un_CheckSum = 0;

    memcpy(CHECKSUMDATA, cWRITEDATA + 4, n_CheckLength);
    CheckSum_CRC(CHECKSUMDATA, n_CheckLength, un_CheckSum);

    memcpy(cWRITEDATA + 7 + un_DataLength, &un_CheckSum, 2);
    memcpy(cWRITEDATA + 9 + un_DataLength, FRAMETAIL, 2);

    if(!m_pQSerialPort->write(cWRITEDATA, 11 + un_DataLength)){
        return false;
    }

    QString str_Info = "write ";
    for(int i = 0; i < (int)(un_DataLength + 11); i++){
        str_Info += " " + QString::number((unsigned char)cWRITEDATA[i], 16);
    }
    qDebug()<<str_Info;

    qDebug()<<"";

    QByteArray byte_Data = QByteArray(p_cData, un_DataLength);
    emit sig_WriteCommand(p_cCommand[0], byte_Data, un_DataLength);

    return true;
}

bool FirmwareCom::OpenCom(const QString &str_ComName)
{
    m_pQSerialPort->setPortName(str_ComName);
    if(m_pQSerialPort->open(QIODevice::ReadWrite)){
        return true;
    }
    else{
        return false;
    }
}

bool FirmwareCom::ByteArrayTrasfer(QByteArray byte_Data, char *p_cData)
{
    for(int i = 0; i < byte_Data.size(); i++){
        p_cData[i] = byte_Data.at(i);
    }
    return true;
}

bool FirmwareCom::CheckSum_CRC(char *p_cData,
                               uint16_t &n_DataLenght,
                               uint16_t &us_CRC)
{
    us_CRC = 0;
    for(int i = 0; i < n_DataLenght; i++){
        us_CRC += (uint8_t)p_cData[i];
    }
    return true;
}

bool FirmwareCom::HandleBagData(QByteArray &byte_BagData)
{
    //check shortestlength
    if(byte_BagData.size() < BAGDATA_SHORTEST){
        m_byteCache.append(byte_BagData);
        byte_BagData.clear();
        return false;
    }

    //check data length
    char FRAMELENGHT[2];
    ByteArrayTrasfer(byte_BagData.mid(4, 2), FRAMELENGHT);
    uint16_t us_FrameLenght;
    memcpy(&us_FrameLenght, FRAMELENGHT, 2);

    if(byte_BagData.size() <
            (BAGDATA_SHORTEST + us_FrameLenght - COMMAND_LENGHT - CHECKSUM_LENGHT)){
        m_byteCache.append(byte_BagData);
        byte_BagData.clear();
        return false;
    }

    int n_BagDataLength = UP_HEADER_LENGHT + FRAME_LENGHT + us_FrameLenght + FRAMETAIL_LENGHT;

    //check frame tail
    if(byte_BagData.at(UP_HEADER_LENGHT + FRAME_LENGHT + us_FrameLenght) != FRAMETAIL[0] ||
            byte_BagData.at(UP_HEADER_LENGHT + FRAME_LENGHT + us_FrameLenght + 1) != FRAMETAIL[1]){
        byte_BagData.remove(0, n_BagDataLength);
        return false;
    }

    uint16_t us_CRC1 = 0;
#ifndef USEDATAPOINT
    char DATABAG[DATA_MAX_LENGHT];
    ByteArrayTrasfer(byte_BagData.mid(UP_HEADER_LENGHT + FRAME_LENGHT, us_FrameLenght), DATABAG);
    CheckSum_CRC(DATABAG, us_FrameLenght, us_CRC1);
#else
    char *p_DataBag = byte_BagData.mid(UP_HEADER_LENGHT, us_FrameLenght).data();
    CheckSum_CRC(p_DataBag, us_FrameLenght, us_CRC1);
#endif

    char CRC1[CHECKSUM_LENGHT] = {};
    memcpy(CRC1, &us_CRC1, 2);

    char CRC2[CHECKSUM_LENGHT];
    ByteArrayTrasfer(byte_BagData.mid(4 + 2 + us_FrameLenght - 2, 2), CRC2);

    //check CRC
    if(CRC1[0] != CRC2[0] && CRC1[1] != CRC2[1]){
        byte_BagData.remove(0, n_BagDataLength);
        return false;
    }

    //send cmd + data + length
    uint n_DataLenght = us_FrameLenght - COMMAND_LENGHT - CHECKSUM_LENGHT;
#ifndef USEDATAPOINT
    char COMMAND[2];
    char DATA[DATA_MAX_LENGHT];
    ByteArrayTrasfer(byte_BagData.mid(4 + 2, 1), COMMAND);
    ByteArrayTrasfer(byte_BagData.mid(4 + 2 + 2, us_FrameLenght - 3), DATA);
    SendCommand(COMMAND[0], DATA, n_DataLenght);
#else
    char c_Command = byte_BagData.mid(4 + 2, 1).at(0);
    char *p_Data = byte_BagData.mid(4 + 2 + 1, us_FrameLenght - 3).data();
    SendCommand(c_Command, p_Data, n_DataLenght);
#endif

    QString str_Info1 = "Receive ";
    for(int i = 0; i < byte_BagData.length(); i++){
        str_Info1 += " " + QString::number((unsigned char)byte_BagData.at(i), 16);
    }
    qDebug()<<str_Info1;
    qDebug()<<"";

    byte_BagData.remove(0, n_BagDataLength);

    return true;
}

bool FirmwareCom::HandleCacheData(QByteArray &byte_Cache)
{
    int n_HeaderIndex = byte_Cache.indexOf(UP_HEADER);

    if(n_HeaderIndex == -1 && byte_Cache.size() > BAGDATA_SHORTEST){
        byte_Cache.clear();
        return false;
    }
    else if(n_HeaderIndex != -1 && n_HeaderIndex != 0){
        byte_Cache.remove(0, n_HeaderIndex);
    }

    //check shortest length
    if(byte_Cache.size() < BAGDATA_SHORTEST){
        return false;
    }

    char FRAMELENGHT[2];
    ByteArrayTrasfer(byte_Cache.mid(4,2), FRAMELENGHT);
    uint16_t us_FrameLenght;
    memcpy(&us_FrameLenght, FRAMELENGHT, 2);

    //check data length
    if(byte_Cache.size() <
            (BAGDATA_SHORTEST + us_FrameLenght - COMMAND_LENGHT - CHECKSUM_LENGHT)){
        return false;
    }

    int n_BagDataLength = UP_HEADER_LENGHT + FRAME_LENGHT + us_FrameLenght + FRAMETAIL_LENGHT;

    //check frame tail
    if(byte_Cache.at(UP_HEADER_LENGHT + FRAME_LENGHT + us_FrameLenght) != FRAMETAIL[0] ||
            byte_Cache.at(UP_HEADER_LENGHT + FRAME_LENGHT + us_FrameLenght + 1) != FRAMETAIL[1]){
        byte_Cache.remove(0, n_BagDataLength);
        qDebug()<<"check frame tail faile";
        return false;
    }

    uint16_t us_CRC1 = 0;
#ifndef USEDATAPOINT
    char DATABAG[DATA_MAX_LENGHT];
    ByteArrayTrasfer(byte_Cache.mid(UP_HEADER_LENGHT + FRAME_LENGHT, us_FrameLenght), DATABAG);
    CheckSum_CRC(DATABAG, us_FrameLenght, us_CRC1);
#else
    char *p_DataBag = byte_Cache.mid(UP_HEADER_LENGHT, us_FrameLenght).data();
    CheckSum_CRC(p_DataBag, us_FrameLenght, us_CRC1);
#endif

    char CRC1[CHECKSUM_LENGHT] = {};
    memcpy(CRC1, &us_CRC1, 2);

    char CRC2[CHECKSUM_LENGHT];
    ByteArrayTrasfer(byte_Cache.mid(4 + 2 + us_FrameLenght - 2, CHECKSUM_LENGHT), CRC2);

    //check CRC
    if(CRC1[0] != CRC2[0] || CRC1[1] != CRC2[1]){
        qDebug()<<QString::number((uchar)CRC1[0], 16);
        qDebug()<<QString::number((uchar)CRC1[1], 16);
        qDebug()<<QString::number((uchar)CRC2[0], 16);
        qDebug()<<QString::number((uchar)CRC2[1], 16);

        byte_Cache.remove(0, n_BagDataLength);
        qDebug()<<"check CRC faile";
        return false;
    }

    //send cmd + data + length
    uint n_DataLenght = us_FrameLenght - COMMAND_LENGHT - CHECKSUM_LENGHT;
#ifndef USEDATAPOINT
    char COMMAND[2];
    char DATA[DATA_MAX_LENGHT];
    ByteArrayTrasfer(byte_Cache.mid(4 + 2, 1), COMMAND);
    ByteArrayTrasfer(byte_Cache.mid(4 + 2 + 2, us_FrameLenght - 3), DATA);
    SendCommand(COMMAND[0], DATA, n_DataLenght);
#else
    char c_Command = byte_Cache.mid(4 + 2, 1).at(0);
    char *p_Data = byte_Cache.mid(4 + 2 + 1, us_FrameLenght - 3).data();
    SendCommand(c_Command, p_Data, n_DataLenght);
#endif

    QString str_Info1 = "Receive ";
    for(int i = 0; i < byte_Cache.length(); i++){
        str_Info1 += " " + QString::number((unsigned char)byte_Cache.at(i), 16);
    }
    qDebug()<<str_Info1;
    qDebug()<<"";


    byte_Cache.remove(0, n_BagDataLength);
    return true;
}

bool FirmwareCom::SendCommand(char c_Command,
                              char *p_cData,
                              uint &un_DataLength)
{    
    QByteArray byte_Data = QByteArray(p_cData, un_DataLength);
    emit sig_ReceiveCommand(c_Command, byte_Data, un_DataLength);
    return true;
}

void FirmwareCom::WorkSleep(ushort un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void FirmwareCom::slot_ReadData()
{
    if(m_pQSerialPort->bytesAvailable() != 0 &&
            m_pQSerialPort->bytesAvailable() != -1){
        QByteArray byte_ReadData = m_pQSerialPort->readAll();

//        QString str_Info = "Read ";
//        for(int i = 0; i < byte_ReadData.length(); i++){
//            str_Info += " " + QString::number((unsigned char)byte_ReadData.at(i), 16);
//        }
//        qDebug()<<str_Info;
//        qDebug()<<"";

        int n_DataLength = byte_ReadData.length();
        if(n_DataLength > MAX_LENGHT){
            return;
        }

        while(!byte_ReadData.isEmpty()){
            int n_HeaderIndex = byte_ReadData.indexOf(UP_HEADER);

            if(n_HeaderIndex == 0){
                HandleBagData(byte_ReadData);
            }
            else if(n_HeaderIndex == -1){
                m_byteCache.append(byte_ReadData);
                byte_ReadData.clear();
                HandleCacheData(m_byteCache);
            }
            else{
                m_byteCache.append(byte_ReadData.left(n_HeaderIndex));
                byte_ReadData.remove(0, n_HeaderIndex);
                HandleCacheData(m_byteCache);
            }
        }

//        QString str_Info1 = "Cache ";
//        for(int i = 0; i < m_byteCache.length(); i++){
//            str_Info1 += " " + QString::number((unsigned char)m_byteCache.at(i), 16);
//        }
//        qDebug()<<str_Info1;
//        qDebug()<<"";
    }
}
