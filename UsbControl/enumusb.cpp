﻿#include "enumusb.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QDebug>

EnumUsb::EnumUsb(QObject *parent)
    :QObject(parent)
{
    m_pUsbOperator = NULL;
    m_pUsbOperator = new UsbOperator;

    connect(m_pUsbOperator, SIGNAL(sig_RunComplete()),
            this, SLOT(slot_EnumComplete()));
}

EnumUsb::~EnumUsb()
{
    qDebug()<<"~EnumUsb()";

    if(m_pUsbOperator != NULL){
        m_pUsbOperator->exit();
        WaitForUsbOperatorFinish();
        delete m_pUsbOperator;
        m_pUsbOperator = NULL;
    }
}

bool EnumUsb::InitEnumUsb()
{
    return m_pUsbOperator->InitUsbOperator();
}

bool EnumUsb::CloseEmumUsb()
{
    return m_pUsbOperator->ExitUsbOperator();
}

bool EnumUsb::GetDevice(QMap<int, libusb_device *> &map_LDevice)
{
    return m_pUsbOperator->GetDevice(map_LDevice);
}

bool EnumUsb::SetEnumUsbConfig(const uint &un_Pid,
                               const uint &un_Vid,
                               const int &n_DeviceNumber,
                               const int &n_Time,
                               const QMap<int,int> &map_StationPort)
{
    if(!m_mapStationPort.isEmpty()){
        m_mapStationPort.clear();
    }

    m_mapStationPort = map_StationPort;
    return m_pUsbOperator->SetFindUsbDeviceConfig(un_Pid,
                                                  un_Vid,
                                                  n_DeviceNumber,
                                                  n_Time);
}

bool EnumUsb::StartNewEnumUsb()
{
    qDebug()<<"StartNewEnumUsb";
    m_pUsbOperator->StartNewOperator();
    m_pUsbOperator->start();
    return true;
}

bool EnumUsb::GetEnumResult(QMap<int, bool> &map_EnumResult)
{
    //map_EnumResult <station, result>
    //m_mapStationPort <station, port>

    QList<int> list_DevicePort;
    m_pUsbOperator->GetDevicePort(list_DevicePort);
//    qDebug()<<list_DevicePort;
    qDebug()<<"m_mapStationPort"<<m_mapStationPort;

    if(/*list_DevicePort.isEmpty() || */m_mapStationPort.isEmpty()){
        return false;
    }

    QMapIterator<int, int> map_IteratorStationPort(m_mapStationPort);
    while(map_IteratorStationPort.hasNext()){
        map_IteratorStationPort.next();

        map_EnumResult.insert(map_IteratorStationPort.key(), false);
        foreach(int n_DevicePort, list_DevicePort){
            if(n_DevicePort == map_IteratorStationPort.value()){
                map_EnumResult.insert(map_IteratorStationPort.key(), true);
            }
        }
    }

    qDebug()<<"map_EnumResult"<<map_EnumResult;
    return true;
}

void EnumUsb::WorkSleep(ushort un_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(un_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

bool EnumUsb::EnumComplete()
{
    //no more thing to do
    qDebug()<<"EnumComplete";
    m_pUsbOperator->exit();
    emit sig_EnumComplete();
    return true;
}

void EnumUsb::WaitForUsbOperatorFinish()
{
    if(m_pUsbOperator == NULL){
        return;
    }

    while(!m_pUsbOperator->isFinished()){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void EnumUsb::slot_EnumComplete()
{
    EnumComplete();
}
