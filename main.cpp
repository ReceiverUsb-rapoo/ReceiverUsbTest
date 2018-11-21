#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    qRegisterMetaType<STRUCT_PCCOMMANDDATA>("STRUCT_PCCOMMANDDATA");
//    qRegisterMetaType<STRUCT_COMINFO>("STRUCT_COMINFO");
//    qRegisterMetaType<STRUCT_FWINFO>("STRUCT_FWINFO");
//    qRegisterMetaType<STRUCT_HANDBAG>("STRUCT_HANDBAG");
//    qRegisterMetaType<ENUM_MACHINEDUTSTATE>("ENUM_MACHINEDUTSTATE");
//    qRegisterMetaType<ENUM_MACHINETESTSTATE>("ENUM_MACHINETESTSTATE");
//    qRegisterMetaType<ENUM_MSGDEFINE>("ENUM_MSGDEFINE");

//    QFile o_QFile(":/myStyle.qss");
//    o_QFile.open(QFile::ReadOnly);
//    qApp->setStyleSheet(o_QFile.readAll());
//    o_QFile.close();

    return a.exec();
}
