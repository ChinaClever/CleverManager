/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "up_datapacket.h"

Up_DataPacket::Up_DataPacket()
{
    data = new Up_sDataPacket;
    data->isRun = false;
    data->devtype = 0;
    data->progress = 0;
    data->subPorgress = 0;
}

Up_DataPacket *Up_DataPacket::bulid()
{
    static Up_DataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new Up_DataPacket();
    return sington;
}
