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
    //获取本此测试结果数据
    void GetTestTotal(int &n_TestTotal,
                      int &n_RetestTotal,
                      int &n_PassTotal,
                      int &n_FaileTotal,
                      int &n_TestNumber);
    //清除本此测试结果数据
    void ClearTestTotal();
    //设置重测开关
    void SetRetest(const ushort &us_SequenceNumber,
                   const bool &b_Retest);
    //设置RF功率 db上下限
    void SetRFPowerDBLimit(const ushort &us_SequenceNumber,
                           const QList<int> &list_RFPowerDBUpperLimit,
                           const QList<int> &list_RFPowerDBLowerLimit);
    //设置DUT测试位置
    void SetDUTFWPositions(const ushort &us_SequenceNumber,
                           const QList<int> &list_DUTFWPosition);
    //计算一次测试数据 
    bool CountOneTestData(const ushort &us_SequenceNumber,
                          const uint &un_SequenceTimes,
                          QMap<int,bool> &map_EnumResult,
                          QMap<int,bool> &map_OpenDeviceResult,
                          QMap<int,bool> &map_SendCmdResult,
                          QList<short> &list_Power_db);
    //计算重测测试数据
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
    //获取所有测试结果
    bool GetAllResultData(const ushort &us_SequenceNumber,
                          QMap<uint, QMap<ENUM_RESULTTYPE, uint>> &map_ErrorAmount);
    //清除所有测试结果
    bool ClearAllResultData(const ushort &us_SequenceNumber);
    //获取单次结果-错误类型数据 
    bool GetSingleResultErrorString(const ushort &us_SequenceNumber,
                                    QList<bool> &list_SingleResult,
                                    QList<QString> &list_ResultString);
    //获取结果 ok/ng
    bool GetResult(const ushort &us_SequenceNumber,
                   bool &b_Result);

    //获取结果 特指分料数据 
    bool GetResultData(const ushort &us_SequenceNumber,
                       QString &str_Result);
signals:
    //测试结果更新
    void sig_ResultUpdata();

private:
    //<SequenceNumber <SequenceTimes  <ErrorType  Number>>>
    //<工作序号，<测试顺次， <测试错误类型， 数量>>>
    QMap<ushort,QMap<uint, QMap<ENUM_RESULTTYPE, uint>>> m_mapErrorAmount;  //测试数据 
    QMap<ushort,QList<int>> m_mapRFPowerDBLowerLimit;   //RF功率下限 <工作序号，list<下限数值>>
    QMap<ushort,QList<int>> m_mapRFPowerDBUpperLimit;   //RF功率上限 <工作序号，list<上限数值>>
    QMap<ushort,QList<int>> m_mapDUTFWPosition;     //DUT位置 <工作序号，list<DUT位置>>
    QMap<ushort,QList<bool>> m_mapResult;           //测试结果 <工作序号，list<测试结果>>
    QMap<ushort,QList<QString>> m_mapResultString;  //测试结果信息 <工作序号，list<结果信息>>
    QMap<ushort,QList<QList<bool>>> m_mapAllResult; //测试所有结果 <工作序号，list<list<测试结果>>>  list按顺序排列

    QMap<ushort,bool> m_mapRetest;  //重测开关 <工作序号，重测开关>

    int m_nTestTotal;   //测试总次
    int m_nRetestTotal; //重测次数
    int m_nPassTotal;   //通过次数
    int m_nFaileTotal;  //失败次数
    int m_nTestNumber;  //测试总数
};

#endif // COUNTTESTDATA_H
