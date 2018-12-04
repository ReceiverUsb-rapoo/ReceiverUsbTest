#include "counttestdata.h"

CountTestData::CountTestData()
{
    m_nTestTotal = 0;
    m_nRetestTotal = 0;
    m_nPassTotal = 0;
    m_nFaileTotal = 0;
    m_nTestNumber = 0;
}

CountTestData::~CountTestData()
{

}

void CountTestData::GetTestTotal(int &n_TestTotal,
                                 int &n_RetestTotal,
                                 int &n_PassTotal,
                                 int &n_FaileTotal,
                                 int &n_TestNumber)
{
    n_TestTotal = m_nTestTotal;
    n_RetestTotal = m_nRetestTotal;
    n_PassTotal = m_nPassTotal;
    n_FaileTotal =  m_nFaileTotal;
    n_TestNumber = m_nTestNumber;
}

void CountTestData::ClearTestTotal()
{
    m_nTestTotal = 0;
    m_nRetestTotal = 0;
    m_nPassTotal = 0;
    m_nFaileTotal =  0;
    m_nTestNumber = 0;
}

void CountTestData::SetRetest(const ushort &us_SequenceNumber,
                              const bool &b_Retest)
{
    m_mapRetest.insert(us_SequenceNumber, b_Retest);
}

void CountTestData::SetRFPowerDBLimit(const ushort &us_SequenceNumber,
                                      const QList<int> &list_RFPowerDBUpperLimit,
                                      const QList<int> &list_RFPowerDBLowerLimit)
{
    m_mapRFPowerDBLowerLimit.insert(us_SequenceNumber,list_RFPowerDBLowerLimit);
    m_mapRFPowerDBUpperLimit.insert(us_SequenceNumber,list_RFPowerDBUpperLimit);
}

void CountTestData::SetDUTFWPositions(const ushort &us_SequenceNumber,
                                      const QList<int> &list_DUTFWPosition)
{
    m_mapDUTFWPosition.insert(us_SequenceNumber, list_DUTFWPosition);
}

bool CountTestData::CountOneTestData(const ushort &us_SequenceNumber,
                                     const uint &un_SequenceTimes,
                                     QMap<int, bool> &map_EnumResult,
                                     QMap<int, bool> &map_OpenDeviceResult,
                                     QMap<int, bool> &map_SendCmdResult,
                                     QList<short> &list_Power_db)
{
    m_nTestTotal ++;

    if(map_EnumResult.count() != OneGroupUsbNumber_CountData ||
            map_OpenDeviceResult.count() != OneGroupUsbNumber_CountData ||
            map_SendCmdResult.count() != OneGroupUsbNumber_CountData ||
            list_Power_db.count() != 32){
        return false;
    }

    if(m_mapDUTFWPosition.value(us_SequenceNumber).count() != 32 ||
            m_mapRFPowerDBLowerLimit.value(us_SequenceNumber).count() != 32 ||
            m_mapRFPowerDBUpperLimit.value(us_SequenceNumber).count() != 32){
        return false;
    }


    uint un_EnumError = 0;
    uint un_OpenDeviceError = 0;
    uint un_SendCmdError = 0;
    uint un_PowerTestError = 0;

    QString str_ErrorString = "";
    QList<QString> list_ErrorString;

    bool b_Result = true;
    QList<bool> list_Result;

    for(int i = 1; i <= OneGroupUsbNumber_CountData; i++){
        if(map_EnumResult.value(i) == false){
            un_EnumError ++;
            str_ErrorString += "枚举异常\n";
            b_Result = false;
        }

        if(map_OpenDeviceResult.value(i) == false){
            un_OpenDeviceError ++;
            str_ErrorString += "打开设备异常\n";
            b_Result = false;
        }

        if(map_SendCmdResult.value(i) == false){
            un_SendCmdError ++;
            str_ErrorString += "发送命令异常\n";
            b_Result = false;
        }

        list_ErrorString.append(str_ErrorString);
        str_ErrorString = "";
        list_Result.append(b_Result);
        b_Result = true;
    }

    int n_OneRang = 0;
    for(int i = 0; i < 32; i++){
        if(m_mapDUTFWPosition.value(us_SequenceNumber).at(i) == 0){
            continue;
        }

        if(list_Power_db.at(i) <= m_mapRFPowerDBLowerLimit.value(us_SequenceNumber).at(i) ||
                list_Power_db.at(i) >= m_mapRFPowerDBUpperLimit.value(us_SequenceNumber).at(i)){
            un_PowerTestError ++;
            str_ErrorString = list_ErrorString.at(n_OneRang);
            str_ErrorString += "RF功率异常\n";
            list_ErrorString.replace(n_OneRang, str_ErrorString);
            str_ErrorString = "";

            list_Result.replace(n_OneRang, false);
        }
        n_OneRang ++;
    }

    if(m_mapRetest.value(us_SequenceNumber)){
        if(list_Result.contains(false)){
            m_mapResultString.insert(us_SequenceNumber, list_ErrorString);
            m_mapResult.insert(us_SequenceNumber, list_Result);
            return true;
        }
    }

    QMap<ENUM_RESULTTYPE, uint> map_TypeError;
    QMap<uint, QMap<ENUM_RESULTTYPE, uint>> map_STTypeError;

    if(m_mapErrorAmount.contains(us_SequenceNumber)){
        if(m_mapErrorAmount.value(us_SequenceNumber).contains(un_SequenceTimes)){
            QMap<ENUM_RESULTTYPE, uint> map_TypeError_1;
            map_TypeError_1 = m_mapErrorAmount.value(us_SequenceNumber).value(un_SequenceTimes);

            un_EnumError += map_TypeError_1.value(ENUM_RESULT);
            un_OpenDeviceError += map_TypeError_1.value(OPENDEVICE_RESULT);
            un_SendCmdError += map_TypeError_1.value(SENDCMD_RESULT);
            un_PowerTestError += map_TypeError_1.value(POWERTEST_REULST);
        }
    }

    map_TypeError.insert(ENUM_RESULT, un_EnumError);
    map_TypeError.insert(OPENDEVICE_RESULT, un_OpenDeviceError);
    map_TypeError.insert(SENDCMD_RESULT, un_SendCmdError);
    map_TypeError.insert(POWERTEST_REULST, un_PowerTestError);

    map_STTypeError.insert(un_SequenceTimes, map_TypeError);

    m_mapErrorAmount.insert(us_SequenceNumber, map_STTypeError);
    m_mapResultString.insert(us_SequenceNumber, list_ErrorString);
    m_mapResult.insert(us_SequenceNumber, list_Result);

    QList<QList<bool>> list_GroupResult;
    if(m_mapAllResult.contains(us_SequenceNumber)){
        list_GroupResult = m_mapAllResult.value(us_SequenceNumber);
    }

    list_GroupResult.append(list_Result);
    m_mapAllResult.insert(us_SequenceNumber, list_GroupResult);

    emit sig_ResultUpdata();

    return true;
}

bool CountTestData::CountRetestData(const ushort &us_SequenceNumber,
                                    const uint &un_SequenceTimes,
                                    QMap<int, bool> &map_EnumResult,
                                    QMap<int, bool> &map_OpenDeviceResult,
                                    QMap<int, bool> &map_SendCmdResult,
                                    QList<short> &list_Power_db,
                                    QMap<int, bool> &map_LastEnumResult,
                                    QMap<int, bool> &map_LastOpenDeviceResult,
                                    QMap<int, bool> &map_LastSendCmdResult,
                                    QList<short> &list_LastPower_db)
{
    m_nTestTotal ++;
    m_nRetestTotal ++;

    if(map_EnumResult.count() != OneGroupUsbNumber_CountData ||
            map_OpenDeviceResult.count() != OneGroupUsbNumber_CountData ||
            map_SendCmdResult.count() != OneGroupUsbNumber_CountData ||
            map_LastEnumResult.count() != OneGroupUsbNumber_CountData ||
            map_LastOpenDeviceResult.count() != OneGroupUsbNumber_CountData ||
            map_LastSendCmdResult.count() != OneGroupUsbNumber_CountData||
            list_Power_db.count() != 32 ||
            list_LastPower_db.count() != 32){
        return false;
    }

    if(m_mapDUTFWPosition.count() != 32 ||
            m_mapRFPowerDBLowerLimit.count() != 32 ||
            m_mapRFPowerDBUpperLimit.count() != 32){
        return false;
    }

    uint un_EnumError = 0;
    uint un_OpenDeviceError = 0;
    uint un_SendCmdError = 0;
    uint un_PowerTestError = 0;

    QString str_ErrorString = "";
    QList<QString> list_ErrorString;

    bool b_Result = true;
    QList<bool> list_Result;

    for(int i = 1; i <= OneGroupUsbNumber_CountData; i++){
        if(map_EnumResult.value(i) == false && map_EnumResult.value(i) == false){
            un_EnumError ++;
            str_ErrorString += "枚举异常\n";
            b_Result = false;
        }

        if(map_OpenDeviceResult.value(i) == false && map_OpenDeviceResult.value(i) == false){
            un_OpenDeviceError ++;
            str_ErrorString += "打开设备异常\n";
            b_Result = false;
        }

        if(map_SendCmdResult.value(i) == false && map_SendCmdResult.value(i) == false){
            un_SendCmdError ++;
            str_ErrorString += "发送命令异常\n";
            b_Result = false;
        }

        list_ErrorString.append(str_ErrorString);
        str_ErrorString = "";
        list_Result.append(b_Result);
        b_Result = true;
    }

    int n_OneRang = 0;
    for(int i = 0; i < 32; i++){
        if(m_mapDUTFWPosition.value(us_SequenceNumber).at(i) == 0){
            continue;
        }

        if((list_Power_db.at(i) <= m_mapRFPowerDBLowerLimit.value(us_SequenceNumber).at(i) ||
                list_Power_db.at(i) >= m_mapRFPowerDBUpperLimit.value(us_SequenceNumber).at(i)) &&
                (list_LastPower_db.at(i) <= m_mapRFPowerDBLowerLimit.value(us_SequenceNumber).at(i) ||
                 list_LastPower_db.at(i) >= m_mapRFPowerDBUpperLimit.value(us_SequenceNumber).at(i))){
            un_PowerTestError ++;
            str_ErrorString = list_ErrorString.at(n_OneRang);
            str_ErrorString += "RF功率异常\n";
            list_ErrorString.replace(n_OneRang, str_ErrorString);
            str_ErrorString = "";

            list_Result.replace(n_OneRang, false);
        }
        n_OneRang ++;
    }

    QMap<ENUM_RESULTTYPE, uint> map_TypeError;
    QMap<uint, QMap<ENUM_RESULTTYPE, uint>> map_STTypeError;

    if(m_mapErrorAmount.contains(us_SequenceNumber)){
        if(m_mapErrorAmount.value(us_SequenceNumber).contains(un_SequenceTimes)){
            QMap<ENUM_RESULTTYPE, uint> map_TypeError_1;
            map_TypeError_1 = m_mapErrorAmount.value(us_SequenceNumber).value(un_SequenceTimes);

            un_EnumError += map_TypeError_1.value(ENUM_RESULT);
            un_OpenDeviceError += map_TypeError_1.value(OPENDEVICE_RESULT);
            un_SendCmdError += map_TypeError_1.value(SENDCMD_RESULT);
            un_PowerTestError += map_TypeError_1.value(POWERTEST_REULST);
        }
    }

    map_TypeError.insert(ENUM_RESULT, un_EnumError);
    map_TypeError.insert(OPENDEVICE_RESULT, un_OpenDeviceError);
    map_TypeError.insert(SENDCMD_RESULT, un_SendCmdError);
    map_TypeError.insert(POWERTEST_REULST, un_PowerTestError);

    map_STTypeError.insert(un_SequenceTimes, map_TypeError);

    m_mapErrorAmount.insert(us_SequenceNumber, map_STTypeError);
    m_mapResultString.insert(us_SequenceNumber, list_ErrorString);
    m_mapResult.insert(us_SequenceNumber, list_Result);

    QList<QList<bool>> list_GroupResult;
    if(m_mapAllResult.contains(us_SequenceNumber)){
        list_GroupResult = m_mapAllResult.value(us_SequenceNumber);
    }

    list_GroupResult.append(list_Result);
    m_mapAllResult.insert(us_SequenceNumber, list_GroupResult);

    emit sig_ResultUpdata();

    return true;
}

bool CountTestData::GetAllResultData(const ushort &us_SequenceNumber,
                                     QMap<uint, QMap<ENUM_RESULTTYPE, uint> > &map_ErrorAmount)
{
    if(!m_mapErrorAmount.contains(us_SequenceNumber)){
        return false;
    }
    map_ErrorAmount = m_mapErrorAmount.value(us_SequenceNumber);
    return true;
}

bool CountTestData::ClearAllResultData(const ushort &us_SequenceNumber)
{
    if(!m_mapErrorAmount.contains(us_SequenceNumber)){
        return false;
    }
    m_mapErrorAmount.remove(us_SequenceNumber);
    return true;
}

bool CountTestData::GetSingleResultErrorString(const ushort &us_SequenceNumber,
                                               QList<bool> &list_SingleResult,
                                               QList<QString> &list_ResultString)
{
    if(!m_mapResultString.contains(us_SequenceNumber) ||
            !m_mapResult.contains(us_SequenceNumber)){
        return false;
    }

    list_SingleResult = m_mapResult.value(us_SequenceNumber);

    for(int i = 0; i < list_SingleResult.count(); i++){
        if(list_SingleResult.at(i))
            m_nPassTotal  ++;
        else
            m_nFaileTotal ++;
        m_nTestNumber ++;
    }

    list_ResultString = m_mapResultString.value(us_SequenceNumber);
    return true;
}

bool CountTestData::GetResult(const ushort &us_SequenceNumber,
                              bool &b_Result)
{
    if(!m_mapResult.contains(us_SequenceNumber)){
        return false;
    }

    if(m_mapResult.value(us_SequenceNumber).contains(false))
        b_Result = false;
    else
        b_Result = true;

    return true;
}

bool CountTestData::GetResultData(const ushort &us_SequenceNumber,
                                 QString &str_Result)
{
    if(m_mapAllResult.contains(us_SequenceNumber)){
        return false;
    }

    QList<QList<bool>> list_AllResult = m_mapAllResult.value(us_SequenceNumber);

    if(list_AllResult.isEmpty()){
        return false;
    }

    QList<bool> list_Result = list_AllResult.takeFirst();
    m_mapAllResult.insert(us_SequenceNumber, list_AllResult);

    if(list_Result.count() != OneGroupUsbNumber_CountData){
        return false;
    }

    for(int i = 0; i < list_Result.count(); i++){
        if(list_Result.at(i)){
            str_Result += "1";
        }
        else{
            str_Result += "0";
        }
    }

    return true;
}
