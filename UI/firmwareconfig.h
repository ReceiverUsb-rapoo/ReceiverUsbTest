#ifndef FIRMWARECONFIG_H
#define FIRMWARECONFIG_H
#include <QDialog>
//#include "enumpointtable.h"
#include "pointtable.h"

namespace Ui {
class FirmwareConfig;
}

class FirmwareConfig : public QDialog
{
    Q_OBJECT
public:
    explicit FirmwareConfig(ushort us_SequenceNumber, QWidget *parent = 0);
    ~FirmwareConfig();

    void SetSequenceNumber(const ushort &us_SequenceNumber);

private:
    void SaveCofig();

private:
    void InitFirmwareConfig();

    void InitFWUI();

    void ShowData();

    void SaveData();

private:
    Ui::FirmwareConfig *ui;

//    EnumPointTable *m_pEnumPointTable;
    PointTable *m_pPT_FWPositionNumber;
    PointTable *m_pPT_PowerTestPositionNumber;
    PointTable *m_pPT_PowerTestGroup;

    ushort m_usSequenceNumber;
};

#endif // FIRMWARECONFIG_H
