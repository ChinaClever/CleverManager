/*
 * cloudalarmlog
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "cloudalarmlog.h"
#include "cloudcabinet.h"
#include "mysql/dc_pdudevices.h"

CloudAlarmLog::CloudAlarmLog()
{

}

CloudAlarmLog *CloudAlarmLog::bulid()
{
    static CloudAlarmLog* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudAlarmLog();
    return sington;
}


int CloudAlarmLog::getAlarmType(QString &str)
{
    int ret = 0;

    if(str.contains(QObject::tr("相电压报警"))) ret = 1;
    else if(str.contains(QObject::tr("相电流报警"))) ret = 2;
    else if(str.contains(QObject::tr("回路电流报警"))) ret = 3;
    else if(str.contains(QObject::tr("输出位电流报警"))) ret = 4;
    else if(str.contains(QObject::tr("温度报警"))) ret = 5;
    else if(str.contains(QObject::tr("湿度报警"))) ret = 6;
    else if(str.contains(QObject::tr("门禁报警"))) ret = 7;
    else if(str.contains(QObject::tr("水浸报警"))) ret = 8;
    else if(str.contains(QObject::tr("烟雾报警"))) ret = 9;

    return ret;
}

int CloudAlarmLog::getRid(CloudAlarmItem &item)
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

void CloudAlarmLog::saveLog(CloudAlarmItem &item)
{
    int rid = getRid(item);
    if(rid>0) {
        DC_DevLogItem logItem;
        logItem.pdu_devices_id = rid;
        logItem.item_type = getAlarmType(item.alarmType);
        logItem.contents = item.msg;
        DC_DevLogs::bulid()->insertItem(logItem);
    }
}
