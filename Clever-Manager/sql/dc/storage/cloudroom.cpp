/*
 * 只用来做试验 验证、测试用
 *
 *  Created on: 2017年1月11日
 *      Author: Lzy
 */
#include "cloudroom.h"
#include "mysql/dc_roomstatus.h"

CloudRoom::CloudRoom(QObject *parent) : QObject(parent)
{
     dc = DC_Rooms::bulid();
     mParam = CloudSetParam::bulid();
     mDbRoom = DbRoomList::get();

#if defined(CLOUD_MODULE)
     connect(mDbRoom, SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
     connect(mDbRoom, SIGNAL(updateSig(QString,QString)), this,SLOT(updateSlot(QString,QString)));
#endif
}

void CloudRoom::itemChangedSlot(int id, int type)
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

void CloudRoom::delRoom(const QString &name)
{
    int rid =  mParam->module.id;
    if(rid > 0) {
        int id = dc->getId(rid, name);
        if(id>0) DC_RoomStatus::bulid()->updateStatus(id, 3); // 0正常， 1报警 3 删除
    }
}

void CloudRoom::del(int id)
{
    QString roomName = mDbRoom->findItemById(id).first().name;
    delRoom(roomName);
}

void CloudRoom::updateSlot(QString oldName, QString name)
{
    int rid =  mParam->module.id;;
    if(rid > 0) {
        int id = dc->getId(rid, oldName);
        if(id>0)
            dc->setName(id, name);
        else
            save(id);
    }
}

CloudRoom *CloudRoom::bulid()
{
    static CloudRoom* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudRoom();
    return sington;
}

bool CloudRoom::save(int id)
{
    bool ret = false;
    int rid =  mParam->module.id;
    if(rid > 0) {
        DC_RoomItem item;
        item.modules_id = rid;
        QString name = mDbRoom->findItemById(id).first().name;
        item.name = name;

        ret = dc->containName(rid, name);
        if(ret) {
            item.id = dc->getId(rid, name);
            ret = dc->updateItem(item);
        } else {
            ret = dc->insertItem(item);
        }
    }

    return ret;
}

int CloudRoom::getRoomId(const QString &name)
{
    int id = -1;
    int rid = mParam->module.id;
    if(rid>0) {
        id = dc->getId(rid, name);
    }
    return id;
}
