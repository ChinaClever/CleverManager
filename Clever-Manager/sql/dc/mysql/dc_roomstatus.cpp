/*
 * dc_roomstatus.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_roomstatus.h"

DC_RoomStatus::DC_RoomStatus()
{

}


DC_RoomStatus *DC_RoomStatus::bulid()
{
    static DC_RoomStatus* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_RoomStatus();
    return sington;
}


