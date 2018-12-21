#ifndef TESTDATAFILE_H
#define TESTDATAFILE_H
#include "datafile_define.h"

class TestDataFile
{
public:
    //获取记录测试数据 测试次数 测试成功次数 测试失败次数 重测次数 测试总数
    bool GetTestRecordData(int &n_TestTotal,
                           int &n_RetestTotal,
                           int &n_PassTotal,
                           int &n_FaileTotal,
                           int &n_TestNumber);
    //保存记录测试数据 测试次数 测试成功次数 测试失败次数 重测次数 测试总数
    bool SaveTestRecordData(const int &n_TestTotal,
                            const int &n_RetestTotal,
                            const int &n_PassTotal,
                            const int &n_FaileTotal,
                            const int &n_TestNumber);
};

#endif // TESTDATAFILE_H
