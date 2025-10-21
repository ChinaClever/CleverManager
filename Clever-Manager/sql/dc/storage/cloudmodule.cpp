/*
 * cloudmodule.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "cloudmodule.h"
#include "mysql/dc_modulestatus.h"

CloudModule::CloudModule()
{
    mTg = DC_ModulesTg::bulid();
}

void CloudModule::changeTo(TgRoomItem &item, DC_TgItem &dcItem)
{
    dcItem.rid = CloudSetParam::bulid()->module.id;

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

    updateStatus(dcItem.alarm_num);
}

bool CloudModule::save(TgRoomItem &item)
{
    bool ret = false;
    DC_TgItem dcItem;
    changeTo(item, dcItem);

    if(dcItem.rid>0) {
        ret = mTg->insertItem(dcItem);
    }
    return ret;
}

bool CloudModule::update(TgRoomItem &item)
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

bool CloudModule::updateStatus(int status)
{    
    bool ret = false;
    int rid = CloudSetParam::bulid()->module.id;
    if(status) // 有报警
        status = 1;

    if(rid > 0)
        ret = DC_ModuleStatus::bulid()->updateStatus(rid, status); // 0正常， 1报警

    return ret;
}
