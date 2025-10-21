/*
 * dc_roomstg.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_roomstg.h"

DC_RoomsTg::DC_RoomsTg()
{

}


DC_RoomsTg *DC_RoomsTg::bulid()
{
    static DC_RoomsTg* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_RoomsTg();
    return sington;
}


