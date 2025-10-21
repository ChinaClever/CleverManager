/*
 * cloudcabinettg.cpp
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "cloudcabinettg.h"

CloudCabinetTg::CloudCabinetTg()
{
    mTg = DC_CabinetsTg::bulid();
    mCabinet = CloudCabinet::bulid();
}


void CloudCabinetTg::changeTo(const QString &name, TgRoomItem &item, DC_TgItem &dcItem)
{
    dcItem.rid = mCabinet->getId(item.roomName, name);

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


bool CloudCabinetTg::save(const QString &name,TgRoomItem &item)
{
    bool ret = false;
    DC_TgItem dcItem;
    changeTo(name, item, dcItem);

    if(dcItem.rid>0) {
        ret = mTg->insertItem(dcItem);
    }
    return ret;
}

bool CloudCabinetTg::update(const QString &name, TgRoomItem &item)
{
    bool ret = false;
    DC_TgItem dcItem;
    changeTo(name,item, dcItem);

    if(dcItem.rid>0) {
        dcItem.id = mTg->getMaxId(dcItem.rid);
        if(dcItem.id>0)
            ret = mTg->updateItem(dcItem);
        else
           ret = save(name,item);
    }
    return ret;
}


bool CloudCabinetTg::updateStatus(int rid, int status)
{
    bool ret = false;
    if(status) // 有报警
        status = 1;
    if(rid > 0)
        ret = DC_CabinetStatus::bulid()->updateStatus(rid, status); // 0正常， 1报警

    return ret;
}
