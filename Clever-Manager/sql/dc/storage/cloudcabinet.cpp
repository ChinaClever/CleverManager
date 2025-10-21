/*
 * 只用来做试验 验证、测试用
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "cloudcabinet.h"
#include "cloudpdudev.h"
#include "cloudserdev.h"

CloudCabinet::CloudCabinet(QObject *parent) : QObject(parent)
{
    CloudSerDev::bulid();
    dc = DC_Cabinets::bulid();
    mDbCabinet = DbCabinetList::get();

#if defined(CLOUD_MODULE)
    connect(mDbCabinet, SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
    connect(mDbCabinet, SIGNAL(updateSig(int,QString)), this,SLOT(updateSlot(int,QString)));
#endif
}

void CloudCabinet::itemChangedSlot(int id, int type)
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


void CloudCabinet::del(int id)
{
    CabinetItem item = mDbCabinet->findItemById(id).first(); ////====== 存在问题
    QString roomName = DbRoomList::get()->findItemById(item.room_id).first().name; ////====== 存在问题
    int rid = CloudRoom::bulid()->getRoomId(roomName); // 获取机房ID
    rid = dc->getId(rid, item.name);
    if(rid>0) {
        DC_CabinetStatus::bulid()->updateStatus(rid, 3);
        delMainDev(rid);
        delSpareDev(rid);
    }
}

void CloudCabinet::delMainDev(int rid)
{
    DC_CabinetsMain *cab = DC_CabinetsMain::bulid();
    int id = cab->getId(rid);
    if(id>0)
        cab->removeById(id);
}

void CloudCabinet::delSpareDev(int rid)
{
    DC_CabinetsSpare *cab = DC_CabinetsSpare::bulid();
    int id = cab->getId(rid);
    if(id>0)
        cab->removeById(id);
}

DC_CabinetItem CloudCabinet::toDcItem(int id, CabinetItem &item)
{
    item = mDbCabinet->findItemById(id).first(); ////====== 存在问题
    QString roomName = DbRoomList::get()->findItemById(item.room_id).first().name; ////====== 存在问题
    int rid = CloudRoom::bulid()->getRoomId(roomName); // 获取机房ID

    DC_CabinetItem dcItem;
    dcItem.rooms_id = rid;
    dcItem.name = item.name;
    dcItem.height = item.height;
    dcItem.row_index = item.row;
    dcItem.column_index = item.column;

    return dcItem;
}


void CloudCabinet::saveMainDev(int rid, const CabinetItem &item)
{
    DC_CabinetPduItem dcItem;
    dcItem.cabinets_id = rid;
    dcItem.dev_type = item.main_type;
    dcItem.ip = item.main_ip;
    dcItem.dev_num = item.main_num;

    QString ip = item.main_ip;
    DC_CabinetsMain *cab = DC_CabinetsMain::bulid();
    int id = cab->getId(rid);
    if(!ip.isEmpty()) {
        if(id>0) {
            dcItem.id = id;
            cab->updateItem(dcItem);
        } else {
            cab->insertItem(dcItem);
        }
    } else if(id>0){
        cab->removeById(id);
    }
}

void CloudCabinet::saveSpareDev(int rid, const CabinetItem &item)
{
    DC_CabinetPduItem dcItem;
    dcItem.cabinets_id = rid;
    dcItem.dev_type = item.spare_type;
    dcItem.ip = item.spare_ip;
    dcItem.dev_num = item.spare_num;

    QString ip = item.spare_ip;
    DC_CabinetsSpare *cab = DC_CabinetsSpare::bulid();
    int id = cab->getId(rid);
    if(!ip.isEmpty()) {
        if(id>0) {
            dcItem.id = id;
            cab->updateItem(dcItem);
        } else {
            cab->insertItem(dcItem);
        }
    } else if(id>0) {
        cab->removeById(id);
    }
}


bool CloudCabinet::save(int id)
{
    CabinetItem item;
    DC_CabinetItem dcItem = toDcItem(id, item);

    return modify(dcItem, item, dcItem.name);
}


CloudCabinet *CloudCabinet::bulid()
{
    static CloudCabinet* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudCabinet();
    return sington;
}


bool CloudCabinet::modify(DC_CabinetItem &dcItem, CabinetItem &item, QString &name)
{
    bool ret = dc->containName(dcItem.rooms_id, name);
    if(ret) {
        dcItem.id = dc->getId(dcItem.rooms_id, name);
        ret = dc->updateItem(dcItem);
    } else {
        ret = dc->insertItem(dcItem);
    }

    if(ret) {
        int id = dc->getId(dcItem.rooms_id, dcItem.name);

        saveMainDev(id, item);
        saveSpareDev(id, item);
    }
    return ret;
}

void CloudCabinet::updateSlot(int id, QString name)
{
    CabinetItem item;
    DC_CabinetItem dcItem = toDcItem(id, item);

    modify(dcItem, item, name);
}

int CloudCabinet::getId(const QString &room, const QString &cabinet)
{
    int rid = CloudRoom::bulid()->getRoomId(room);

    return dc->getId(rid, cabinet);
}
