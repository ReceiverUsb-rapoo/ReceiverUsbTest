#ifndef COUNTTESTDATA_H
#define COUNTTESTDATA_H
#include <QString>
#include <QMap>
#include <QList>
#include "upperdefine.h"

const int OneGroupUsbNumber_CountData = 20;

class CountTestData
{
    friend class CountTestResultInstanceGetter;
private:
    CountTestData();

public:
    ~CountTestData();
//    void SetSequenceNumber(const ushort &us_SequenceNumber);

    void SetRFPowerDBLimit(const ushort &us_SequenceNumber,
                           const QList<int> &list_RFPowerDBUpperLimit,
                           const QList<int> &list_RFPowerDBLowerLimit);

    void SetDUTFWPositions(const ushort &us_SequenceNumber,
                           const QList<int> &list_DUTFWPosition);

    bool CountOneTestData(const ushort &us_SequenceNumber,
                          const uint &un_SequenceTimes,
                          QMap<int,bool> &map_EnumResult,
                          QMap<int,bool> &map_OpenDeviceResult,
                          QMap<int,bool> &map_SendCmdResult,
                          QList<short> &list_Power_db);

    bool GetAllResultData(const ushort &us_SequenceNumber,
                          QMap<uint, QMap<ENUM_RESULTTYPE, uint>> &map_ErrorAmount);

    bool ClearAllResultData(const ushort &us_SequenceNumber);

    bool GetResultErrorString(const ushort &us_SequenceNumber,
                              QList<QString> &list_ResultString);

private:
    QMap<ushort,QMap<uint, QMap<ENUM_RESULTTYPE, uint>>> m_mapErrorAmount;
    QMap<ushort,QList<int>> m_mapRFPowerDBLowerLimit;
    QMap<ushort,QList<int>> m_mapRFPowerDBUpperLimit;
    QMap<ushort,QList<int>> m_mapDUTFWPosition;
    QMap<ushort,QList<bool>> m_mapResult;
    QMap<ushort,QList<QString>> m_mapResultString;


};

#endif // COUNTTESTDATA_H
