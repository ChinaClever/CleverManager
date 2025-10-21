/*
 * cloudroomtg.cpp
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "cloudroomtg.h"

#include "mysql/dc_roomstatus.h"

CloudRoomTg::CloudRoomTg()
{
    mTg = DC_RoomsTg::bulid();
    mRoom = CloudRoom::bulid();
}


void CloudRoomTg::changeTo(TgRoomItem &item, DC_TgItem &dcItem)
{
    dcItem.rid = mRoom->getRoomId(item.roomName);

    dcItem.vol = item.vol;
    dcItem.cur = item.cur;
    dcItem.pow = item.pow;
    dcItem.ele = item.ele;
    dcItem.pf = item.pf;
    dcItem.tem = item.tem;
    dcItem.hum = item.hum;

    dcItem.all_num = item.allNum;
    dcItem.line_num = item.lineNum;
    dcItem.alarm_num = item.alarmNum;
    dcItem.offLine_num = item.offLineNum;

    updateStatus(dcItem.rid, dcItem.alarm_num);
}


bool CloudRoomTg::save(TgRoomItem &item)
{
    bool ret = false;
    DC_TgItem dcItem;
    changeTo(item, dcItem);

    if(dcItem.rid>0) {
        ret = mTg->insertItem(dcItem);
    }
    return ret;
}

bool CloudRoomTg::update(TgRoomItem &item)
{
    bool ret = false;
    DC_TgItem dcItem;
    changeTo(item, dcItem);

    if(dcItem.rid>0) {
        dcItem.id = mTg->getMaxId(dcItem.rid);
        if(dcItem.id>0)
            ret = mTg->updateItem(dcItem);
        else
           ret = save(item);
    }
    return ret;
}

bool CloudRoomTg::updateStatus(int rid, int status)
{
    bool ret = false;
    if(status) // 有报警
        status = 1;
    if(rid > 0)
        ret = DC_RoomStatus::bulid()->updateStatus(rid, status); // 0正常， 1报警

    return ret;
}
