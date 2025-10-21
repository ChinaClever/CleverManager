/*
 * cloudsetparam.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "cloudsetparam.h"
#include "common.h"
#include "services/config/sysconfigfile.h"

CloudSetParam::CloudSetParam()
{
    readFile();
    DC_Companys::bulid();
}

CloudSetParam *CloudSetParam::bulid()
{
    static CloudSetParam* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudSetParam();
    return sington;
}


void CloudSetParam::readFile()
{
    bool ret = Sys_ConfigFile_Open();
    if(ret) {
        company.id = Sys_ConfigFile_ReadInt("company_id");
        company.name = Sys_ConfigFile_ReadStr("company_name");
        company.domain = Sys_ConfigFile_ReadStr("company_domain");

        idc.id = Sys_ConfigFile_ReadInt("idc_id");
        idc.name = Sys_ConfigFile_ReadStr("idc_name");

        module.id = Sys_ConfigFile_ReadInt("module_id");
        module.name = Sys_ConfigFile_ReadStr("module_name");

        Sys_ConfigFile_Close();
    }
}


void CloudSetParam::saveCompany()
{
   Sys_ConfigFile_WriteParam("company_id", QString::number(company.id));
   Sys_ConfigFile_WriteParam("company_name", company.name);
   Sys_ConfigFile_WriteParam("company_domain", company.domain);
}

void CloudSetParam::saveIdc()
{
   Sys_ConfigFile_WriteParam("idc_id", QString::number(idc.id));
   Sys_ConfigFile_WriteParam("idc_name", idc.name);
}

void CloudSetParam::saveModule()
{
   Sys_ConfigFile_WriteParam("module_id", QString::number(module.id));
   Sys_ConfigFile_WriteParam("module_name", module.name);
}

void CloudSetParam::saveAll()
{
    saveCompany();
    saveIdc();
    saveModule();
}
