#ifndef FWUPLOADDATA_H
#define FWUPLOADDATA_H
#include <QList>
#include <QString>
#include "Firmware/firmware_define.h"

class FWUploadData{
public:
    ushort us_SequenceNumber;
    QString str_PortName;

    STRUCT_FWINFO struct_GetFWInfo;

    int n_TestTimeOut;

    ENUM_MACHINEDUTSTATE MachineDUTState;
    ENUM_MACHINETESTSTATE MachineTestState;

    short s_AdjustPowerVslop;
    short s_AdjustPowerVslopAdc;
    short s_AdjustPowerIntercept;

    QList<short> list_ReadPowerVslop;
    QList<short> list_ReadPowerVslopAdc;
    QList<short> list_ReadPowerIntercept;
    QList<char> list_ReadPower_db_Compensation;
    short s_ReadPowerReferenceVoltage_1;
    short s_ReadPowerReferenceVoltage_2;

    QList<short> list_PowerSelfTestSelfVoltage;

    STRUCT_FWINFO struct_FWBoot;

    ENUM_MSGDEFINE FWIntact;

    STRUCT_HANDBAG struct_HandBag;

    char c_StartTestNoticeTestType;
    char c_StartTestNoticeTotalGroup;
    char c_StartTestNoticAmoutOfAGroup;
    QList<char> list_StartTestNoticUsefulDUT;

    QList<char> list_CompleteTestUsefulResult;

    char c_StartOneGroupPowerTestSurplusGroup;
    int n_StartOneGroupPowerTestDUTMask;

    char c_StartOneGroupEnumTestSurplusGroup;
    int n_StartOneGroupEnumTestDUTMask;

    QList<short> list_UploadRFPowerResult_db;


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
