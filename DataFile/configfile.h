#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include "Firmware/firmware_define.h"
#include "datafile_define.h"

class ConfigFile
{
public:
    //保存 固件和PC测试参数
    bool SavePCTestConfig(const ushort &us_SequenceNumber,
                          STRUCT_PCTESTCONFIG &struct_PCTestConfig);
    //获取 固件和PC测试参数
    bool GetPCTestConfig(const ushort &us_SequenceNumber,
                         STRUCT_PCTESTCONFIG &struct_PCTestConfig);
    //保存 usb接收器配置参数
    bool SaveUsbControlConfig(const ushort &us_SequenceNumber,
                              STRUCT_USBCONTROLCONFIG &struct_UsbControlConfig);
    //获取 usb接收器配置参数
    bool GetUsbControlConfig(const ushort &us_SequenceNumber,
                             STRUCT_USBCONTROLCONFIG &struct_UsbControlConfig);
    //获取 外围设备参数（ip，port，控制配置 ）
    bool GetEquitmentConfig(STRUCT_EQUITMENTCONFIG &struct_EquitmentConfig);
    //保存 外围设备参数（ip，port，控制配置 ）
    bool SaveEquitmenConfig(STRUCT_EQUITMENTCONFIG &struct_EquitmentConfig);


public:
    //转换数据类型 QString to QList<int>
    bool TransformToList(const QString &str_Data, QList<int> &list_Data);
    //转换数据类型 QList<int> to QString
    bool TransformToQString(const QList<int> &list_Data, QString &str_Data);
    //转换数据类型 QString to QList<QString>
    void TransformToList(const QString &str_Data, QList<QString> &list_Data);
    //转换数据类型 QList<QString> to QString
    bool TransformToQString(const QList<QString> &list_Data, QString &str_Data);
    //转换数据类型 QString to Qmap<int,int>
    bool TransformToMap(const QString &str_Data,
                        QMap<int,int> &map_SequenceData);
    //转换数据类型 Qmap<int,int> to QString
    bool TransformToQString(const QMap<int,int> &map_SequenceData,
                            QString &str_Data);

    //转换数据类型 Qmap<int,int> to QString 带序号 
    bool TransformToQString(const ushort &us_SequenceNumber,
                            const QMap<int,int> &map_SequenceData,
                            QString &str_Data);
    //转换数据类型 QString to Qmap<int,int> 带序号
    bool TransformToMap(const ushort &us_SequenceNumber,
                        const QString &str_Data,
                        QMap<int,int> &map_SequenceData);

};

#endif // CONFIGFILE_H
