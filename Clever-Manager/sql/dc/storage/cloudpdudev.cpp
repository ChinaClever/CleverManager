/*
 * 只用来做试验 验证、测试用
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "cloudpdudev.h"
#include "pdu/data/hash/read/pduhashread.h"
#include "cloudcabinet.h"
#include "mysql/dc_pdudevstatus.h"

CloudPduDev::CloudPduDev(QObject *parent) : QObject(parent)
{
    mID = -1;
    dc = DC_PduDevices::bulid();
}


CloudPduDev *CloudPduDev::bulid()
{
    static CloudPduDev* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudPduDev();
    return sington;
}

DC_PduDevItem CloudPduDev::toItem(PduDevIndexItem &item)
{
    DC_PduDevItem dcItem;
    int rid = CloudCabinet::bulid()->getId(item.roomName, item.cabinetName);
    dcItem.cabinets_id = rid;
    dcItem.type_index = item.devType;
    dcItem.name = item.devName;
    dcItem.ip = item.ip;
    dcItem.num = item.devNum;

    return dcItem;
}

bool CloudPduDev::save(PduDevIndexItem &item)
{
    bool ret = false;
    if(!item.cabinetName.isEmpty()) {
        DC_PduDevItem dcItem = toItem(item);
        ret  = dc->containDev(dcItem);
        if(ret) {
            mID = dc->getId(dcItem);
            dcItem.id = mID;
            ret = dc->updateItem(dcItem);
        } else {
            ret = dc->insertItem(dcItem);
            mID = dc->getId(dcItem);
        }
        ret = updateStatus(mID, item.status);
    } else {
        mID = -1;
    }

    return ret;
}


bool CloudPduDev::updateStatus(int rid, int status)
{
    bool ret = false;
    if(status) // 有报警
        status = 1;
    if(rid > 0)
        ret = DC_PduDevStatus::bulid()->updateStatus(rid, status); // 0正常， 1报警

    return ret;
}
