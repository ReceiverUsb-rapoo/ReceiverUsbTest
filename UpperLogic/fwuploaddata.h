#ifndef FWUPLOADDATA_H
#define FWUPLOADDATA_H
#include <QList>
#include <QString>
#include "Firmware/firmware_define.h"

/*
* 固件数据缓存类
* 保存所有固件上传到pc端的的数据，便于查询，或者后期用于异常恢复
*/

class FWUploadData{
public:
    ushort us_SequenceNumber;                       //工作序号
    QString str_PortName;                           //com名称

    STRUCT_FWINFO struct_GetFWInfo;                 //FW信息

    int n_TestTimeOut;                              //测试时间

    ENUM_MACHINEDUTSTATE MachineDUTState;           //设备DUT状态
    ENUM_MACHINETESTSTATE MachineTestState;         //设备测试状态 

    short s_AdjustPowerVslop;                       //校准功率 Vslop
    short s_AdjustPowerVslopAdc;                    //校准功率 VslopAdc
    short s_AdjustPowerIntercept;                   //校准功率 Intercept

    QList<short> list_ReadPowerVslop;               //读取校准功率 32 Vslop
    QList<short> list_ReadPowerVslopAdc;            //读取校准功率 32 VslopAdc
    QList<short> list_ReadPowerIntercept;           //读取校准功率 32 Intercept
    QList<char> list_ReadPower_db_Compensation;     //读取校准功率 32 补偿db
    short s_ReadPowerReferenceVoltage_1;            //读取校准功率 参考电压1
    short s_ReadPowerReferenceVoltage_2;            //读取校准功率 参考电压2

    QList<short> list_PowerSelfTestSelfVoltage;     //功率自检测试 自检电压

    STRUCT_FWINFO struct_FWBoot;                    //boot数据

    ENUM_MSGDEFINE FWIntact;                        //固件设备状态 

    STRUCT_HANDBAG struct_HandBag;                  //心跳包

    char c_StartTestNoticeTestType;                 //开始测试通知 测试类型
    char c_StartTestNoticeTotalGroup;               //开始测试通知 测试总组数
    char c_StartTestNoticAmoutOfAGroup;             //开始测试通知 测试每组数量
    QList<char> list_StartTestNoticUsefulDUT;       //开始测试通知 有效DUT

    QList<char> list_CompleteTestUsefulResult;      //完成测试通知 有效结果DUT

    char c_StartOneGroupPowerTestSurplusGroup;      //开始一组功率测试剩余分组
    int n_StartOneGroupPowerTestDUTMask;            //开始一组功率测试DUT掩码

    char c_StartOneGroupEnumTestSurplusGroup;       //开始一组枚举测试剩余分组
    int n_StartOneGroupEnumTestDUTMask;             //开始一组枚举测试DUT掩码

    QList<short> list_UploadRFPowerResult_db;       //上传RF功率结果数据 


//类复制，数据复制
public:
    FWUploadData operator == (const FWUploadData* o_FWData){
        this->us_SequenceNumber = o_FWData->us_SequenceNumber;
        this->str_PortName = o_FWData->str_PortName;

        this->struct_GetFWInfo = o_FWData->struct_GetFWInfo;

        this->n_TestTimeOut = o_FWData->n_TestTimeOut;

        this->MachineDUTState = o_FWData->MachineDUTState;
        this->MachineTestState = o_FWData->MachineTestState;

        this->s_AdjustPowerVslop = o_FWData->s_AdjustPowerVslop;
        this->s_AdjustPowerVslopAdc = o_FWData->s_AdjustPowerVslopAdc;
        this->s_AdjustPowerIntercept = o_FWData->s_AdjustPowerIntercept;

        this->list_ReadPowerVslop = o_FWData->list_ReadPowerVslop;
        this->list_ReadPowerVslopAdc = o_FWData->list_ReadPowerVslopAdc;
        this->list_ReadPowerIntercept = o_FWData->list_ReadPowerIntercept;
        this->list_ReadPower_db_Compensation = o_FWData->list_ReadPower_db_Compensation;
        this->s_ReadPowerReferenceVoltage_1 = o_FWData->s_ReadPowerReferenceVoltage_1;
        this->s_ReadPowerReferenceVoltage_2 = o_FWData->s_ReadPowerReferenceVoltage_2;

        this->list_PowerSelfTestSelfVoltage = o_FWData->list_PowerSelfTestSelfVoltage;

        this->struct_FWBoot = o_FWData->struct_FWBoot;

        this->FWIntact = o_FWData->FWIntact;

        this->struct_HandBag = o_FWData->struct_HandBag;

        this->c_StartTestNoticeTestType = o_FWData->c_StartTestNoticeTestType;
        this->c_StartTestNoticeTotalGroup = o_FWData->c_StartTestNoticeTotalGroup;
        this->c_StartTestNoticAmoutOfAGroup = o_FWData->c_StartTestNoticAmoutOfAGroup;
        this->list_StartTestNoticUsefulDUT = o_FWData->list_StartTestNoticUsefulDUT;

        this->list_CompleteTestUsefulResult = o_FWData->list_CompleteTestUsefulResult;

        this->c_StartOneGroupPowerTestSurplusGroup = o_FWData->c_StartOneGroupPowerTestSurplusGroup;
        this->n_StartOneGroupPowerTestDUTMask = o_FWData->n_StartOneGroupPowerTestDUTMask;

        this->c_StartOneGroupEnumTestSurplusGroup = o_FWData->c_StartOneGroupEnumTestSurplusGroup;
        this->n_StartOneGroupEnumTestDUTMask = o_FWData->n_StartOneGroupEnumTestDUTMask;

        this->list_UploadRFPowerResult_db = o_FWData->list_UploadRFPowerResult_db;

        return *this;
    }

    //初始化数据
    void ClearFWData(){

        n_TestTimeOut = 0;

        MachineDUTState = DUTOUTFIXTURE;
        MachineTestState = UNSTART;

        s_AdjustPowerVslop = 0;
        s_AdjustPowerVslopAdc = 0;
        s_AdjustPowerIntercept = 0;

        list_ReadPowerVslop.clear();
        list_ReadPowerVslopAdc.clear();
        list_ReadPowerIntercept.clear();
        list_ReadPower_db_Compensation.clear();
        s_ReadPowerReferenceVoltage_1 = 0;
        s_ReadPowerReferenceVoltage_2 = 0;

        list_PowerSelfTestSelfVoltage.clear();

        memset(&struct_FWBoot, 0, sizeof(struct_FWBoot));

        FWIntact = NORMAL;

        memset(&struct_HandBag, 0, sizeof(struct_HandBag));

        c_StartTestNoticeTestType = 0;
        c_StartTestNoticeTotalGroup = 0;
        c_StartTestNoticAmoutOfAGroup = 0;
        list_StartTestNoticUsefulDUT.clear();

        list_CompleteTestUsefulResult.clear();

        c_StartOneGroupPowerTestSurplusGroup = 0;
        n_StartOneGroupPowerTestDUTMask = 0;

        c_StartOneGroupEnumTestSurplusGroup = 0;
        n_StartOneGroupEnumTestDUTMask = 0;

        list_UploadRFPowerResult_db.clear();
    }
};

#endif // FWUPLOADDATA_H
