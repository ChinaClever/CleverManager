/*
 * DbRtPduDevRecord.cpp
 * 实时PDU设备数据表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbrtpdudevrecord.h"
#include "dbrtpdudevindex.h"
#include "dbrtpduelecparams.h"
#include "dbrtpduenvsensor.h"
#include "sql/dc/storage/clouddevrecord.h"

DbRtPduDevRecord::DbRtPduDevRecord(QObject *parent): DbPduDevRecord(parent)
{

}

DbRtPduDevRecord *DbRtPduDevRecord::get(QObject *parent)
{
    static DbRtPduDevRecord* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRtPduDevRecord(parent);
    return sington;
}


void DbRtPduDevRecord::insertThItem(PduThresholdItem &item)
{
    int id = DbRtPduThreshold::get()->getId(item.rid, item.type, item.index);msleep(2);
    if(id < 0) {
        DbRtPduThreshold::get()->insertItem(item);
    } else {
        item.id = id;
        DbRtPduThreshold::get()->updateItem(item);
    }
}

void DbRtPduDevRecord::changeItem(PduThresholdItem &item)
{
    double rate = 10.0;
    if(item.min > 0)  item.min /= rate;
    if(item.max > 0)  item.max /= rate;
    if(item.crMin > 0)  item.crMin /= rate;
    if(item.crMax > 0)  item.crMax /= rate;
}

/**
 * @brief 数据阈值单元
 */
void DbRtPduDevRecord::slaveThDataUnit(int index, int type, PduDataUnit *dataUnit)
{   
    int len = dataUnit->value->size();
    if(type == 3) len = dataUnit->num; // 回路
    for(int i=0; i<len; ++i)
    {
        PduThresholdItem item;
        item.rid = index;
        item.type = type;
        item.index = i+1;

        item.min = dataUnit->min->get(i);
        item.max = dataUnit->max->get(i);
        item.alarm = dataUnit->alarm->get(i);
        item.crMin = dataUnit->crMin->get(i);
        item.crMax = dataUnit->crMax->get(i);
        item.crAlarm = dataUnit->crAlarm->get(i);
        changeItem(item);
//        insertThItem(item); /////========
#if defined(CLOUD_MODULE)
        CloudDevRecord::bulid()->saveThresholdItem(item);
        usleep(3);
#endif
    }
}

/**
 * @brief 保存设备阈值
 */
void DbRtPduDevRecord::slaveThreshold(int index, PduDevData *devData)
{
    int type = 1;

    slaveThDataUnit(index, type++, devData->line->vol);
    slaveThDataUnit(index, type++, devData->line->cur);
    slaveThDataUnit(index, type++, devData->loop->cur);
    slaveThDataUnit(index, type++, devData->output->cur);
    slaveThDataUnit(index, type++, devData->env->tem);
    slaveThDataUnit(index, type++, devData->env->hum);
}

/**
 * @brief 保存设备数据
 */
void DbRtPduDevRecord::rtSlave(int devType, QString &ip, int devNum)
{
    int index = slave(devType, ip, devNum);
    if(index > 0) {
        PduDataPacket *dataPacket = pdu_hd_get(devType, ip, devNum);
        slaveThreshold(index, dataPacket->data);
    }
}


/**
 * @brief 插入PDU设备索引数据
 * @param item
 */
bool DbRtPduDevRecord::insertItem(PduDevIndexItem &item)
{
    bool ret = false;
    int id = DbRtPduDevIndex::get()->getId(item.ip, item.devNum, item.devType);msleep(1);
    if(id < 0) {
        ret = DbRtPduDevIndex::get()->insertItem(item);
    } else {
        item.id = id;
        ret = DbRtPduDevIndex::get()->updateItem(item);
    }
    msleep(1);
    return ret;
}

/**
 * @brief 插入电气参数数据
 * @param item
 */
bool DbRtPduDevRecord::insertItem(PduElecParamsItem &item)
{
    bool ret =false;
    int id = DbRtPduElecParams::get()->getId(item.rid, item.type, item.index);msleep(1);
    if(id < 0) {
        ret = DbRtPduElecParams::get()->insertItem(item);
    } else {
        item.id = id;
        ret = DbRtPduElecParams::get()->updateItem(item);
    }
    msleep(1);

    return ret;
}

/**
 * @brief 插入环境数据参数
 * @param item
 */
void DbRtPduDevRecord::insertItem(PduEnvSensorItem &item)
{
    int id = DbRtPduEnvSensor::get()->getId(item.rid, item.type);msleep(1);
    if(id < 0) {
        DbRtPduEnvSensor::get()->insertItem(item);
    } else {
        item.id = id;
        DbRtPduEnvSensor::get()->updateItem(item);
    }
    msleep(1);
}
