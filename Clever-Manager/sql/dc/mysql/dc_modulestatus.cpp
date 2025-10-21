/*
 * dc_modulestatus.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_modulestatus.h"

DC_ModuleStatus::DC_ModuleStatus()
{

}

DC_ModuleStatus *DC_ModuleStatus::bulid()
{
    static DC_ModuleStatus* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_ModuleStatus();
    return sington;
}

