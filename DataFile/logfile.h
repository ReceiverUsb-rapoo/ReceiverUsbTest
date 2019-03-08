#ifndef LOGFILE_H
#define LOGFILE_H
#include <QString>
#include "datafile_define.h"

class LogFile
{
public:
    //log保存
    static QString Addlog(const QString &str_Info);
};

#endif // LOGFILE_H
