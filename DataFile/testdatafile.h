#ifndef TESTDATAFILE_H
#define TESTDATAFILE_H
#include "datafile_define.h"

class TestDataFile
{
public:
    bool GetTestRecordData(int &n_TestTotal,
                           int &n_RetestTotal,
                           int &n_PassTotal,
                           int &n_FaileTotal,
                           int &n_TestNumber);

    bool SaveTestRecordData(const int &n_TestTotal,
                            const int &n_RetestTotal,
                            const int &n_PassTotal,
                            const int &n_FaileTotal,
                            const int &n_TestNumber);
};

#endif // TESTDATAFILE_H
