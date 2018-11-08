#-------------------------------------------------
#
# Project created by QtCreator 2018-09-10T16:10:31
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReceiverUsbTest
TEMPLATE = app

DEFINES += _X86_

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    Firmware/firmware.cpp \
    UsbControl/usbcontrol.cpp \
    UsbControl/powertest.cpp \
    UsbControl/enumusb.cpp \
    Box/box.cpp \
    UI/statisticaltable.cpp \
    UI/equitmentcontrol.cpp \
    UI/firmwareconfig.cpp \
    UI/firewaredebug.cpp \
    UI/resultshow.cpp \
    UsbControl/usboperator.cpp \
    UsbControl/writecmdthread.cpp \
    Firmware/firmwarecom.cpp \
    UsbControl/powertestinstancegetter.cpp \
    Firmware/outputfirmwareconfig.cpp \
    UpperLogic/deviceobserver.cpp \
    UpperLogic/deviceoperator.cpp \
    UpperLogic/mastercontrol.cpp \
    Firmware/comdiscoverd.cpp \
    UpperLogic/deviceobserverinstancegetter.cpp \
    UpperLogic/deviceoperatorinstancegetter.cpp \
    DataFile/configfile.cpp \
    DataFile/logfile.cpp \
    UI/pointtable.cpp \
    UI/enumpointtable.cpp \
    UI/powerdebug.cpp \
    UI/countpowerlimit.cpp \
    tcp/tcpserver.cpp \
    tcp/tcpsocket.cpp \
    tcp/threadhandle.cpp \
    UpperLogic/counttestdata.cpp \
    UpperLogic/counttestresultinstancegetter.cpp \
    DataFile/tablefile.cpp \
    UI/equitmentconnectconfig.cpp \
    UI/usbcontrolconfig.cpp \
    tcp/tcpserverinstancegetter.cpp \
    Robot/catchrobot.cpp

HEADERS  += mainwindow.h \
    Firmware/firmware.h \
    UsbControl/usbcontrol.h \
    UsbControl/powertest.h \
    UsbControl/enumusb.h \
    Box/box.h \
    UI/statisticaltable.h \
    UI/equitmentcontrol.h \
    UI/firmwareconfig.h \
    UI/firewaredebug.h \
    UI/resultshow.h \
    UsbControl/usboperator.h \
    UsbControl/writecmdthread.h \
    Firmware/firmwarecom.h \
    Firmware/firmware_define.h \
    UsbControl/powertestinstancegetter.h \
    Firmware/outputfirmwareconfig.h \
    UpperLogic/deviceobserver.h \
    UpperLogic/deviceoperator.h \
    UpperLogic/mastercontrol.h \
    UpperLogic/fwuploaddata.h \
    UpperLogic/upperdefine.h \
    Firmware/comdiscoverd.h \
    UpperLogic/deviceobserverinstancegetter.h \
    UpperLogic/deviceoperatorinstancegetter.h \
    DataFile/configfile.h \
    DataFile/datafile_define.h \
    DataFile/logfile.h \
    UI/pointtable.h \
    UI/enumpointtable.h \
    UI/powerdebug.h \
    UI/countpowerlimit.h \
    UI/ui_define.h \
    tcp/tcpserver.h \
    tcp/tcpsocket.h \
    tcp/threadhandle.h \
    UpperLogic/counttestdata.h \
    UpperLogic/counttestresultinstancegetter.h \
    DataFile/tablefile.h \
    UI/equitmentconnectconfig.h \
    UI/usbcontrolconfig.h \
    tcp/tcpserverinstancegetter.h \
    Robot/catchrobot.h

FORMS    += mainwindow.ui \
    UI/firmwareconfig.ui \
    UI/firewaredebug.ui \
    UI/resultshow.ui \
    UI/pointtable.ui \
    UI/enumpointtable.ui \
    UI/powerdebug.ui \
    UI/countpowerlimit.ui \
    UI/equitmentconnectconfig.ui \
    UI/usbcontrolconfig.ui \
    UI/equitmentcontrol.ui

LIBS += -L/usr/local/lib -lusb-1.0
INCLUDEPATH +=/usr/local/include

LIBS += -L"/usr/lib/x86_64-linux-gnu" -lusb-1.0
INCLUDEPATH += /usr/include/libusb-1.0

include(qextserial/qextserialport.pri)
