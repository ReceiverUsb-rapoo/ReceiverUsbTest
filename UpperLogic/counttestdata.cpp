#include "counttestdata.h"

CountTestData::CountTestData()
{

}

CountTestData::~CountTestData()
{

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
    if(map_EnumResult.count() != OneGroupUsbNumber_CountData ||
            map_OpenDeviceResult.count() != OneGroupUsbNumber_CountData ||
            map_SendCmdResult.count() != OneGroupUsbNumber_CountData ||
            list_Power_db.count() != 32){
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

bool CountTestData::GetResultErrorString(const ushort &us_SequenceNumber,
                                         QList<QString> &list_ResultString)
{
    list_ResultString = m_mapResultString.value(us_SequenceNumber);
    return true;
}
