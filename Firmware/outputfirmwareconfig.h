#ifndef OUTPUTFIRMWARECONFIG_H
#define OUTPUTFIRMWARECONFIG_H
#include "firmware_define.h"

class OutPutFirmwareConfig
{
public:
    //输入pc配置参数
    void InPutPCConfig(STRUCT_PCTESTCONFIG &struct_PCConfig);
    //输出固件配置参数 通过pc配置参数转换
    bool OutPutFWConfig(char *p_cFWConfig, uint &un_Lenght);

private:
    STRUCT_PCTESTCONFIG m_structPCConfig;   //pc配置参数 
};

#endif // OUTPUTFIRMWARECONFIG_H
