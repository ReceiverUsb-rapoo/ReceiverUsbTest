#ifndef COUNTTESTDATA_H
#define COUNTTESTDATA_H
#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include "upperdefine.h"

const int OneGroupUsbNumber_CountData = 20;

class CountTestData : public QObject
{
    Q_OBJECT
    friend class CountTestResultInstanceGetter;
private:
    CountTestData();

public:
    ~CountTestData();
//    void SetSequenceNumber(const ushort &us_SequenceNumber);

    void GetTestTotal(int &n_TestTotal,
                      int &n_RetestTotal,
                      int &n_PassTotal,
                      int &n_FaileTotal,
                      int &n_TestNumber);

    void ClearTestTotal();

    void SetRetest(const bool &b_Retest);

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

    bool CountRetestData(const ushort &us_SequenceNumber,
                         const uint &un_SequenceTimes,
                         QMap<int,bool> &map_EnumResult,
                         QMap<int,bool> &map_OpenDeviceResult,
                         QMap<int,bool> &map_SendCmdResult,
                         QList<short> &list_Power_db,
                         QMap<int,bool> &map_LastEnumResult,
                         QMap<int,bool> &map_LastOpenDeviceResult,
                         QMap<int,bool> &map_LastSendCmdResult,
                         QList<short> &list_LastPower_db);

    bool GetAllResultData(const ushort &us_SequenceNumber,
                          QMap<uint, QMap<ENUM_RESULTTYPE, uint>> &map_ErrorAmount);

    bool ClearAllResultData(const ushort &us_SequenceNumber);

    bool GetSingleResultErrorString(const ushort &us_SequenceNumber,
                                    QList<bool> &list_SingleResult,
                                    QList<QString> &list_ResultString);

    bool GetResult(const ushort &us_SequenceNumber,
                   bool &b_Result);

    //00000000000000000000
    bool GetResultData(const ushort &us_SequenceNumber,
                       QString &str_Result);
signals:
    void sig_ResultUpdata();

private:
    //<SequenceNumber <SequenceTimes  <ErrorType  Number>>>
    QMap<ushort,QMap<uint, QMap<ENUM_RESULTTYPE, uint>>> m_mapErrorAmount;
    QMap<ushort,QList<int>> m_mapRFPowerDBLowerLimit;
    QMap<ushort,QList<int>> m_mapRFPowerDBUpperLimit;
    QMap<ushort,QList<int>> m_mapDUTFWPosition;
    QMap<ushort,QList<bool>> m_mapResult;
    QMap<ushort,QList<QString>> m_mapResultString;
    QMap<ushort,QList<QList<bool>>> m_mapAllResult;

    bool m_bRetest;

    int m_nTestTotal;
    int m_nRetestTotal;
    int m_nPassTotal;
    int m_nFaileTotal;
    int m_nTestNumber;
};

#endif // COUNTTESTDATA_H
