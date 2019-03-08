#ifndef USBCONTROLCONFIG_H
#define USBCONTROLCONFIG_H
#include <QWidget>
#include "enumpointtable.h"

/*
* 外围设备参数配置UI
*/

namespace Ui {
class UsbControlConfig;
}

class UsbControlConfig : public QWidget
{
    Q_OBJECT
public:
    explicit UsbControlConfig(ushort us_SequenceNumber, QWidget *parent = 0);
    ~UsbControlConfig();
    //设置工作序号
    void SetSequenceNumber(const ushort &us_SequenceNumber);

private:
    //保存配置
    void SaveCofig();

private:
    //初始化配置
    void InitUsbControlConfig();
    //显示数据
    void ShowData();
    //保存村数据 
    void SaveData();

private:
    Ui::UsbControlConfig *ui;
    EnumPointTable *m_pEnumPointTable;

    ushort m_usSequenceNumber;
};

#endif // USBCONTROLCONFIG_H
