/*
 * dbpdudevrecord.cpp
 * PDU设备记录表 由三个表来组成，
 *      1、PDU设备索引表（pduDevIndex）
 *      2、电气参数表（pduElecParams）
 *      3、环境状态表（pduEnvSensor）
 *
 * 设计思路：提供一个接口，传入设备类型、设备IP、设备号，来保存PDU数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbpdudevrecord.h"
#include "storage/clouddevrecord.h"
#include "db_export/realTime/dbrtpdudevrecord.h"

#define DB_SLAVE_TIME 1

int gDbDevType;

DbPduDevRecord::DbPduDevRecord(QObject *parent) : QThread(parent)
{
}

DbPduDevRecord *DbPduDevRecord::get(QObject *parent)
{
    static DbPduDevRecord* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduDevRecord(parent);
    return sington;
}

/**
 * @brief 插入PDU设备索引数据
 * @param item
 */
bool DbPduDevRecord::insertItem(PduDevIndexItem &item)
{
    return DbPduDevIndex::get()->insertItem(item);
}
/**
 * @brief 插入电气参数数据
 * @param item
 */
bool DbPduDevRecord::insertItem(PduElecParamsItem &item)
{
    return DbPduElecParams::get()->insertItem(item);
}

/**
 * @brief 插入环境数据参数
 * @param item
 */
void DbPduDevRecord::insertItem(PduEnvSensorItem &item)
{
    DbPduEnvSensor::get()->insertItem(item);
}

/**
 * @brief 保存设备索引
 * @param dataPacket
 * @return 索引号
 */
int DbPduDevRecord::slaveDevIndex(PduDataPacket *dataPacket)
{
    PduDevIndexItem item;
    item.ip = dataPacket->ip->get();
    item.devNum = dataPacket->devNum;
    item.devType = dataPacket->devType;
    item.status = dataPacket->state;
    item.devName = dataPacket->info->type->name->get();
    dataPacket->info->addr->area->get(item.roomName);
    dataPacket->info->addr->cab->get(item.cabinetName);
    if(isSave) {
        bool ret = insertItem(item);
        if(!ret) item.id = -1;
    }

#if defined(CLOUD_MODULE)
    CloudPduDev::bulid()->save(item);
#endif
    //msleep(DB_SLAVE_TIME);

    return item.id;
}


void DbPduDevRecord::changeItem(PduElecParamsItem &item)
{
    if(item.vol<0) item.vol=0;
    if(item.cur<0) item.cur=0;
    if(item.pow<0) item.pow=0;
    if(item.ele<0) item.ele=0;
    if(item.pf<0) item.pf=0;
    if(item.sw<0) item.sw=0;

    if(item.vol > 0) item.vol /= COM_RATE_VOL;
    if(item.cur > 0) item.cur /= COM_RATE_CUR;
    if(item.pow > 0) item.pow /= COM_RATE_POW;
    if(item.ele > 0) item.ele /= COM_RATE_ELE;
    if(item.pf > 0)  item.pf /= COM_RATE_PF;
}

void DbPduDevRecord::slaveElecDataUnit(int index, int type, PduObjData *objData)
{
    int len = objData->cur->value->size();
    if(type==2) { // 回路数据有待优化
        len = objData->cur->num;
    }

    static int vol =0; // 用来设置输出位电流

    for(int i=0; i<len; i++)
    {
        PduElecParamsItem item;
        item.rid = index;
        item.type = type;
        item.index = i+1;

        item.vol = objData->vol->value->get(i);
        item.cur = objData->cur->value->get(i);
        item.pow = objData->pow->get(i);
        item.ele = objData->ele->get(i);
        item.pf = objData->pf->get(i);
        item.sw = objData->sw->get(i);

        if(type == 1)
            vol = objData->vol->value->get(0);
        else if((type == 2) || (type == 3))
            item.vol = vol; // 设置输出位电流
        changeItem(item);
        if(isSave) insertItem(item);       

#if defined(CLOUD_MODULE)
        CloudDevRecord::bulid()->saveElecItem(item, isSave);
#endif
        //msleep(DB_SLAVE_TIME);
    }
}


void DbPduDevRecord::slaveTgData(int index, int type, PduDataPacket *dataPacket)
{
    PduElecParamsItem item;
    item.rid = index;
    item.type = type;
    item.index = 0;

    PduPacketTG packetTG;
    sPduTGDataStr *tg = packetTG.getTgData(dataPacket);
    if(tg) {
        item.vol = tg->vol;
        item.cur = tg->cur;
        item.pow = tg->pow;
        item.ele = tg->ele;
        item.pf = tg->pf;
        changeItem(item);
        if(isSave) insertItem(item);

#if defined(CLOUD_MODULE)
        CloudDevRecord::bulid()->devTg(item, isSave);
#endif
        msleep(DB_SLAVE_TIME);
        delete tg;
    }
}


/**
 * @brief 保存电气参数
 * @param index 索引号
 * @param devData 设备数据
 */
void DbPduDevRecord::slaveElecParams(int index,PduDataPacket *devData)
{
    //slaveTgData(index, 0, devData);
    slaveElecDataUnit(index, 1, devData->data->line);
    //slaveElecDataUnit(index, 2, devData->data->loop);
    //slaveElecDataUnit(index, 3, devData->data->output);//输出位信息
}

/**
 * @brief 只用来做试验 验证、测试用
 * @param item
 */
void DbPduDevRecord::slaveEnvCloud(PduEnvSensorItem &item)
{
    double sensor[4] = {item.sensor1, item.sensor2, item.sensor3, item.sensor4};
    for(int i=0; i<4; ++i) {
        if(sensor[i] >= 0) {
#if defined(CLOUD_MODULE)
            CloudDevRecord::bulid()->saveEnv(item. type, i+1, sensor[i], isSave);
            msleep(DB_SLAVE_TIME);
#endif
        }
    }
}


void DbPduDevRecord::slaveThUnit(int index,int type,PduDataUnit *unitData)
{
    double rate = COM_RATE_TEM;
    if(type == 2) rate = COM_RATE_HUM;

    PduEnvSensorItem item;
    item.rid = index;
    item.type = type;
    item.sensor1 = unitData->value->get(0)/rate;
    item.sensor2 = unitData->value->get(1)/rate;
    item.sensor3 = unitData->value->get(2)/rate;
    item.sensor4 = unitData->value->get(3)/rate;
    if(isSave) insertItem(item);

#if defined(CLOUD_MODULE)
    slaveEnvCloud(item);
#endif
}

void DbPduDevRecord::slaveSensor(int index,int type,PduDataBase *baseData)
{
    PduEnvSensorItem item;
    item.rid = index;
    item.type = type;
    item.sensor1 = baseData->get(0);
    item.sensor2 = baseData->get(1);
    item.sensor3 = baseData->get(2);
    item.sensor4 = baseData->get(3);
    if(isSave) insertItem(item);

#if defined(CLOUD_MODULE)
    slaveEnvCloud(item);
#endif
}

/**
 * @brief 保存环境数据
 * @param index
 * @param envData
 */
void DbPduDevRecord::slaveEnvData(int index,PduEnvData *envData)
{
    int type = 1;
    slaveThUnit(index, type++, envData->tem);
    slaveThUnit(index, type++, envData->hum);

    slaveSensor(index, type++, envData->door);
    slaveSensor(index, type++, envData->water);
    slaveSensor(index, type++, envData->smoke);
}



/**
 * @brief 设备数据保存至数据库
 * @param devType 设备类型
 * @param ip 设备IP
 * @param devNum 设备编号
 */
int DbPduDevRecord::slave(int devType, QString &ip, int devNum, bool save)
{    
    int index = -1;
    isSave = save;
    PduDataPacket *dataPacket = pdu_hd_get(devType, ip, devNum);
    if(dataPacket && (dataPacket->offLine>0)) {
        gDbDevType = dataPacket->devType;
        index = slaveDevIndex(dataPacket);
        if(index > 0) {
            slaveElecParams(index, dataPacket);
            //slaveEnvData(index,dataPacket->data->env);
        }
        //   DbRtPduDevRecord::get(this)->slaveThreshold(index, dataPacket->data); //====== thread not slave
    }

    return index;
}
