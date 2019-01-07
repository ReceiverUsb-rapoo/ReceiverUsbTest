﻿#ifndef DATAFILE_DEFINE_H
#define DATAFILE_DEFINE_H
#include <QString>
#include <QMap>
#include <QList>

const QString PCConfigPath = "/Config";
const QString LogPath = "/Log";
const QString UsbControlConfigPath = "/UsbControl";
const QString EquitmentConfigPath = "/Equitment";
const QString TableDataPath = "/TableData";
const QString TestRecordDataPath = "/TestRecordData";

const QString PCConfigFileName = "_PCConfig.xml";
const QString LogFileName = "_Log.txt";
const QString UsbControlFileName = "_UsbControl.xml";
const QString EquitmentFileName = "Equitment.xml";
const QString TableDataFileName = "_TableData.csv";
const QString TestTotalRecordDataFileName = "TestTotalRecordData.xml";
const QString CurrentTestRecordDataFileName = "CurrentTestRecordDataFileName.xml";

const int OneGroupUsbNumber_DataFile = 20;

struct STRUCT_USBCONTROLCONFIG
{
    uint un_Pid;
    uint un_Vid;
    int n_Time;
    QMap<int,int> map_StationPort;

    STRUCT_USBCONTROLCONFIG(){
        un_Pid = 0;
        un_Vid = 0;
        n_Time = 0;
    }
};

struct STRUCT_EQUITMENTCONFIG{
    QList<QString> list_ComName;
    uint un_ComPid;
    uint un_ComVid;
    QString str_ComputerIP;
    ushort us_ComputerPort;
    int n_OpemWithComName;      //0 or 1
    int n_OpemWithPidVid;       //0 or 1
    int n_BoxUnuse;              //0 or 1
    int n_RobotUnuse;            //0 or 1
    QList<QString> list_BoxIP;
    QList<QString> list_RobotIP;

    STRUCT_EQUITMENTCONFIG(){
        un_ComPid = 0;
        un_ComVid = 0;
        str_ComputerIP = "";
        us_ComputerPort = 0;
        n_OpemWithComName = 0;
        n_OpemWithPidVid = 0;
        n_BoxUnuse = 0;
        n_RobotUnuse = 0;
    }
};

#endif // DATAFILE_DEFINE_H
