#ifndef OUTPUTFIRMWARECONFIG_H
#define OUTPUTFIRMWARECONFIG_H
#include "firmware_define.h"

class OutPutFirmwareConfig
{
public:
    void InPutPCConfig(STRUCT_PCTESTCONFIG &struct_PCConfig);
    bool OutPutFWConfig(char *p_cFWConfig, uint &un_Lenght);

private:
    STRUCT_PCTESTCONFIG m_structPCConfig;

};

#endif // OUTPUTFIRMWARECONFIG_H
