/*
 * 只用来做试验 验证、测试用
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "clouddevrecord.h"
#include "mysql/dc_pduelecparams.h"
#include "mysql/dc_envsensors.h"
#include "mysql/dc_pduthresholds.h"
#include "mysql/dc_rooms.h"
#include "mysql/dc_pdutg.h"

CloudDevRecord::CloudDevRecord(QObject *parent) : QObject(parent)
{

}

CloudDevRecord *CloudDevRecord::bulid()
{
    static CloudDevRecord* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudDevRecord();
    return sington;
}

bool CloudDevRecord::devTg(PduElecParamsItem &item, bool isSave)
{
    int rid = CloudPduDev::bulid()->getId();
    if(rid <= 0) return false;

    DC_PduTgItem tgItem;
    tgItem.pdu_devices_id = rid;
    tgItem.vol = item.vol;
    tgItem.cur = item.cur;
    tgItem.pow = item.pow;
    tgItem.ele = item.ele;
    tgItem.pf = item.pf;

    DC_PduTg *dcTg = DC_PduTg::bulid();
    bool ret = dcTg->containRid(rid);
    if((!isSave) && ret) {
        tgItem.id = dcTg->getMaxId(rid);
        dcTg->updateItem(tgItem);
    } else {
         dcTg->insertItem(tgItem);
    }

    return ret;
}

bool CloudDevRecord::saveElecItem(PduElecParamsItem &item, bool isSave)
{
    int rid = CloudPduDev::bulid()->getId();
    if(rid <= 0) return false;

    DC_PduElecParamsItem dcItem;
    dcItem.pdu_devices_id = rid;
    dcItem.type_index = item.type;
    dcItem.bit_index = item.index;
    dcItem.cur = item.cur;
    dcItem.vol = item.vol;

    dcItem.pow = item.pow;
    dcItem.ele = item.ele;
    dcItem.pf = item.pf;
    dcItem.sw = item.sw;

    DC_PduElecParams *pduParam = DC_PduElecParams::bulid();
    bool ret = pduParam->containRid(rid, item.type, item.index);
    if((!isSave) && ret) {
        dcItem.id = pduParam->getMaxId(rid, item.type, item.index);
        pduParam->updateItem(dcItem);
    } else {
        ret = pduParam->insertItem(dcItem);
    }

    return ret;
}


bool CloudDevRecord::saveEnv(int type, int num, double value, bool isSave)
{
    int rid = CloudPduDev::bulid()->getId();
    if(rid <= 0) return false;

    DC_EnvSensorItem dcItem;
    dcItem.pdu_devices_id = rid;
    dcItem.type_index = type;
    dcItem.sensor_num = num;
    dcItem.sensor_value = value;

    DC_EnvSensors *evn = DC_EnvSensors::bulid();
    bool ret = evn->containRid(rid, type, num);
    if((!isSave) && ret) {
        dcItem.id = evn->getMaxId(rid, type, num);
        evn->updateItem(dcItem);
    } else {
         evn->insertItem(dcItem);
    }
    return ret;
}


void CloudDevRecord::saveThresholdItem(PduThresholdItem &item)
{
    int rid = CloudPduDev::bulid()->getId();
    if(rid <= 0) return;

    DC_PduThresholdItem dcItem;
    dcItem.pdu_devices_id = rid;
    dcItem.type_index = item.type;
    dcItem.bit_index = item.index;

    dcItem.min = item.min;
    dcItem.max = item.max;
    dcItem.cr_min = item.crMin;
    dcItem.cr_max = item.crMax;

    int id = DC_PduThresholds::bulid()->getId(dcItem);
    if(id>0) {
        dcItem.id = id;
        DC_PduThresholds::bulid()->updateItem(dcItem);
    } else
        DC_PduThresholds::bulid()->insertItem(dcItem);
}

