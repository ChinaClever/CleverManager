/*
 * dc_cabinetstg.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_cabinetstg.h"

DC_CabinetsTg::DC_CabinetsTg()
{

}

DC_CabinetsTg *DC_CabinetsTg::bulid()
{
    static DC_CabinetsTg* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_CabinetsTg();
    return sington;
}


