#include "comdiscoverd.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QList>
#include <QListIterator>

ComDiscoverd::ComDiscoverd(QObject *parent)
    :QObject(parent)
{
    qDebug()<<"enter com disc";

    m_pQTimer = NULL;
    m_pQTimer = new QTimer;
    connect(m_pQTimer,SIGNAL(timeout()),
            this, SLOT(slot_TimerOut()));
    m_pQTimer->start(5000);
}

ComDiscoverd::~ComDiscoverd()
{
    if(m_pQTimer != NULL){
        m_pQTimer->stop();
        delete m_pQTimer;
        m_pQTimer = NULL;
    }
}

void ComDiscoverd::slot_TimerOut()
{
//    qDebug()<<"ComDiscoverdTimerOut";
    bool b_DiscoverdEqual = false;
    QList<QSerialPortInfo> list_PortInfo = QSerialPortInfo::availablePorts();

    for(int i = 0; i < list_PortInfo.count(); i++){
//        qDebug()<<list_PortInfo.at(i).portName();
//        qDebug()<<list_PortInfo.at(i).productIdentifier();
//        qDebug()<<list_PortInfo.at(i).vendorIdentifier();
//        qDebug()<<list_PortInfo.at(i).description();
//        qDebug()<<list_PortInfo.at(i).serialNumber();
//        qDebug()<<list_PortInfo.at(i).systemLocation();

        for(int j = 0; j < m_listPortInfo.count(); j++){
            if(m_listPortInfo.at(j).portName() == list_PortInfo.at(i).portName() &&
                    m_listPortInfo.at(j).productIdentifier() == list_PortInfo.at(i).productIdentifier() &&
                    m_listPortInfo.at(j).vendorIdentifier() == list_PortInfo.at(i).vendorIdentifier()){
                b_DiscoverdEqual = true;
            }
            if(!b_DiscoverdEqual){

                m_listPortInfo.append(list_PortInfo.at(i));
                emit sig_ComDiscoverd(list_PortInfo.at(i).portName(),
                                      (uint)list_PortInfo.at(i).productIdentifier(),
                                      (uint)list_PortInfo.at(i).vendorIdentifier());
                b_DiscoverdEqual = false;
            }
        }

        if(m_listPortInfo.count() == 0){
            m_listPortInfo.append(list_PortInfo.at(i));
            emit sig_ComDiscoverd(list_PortInfo.at(i).portName(),
                                  (uint)list_PortInfo.at(i).productIdentifier(),
                                  (uint)list_PortInfo.at(i).vendorIdentifier());
            b_DiscoverdEqual = false;
        }
    }

    bool b_RemoveEqual = false;
    for(int i = 0; i < m_listPortInfo.count(); i++){
        for(int j = 0; j < list_PortInfo.count(); j++){
            if(list_PortInfo.at(j).portName() == m_listPortInfo.at(i).portName() &&
                    list_PortInfo.at(j).productIdentifier() == m_listPortInfo.at(i).productIdentifier() &&
                    list_PortInfo.at(j).vendorIdentifier() == m_listPortInfo.at(i).vendorIdentifier()){
                b_RemoveEqual = true;
            }
            if(!b_RemoveEqual){
                emit sig_ComRemove(m_listPortInfo.at(i).portName(),
                                   (uint)m_listPortInfo.at(i).productIdentifier(),
                                   (uint)m_listPortInfo.at(i).vendorIdentifier());

                m_listPortInfo.removeAt(i);
                b_RemoveEqual = false;
            }
        }

        if(list_PortInfo.count() == 0){
            emit sig_ComRemove(m_listPortInfo.at(i).portName(),
                               (uint)m_listPortInfo.at(i).productIdentifier(),
                               (uint)m_listPortInfo.at(i).vendorIdentifier());

            m_listPortInfo.removeAt(i);
            b_RemoveEqual = false;
        }
    }


    /*
    QString str_PortName = "";
    ushort us_PID = 0;
    ushort us_VID = 0;
    foreach(QSerialPortInfo PortInfo_1, QSerialPortInfo::availablePorts()){
        qDebug()<<PortInfo_1.portName();
        qDebug()<<PortInfo_1.productIdentifier();
        qDebug()<<PortInfo_1.vendorIdentifier();
        qDebug()<<PortInfo_1.description();
        qDebug()<<PortInfo_1.serialNumber();
        qDebug()<<PortInfo_1.systemLocation();

        str_PortName = PortInfo_1.portName();
        us_PID = PortInfo_1.productIdentifier();
        us_VID = PortInfo_1.vendorIdentifier();

        foreach(STRUCT_PORTINFO struct_PortInfo, m_listPortInfo){
            if(struct_PortInfo.str_PortName == str_PortName&&
                    struct_PortInfo.us_PID == us_PID &&
                    struct_PortInfo.us_VID == us_VID){
                b_DiscoverdEqual = true;
            }
        }

        if(!b_DiscoverdEqual){
            STRUCT_PORTINFO struct_PortInfo;
            struct_PortInfo.str_PortName = str_PortName;
            struct_PortInfo.us_PID = us_PID;
            struct_PortInfo.us_VID == us_VID;

            m_listPortInfo.append(struct_PortInfo);
            emit sig_ComDiscoverd(struct_PortInfo.str_PortName,
                                  (uint)struct_PortInfo.us_PID,
                                  (uint)struct_PortInfo.us_VID);
            b_DiscoverdEqual = false;
        }

    }

    bool b_RemoveEqual = false;
    foreach(STRUCT_PORTINFO struct_PortInfo, m_listPortInfo){
        foreach(QSerialPortInfo PortInfo_2, QSerialPortInfo::availablePorts()){
            str_PortName = PortInfo_2.portName();
            us_PID = PortInfo_2.productIdentifier();
            us_VID = PortInfo_2.vendorIdentifier();

            if(str_PortName == struct_PortInfo.str_PortName &&
                    us_PID == struct_PortInfo.us_PID&&
                    us_VID == struct_PortInfo.us_VID){
                b_RemoveEqual = true;
            }
        }

        if(!b_RemoveEqual){
            m_listPortInfo.removeAll(struct_PortInfo);
            emit sig_ComRemove(struct_PortInfo.str_PortName,
                               (uint)struct_PortInfo.us_PID,
                               (uint)struct_PortInfo.us_VID);
            b_RemoveEqual = false;
        }
    }
    */
}
