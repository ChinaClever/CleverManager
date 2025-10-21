/*
 * cloudoffline.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "cloudoffline.h"
#include "cloudcabinet.h"
#include "mysql/dc_pdudevices.h"

CloudOffLine::CloudOffLine()
{

}

CloudOffLine *CloudOffLine::bulid()
{
    static CloudOffLine* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudOffLine();
    return sington;
}


int CloudOffLine::getRid(CloudOffLineItem &item)
{
    int rid = -1;
    if(!item.cab.isEmpty()) {
        rid = CloudCabinet::bulid()->getId(item.room, item.cab);
        if(rid>0) {
            DC_PduDevItem dcItem;
            dcItem.cabinets_id = rid;
            dcItem.type_index = item.devType;
            dcItem.ip = item.ip;
            dcItem.num = item.num;

            rid = DC_PduDevices::bulid()->getId(dcItem);
        }
    }

    return rid;
}


void CloudOffLine::saveLog(CloudOffLineItem &item)
{
    int rid = getRid(item);
    if(rid>0) {
        DC_DevOfflineItem dcItem;
        dcItem.pdu_devices_id = rid;
        dcItem.contents = item.msg;

        DC_DevOfflines::bulid()->insertItem(dcItem);
    }
}
