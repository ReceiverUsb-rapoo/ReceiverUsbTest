#ifndef WRITECMDTHREAD_H
#define WRITECMDTHREAD_H
#include <QRunnable>
#include <QString>
#include "libusb-1.0/libusb.h"

class WriteCmdThread : public QRunnable
{
public:
    void run();
    //设置位置
    void SetStation(int n_Station);
    //设置设备句柄
    void SetLDHandle(libusb_device_handle *p_LDHandle);
private:
    int m_nStation;                         //位置
    libusb_device_handle *m_pLDHandle;      //设备句柄
};

#endif // WRITECMDTHREAD_H
