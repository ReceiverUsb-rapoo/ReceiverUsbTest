#include "outputfirmwareconfig.h"
#include "QStringList"
#include <QDebug>

void OutPutFirmwareConfig::InPutPCConfig(STRUCT_PCTESTCONFIG &struct_PCConfig)
{
    m_structPCConfig = struct_PCConfig;
}

bool OutPutFirmwareConfig::OutPutFWConfig(char *p_cFWConfig, uint &un_Lenght)
{
    char cFWConfig[254] = {};
    STRUCT_FWCONGIF struct_FWConfig;

    struct_FWConfig.us_WorkCurrentTime = 0;
    struct_FWConfig.us_SleepCurrentTime = 0;
    struct_FWConfig.us_LEDTestTime = 0;
    struct_FWConfig.us_LED2TestTime = 0;
    struct_FWConfig.us_IRTestTime = 0;
    struct_FWConfig.us_LaserTestTime = 0;
    struct_FWConfig.us_BurnEEPROMTime = 0;
    struct_FWConfig.us_ChargeTextTime = 0;
    struct_FWConfig.us_SendTime = 0;
    struct_FWConfig.us_ReceiveTime = 0;
    struct_FWConfig.us_PoweredDelay = m_structPCConfig.us_PoweredDelay;
    struct_FWConfig.us_ReadRFDelay = m_structPCConfig.us_ReadRFDelay;
    struct_FWConfig.us_SleepCurrentDelay = 0;
    struct_FWConfig.us_DischargeTimeSelect = m_structPCConfig.us_DischargeTimeSelect;
    struct_FWConfig.us_DUTVoltageSelect = m_structPCConfig.us_DUTVoltageSelect;
    struct_FWConfig.us_CloseCurrentSet = 0;
    struct_FWConfig.uc_IRTestSwitch = 0;
    struct_FWConfig.uc_LEDTestSwitch = 0;
    struct_FWConfig.uc_LaserTestSwitch = 0;
    struct_FWConfig.uc_FunctionTextSwitch = 0;
    struct_FWConfig.uc_SelfTestCode = 0;
    struct_FWConfig.uc_RetestTimes = m_structPCConfig.uc_RetestTimes;
    struct_FWConfig.uc_IRCollectAvgTimes = 0;
    struct_FWConfig.uc_LEDCollectAvgTimes = 0;
    struct_FWConfig.uc_LED2CollectAvgTimes =0;
    struct_FWConfig.uc_CurrentCollectAvgTimes = 0;
    struct_FWConfig.uc_CrytalCollectAvgTimes = 0;
    struct_FWConfig.uc_ReceiveRSSILimit = 0;
    struct_FWConfig.uc_SingleTestCount = m_structPCConfig.uc_SingleTestCount;

    memcpy(cFWConfig, &struct_FWConfig, 45);

    //address to 45
    QStringList list_DUTFWPositions = m_structPCConfig.str_DUTFWPositions.split(',');
    QStringList list_PowerDUTPositions = m_structPCConfig.str_PowerDUTPositions.split(',');
    QStringList list_PowerTestGroups = m_structPCConfig.str_PowerTestGroups.split(',');

    if(list_DUTFWPositions.count() != PositionTotal ||
            list_PowerDUTPositions.count() != PositionTotal ||
            list_PowerTestGroups.count() != PositionTotal){
        return false;
    }

    for(int i = 0; i < PositionTotal; i++){
        cFWConfig[45 + i] = (signed char)list_DUTFWPositions.at(i).toInt();
        cFWConfig[45 + PositionTotal + i] = 0x00;
        cFWConfig[45 + 2*PositionTotal + i] = 0x00;
    }

    //address to 141
    struct_FWConfig.us_SinglePowerDelay = m_structPCConfig.us_SinglePowerDelay;
    struct_FWConfig.us_SinglePowerTime = m_structPCConfig.us_SinglePowerTime;
    struct_FWConfig.uc_PowerAvgTimes = m_structPCConfig.uc_PowerAvgTimes;
    struct_FWConfig.uc_PowerChannel = m_structPCConfig.uc_PowerChannel;
    struct_FWConfig.uc_PowerTestSwitch = m_structPCConfig.uc_PowerTestSwitch;

    memcpy(cFWConfig + 141, (signed char *)&struct_FWConfig + 45, 7);

    //address to 148
    for(int i = 0; i < PositionTotal; i++){
        cFWConfig[148 + i] = (signed char)list_PowerDUTPositions.at(i).toInt();
        cFWConfig[148 + PositionTotal + i] = (signed char)list_PowerTestGroups.at(i).toInt();
    }

    //address to 212
    struct_FWConfig.us_FirstPowerTestDelay = m_structPCConfig.us_FirstPowerTestDelay;
    struct_FWConfig.us_EnumTime = m_structPCConfig.us_EnumTime;
    struct_FWConfig.uc_EnumTestSwitch = m_structPCConfig.uc_EnumTestSwitch;
    struct_FWConfig.uc_TotalByte = 0xda;

    memcpy(cFWConfig + 212, (signed char *)&struct_FWConfig + 52, 6);

    memcpy(p_cFWConfig, cFWConfig, 241);
    un_Lenght = 218;

//    for(int i = 0; i < 218; i++){
//        qDebug()<<i<<" "<<QString::number((uchar)p_cFWConfig[i], 16);
//    }

    //address to 218 and no data
    return true;
}


