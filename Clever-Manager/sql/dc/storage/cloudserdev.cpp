/*
 * 只用来做试验 验证、测试用
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "cloudserdev.h"
#include "dbroomlist.h"
#include "dbcabinetlist.h"
#include "cloudcabinet.h"

CloudSerDev::CloudSerDev(QObject *parent) : QObject(parent)
{
     dc = DC_ServerDevices::bulid();
     mDev = DbConsumerDevices::get();

#if defined(CLOUD_MODULE)
     connect(mDev, SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
     connect(mDev, SIGNAL(updateSig(int,QString)), this,SLOT(updateSlot(int,QString)));
#endif
}

CloudSerDev *CloudSerDev::bulid()
{
    static CloudSerDev* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudSerDev();
    return sington;
}

void CloudSerDev::itemChangedSlot(int id,int type)
{
    switch (type)
    {
    case Insert:
        save(id);
        break;
    case Remove:

        break;
    default:
        break;
    }
}


DC_SerDevItem CloudSerDev::toItem(int id)
{
    ConsumerDeviceItem item = mDev->findItemById(id).first();
    CabinetItem cabItem = DbCabinetList::get()->findItemById(item.cabinet_id).first();
    QString roomName = DbRoomList::get()->findItemById(cabItem.room_id).first().name;
    int rid = CloudCabinet::bulid()->getId(roomName, cabItem.name);

    DC_SerDevItem dcItem;
    dcItem.cabinets_id = rid;
    dcItem.name = item.name;
    dcItem.main_pdu_port = item.main_pdu_port;
    dcItem.spare_pdu_port = item.spare_pdu_port;
    dcItem.address = item.address;
    dcItem.height = item.occupy_height;

    return dcItem;
}

void CloudSerDev::save(int id)
{
    DC_SerDevItem dcItem = toItem(id);
    modify(dcItem, dcItem.name);
}

void CloudSerDev::updateSlot(int id, QString name)
{
    DC_SerDevItem dcItem = toItem(id);
    modify(dcItem, name);
}

void CloudSerDev::modify(DC_SerDevItem &dcItem, QString &name)
{
    bool ret = dc->containName(dcItem.cabinets_id, name);
    if(ret) {
        dcItem.id = dc->getId(dcItem.cabinets_id, name);
        dc->updateItem(dcItem);
    } else
         dc->insertItem(dcItem);
}

void CloudSerDev::del(int id)
{
    DC_SerDevItem dcItem = toItem(id);
    id = dc->getId(dcItem.cabinets_id, dcItem.name);
    if(id>0)
        dc->removeById(id);
}
