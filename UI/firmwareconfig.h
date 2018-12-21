#ifndef FIRMWARECONFIG_H
#define FIRMWARECONFIG_H
#include <QDialog>
//#include "enumpointtable.h"
#include "pointtable.h"

/*
* 固件配置参数UI  
*/

namespace Ui {
class FirmwareConfig;
}

class FirmwareConfig : public QDialog
{
    Q_OBJECT
public:
    explicit FirmwareConfig(ushort us_SequenceNumber, QWidget *parent = 0);
    ~FirmwareConfig();
    //设置工作序号
    void SetSequenceNumber(const ushort &us_SequenceNumber);

private:
    //保存配置参数
    void SaveCofig();

private:
    //初始化固件配置参数
    void InitFirmwareConfig();
    //初始化UI
    void InitFWUI();
    //显示配置参数
    void ShowData();
    //保存配置参数
    void SaveData();

private:
    Ui::FirmwareConfig *ui;

//    EnumPointTable *m_pEnumPointTable;
    PointTable *m_pPT_FWPositionNumber;         //DUT位置表格 
    PointTable *m_pPT_PowerTestPositionNumber;  //功率测试位置表格
    PointTable *m_pPT_PowerTestGroup;           //功率测试组号

    ushort m_usSequenceNumber;                  //工作序号
};

#endif // FIRMWARECONFIG_H
