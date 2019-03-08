#ifndef TABLEFILE_H
#define TABLEFILE_H
#include <QString>
#include <QTableWidget>
#include "datafile_define.h"

class TableFile
{
public:
    //保存数据表格数据 不可选位置
    bool SaveTableData_FixeAddress(const ushort &us_SequenceNumber,
                                   QTableWidget *p_QTableWidget);
    //保存数据表格数据 自动保存位置
    bool SaveTableData_SelectAddress(const ushort &us_SequenceNumber,
                                     QTableWidget *p_QTableWidget);
    //保存数据表格数据 log保存
    bool SaveTableData_Log(const ushort &us_SequenceNumber,
                           QTableWidget *p_QTableWidget);

private:
    //保存数据表格
    bool SaveTableData(const ushort &us_SequenceNumber,
                       const QString &str_FilePath,
                       QTableWidget *p_QTableWidget);
};

#endif // TABLEFILE_H
