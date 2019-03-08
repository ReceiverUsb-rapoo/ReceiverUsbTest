#ifndef TESTDATAFILE_H
#define TESTDATAFILE_H
#include "datafile_define.h"

struct RECORDDATA
{
    int n_TestTotal;
    int n_RetestTotal;
    int n_PassTotal;
    int n_FaileTotal;
    int n_TestNumber;

    RECORDDATA(){
        n_TestTotal = 0;
        n_RetestTotal = 0;
        n_PassTotal = 0;
        n_FaileTotal = 0;
        n_TestNumber = 0;
    }
};

class TestDataFile
{
public:
    //获取当前记录测试数据 测试次数 测试成功次数 测试失败次数 重测次数 测试总数
    bool GetCurrentTestRecordData(RECORDDATA &CurrentData);

    //保存当前记录测试数据 测试次数 测试成功次数 测试失败次数 重测次数 测试总数
    bool SaveCurrentTestRecordData(const RECORDDATA &CurrentData);

    //获取总记录测试数据 测试次数 测试成功次数 测试失败次数 重测次数 测试总数
    bool GetTestTotalRecordData(RECORDDATA &TotalData);

    //保存总记录测试数据 测试次数 测试成功次数 测试失败次数 重测次数 测试总数
    bool SaveTestTotalRecordData(const RECORDDATA &TotalData);

    void PooledData();
};

#endif // TESTDATAFILE_H
