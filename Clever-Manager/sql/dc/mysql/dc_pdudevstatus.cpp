/*
 * dc_pdudevstatus.cpp
 *  PDU设备状态表
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_pdudevstatus.h"

DC_PduDevStatus::DC_PduDevStatus()
{

}

DC_PduDevStatus *DC_PduDevStatus::bulid()
{
    static DC_PduDevStatus* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_PduDevStatus();
    return sington;
}


