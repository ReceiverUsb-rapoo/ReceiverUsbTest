#ifndef USBCONTROLCONFIG_H
#define USBCONTROLCONFIG_H
#include <QWidget>
#include "enumpointtable.h"

namespace Ui {
class UsbControlConfig;
}

class UsbControlConfig : public QWidget
{
    Q_OBJECT
public:
    explicit UsbControlConfig(ushort us_SequenceNumber, QWidget *parent = 0);
    ~UsbControlConfig();

    void SetSequenceNumber(const ushort &us_SequenceNumber);

private:
    void SaveCofig();

private:
    void InitUsbControlConfig();

    void ShowData();

    void SaveData();

private:
    Ui::UsbControlConfig *ui;
    EnumPointTable *m_pEnumPointTable;

    ushort m_usSequenceNumber;
};

#endif // USBCONTROLCONFIG_H
