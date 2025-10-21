/*
 * dc_cabinetsspare.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_cabinetsspare.h"

DC_CabinetsSpare::DC_CabinetsSpare()
{

}

DC_CabinetsSpare *DC_CabinetsSpare::bulid()
{
    static DC_CabinetsSpare* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_CabinetsSpare();
    return sington;
}

