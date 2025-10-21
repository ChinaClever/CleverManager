/*
 * dc_cabinetstatus.cpp
 *  机柜状态表
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_cabinetstatus.h"

DC_CabinetStatus::DC_CabinetStatus()
{

}


DC_CabinetStatus *DC_CabinetStatus::bulid()
{
    static DC_CabinetStatus* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_CabinetStatus();
    return sington;
}






