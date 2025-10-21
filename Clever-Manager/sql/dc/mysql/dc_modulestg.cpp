/*
 * dc_modulestg.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_modulestg.h"

DC_ModulesTg::DC_ModulesTg()
{

}

DC_ModulesTg *DC_ModulesTg::bulid()
{
    static DC_ModulesTg* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_ModulesTg();
    return sington;
}

