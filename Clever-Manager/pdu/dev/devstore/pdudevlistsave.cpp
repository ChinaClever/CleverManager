/*
 * db_devlist.cpp
 * 把备保存至数据库中
 *  1、有新设备，会调用 dl_save_add() 方法，把设备IP保存至链表中
 *  2、保存线程，会调用 dl_save_devData() 方法，把设备保存至数据库中
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevlistsave.h"

static QReadWriteLock *gLock = new QReadWriteLock();
static QStringList *gDevList = new QStringList(); /*新设备 会加入到此链表中*/


/**
 * @brief 需要保存的设备信息加至队列中
 * @param type
 * @param ip
 */
void dl_save_add(QString &ip)
{
    bool ret = gDevList->contains(ip);
    if(ret == false)
    {
        QWriteLocker locker(gLock);
        if(!ip.isEmpty()) {
            gDevList->append(ip);
        }
    }
}

/**
 * @brief 把设备信息保存到数据库
 */
static void dl_save_db(int num, PduDataPacket *packet)
{
    db_devStruct *devStr = new db_devStruct;

    devStr->IP = packet->ip->get();
    devStr->type = packet->devType;
    devStr->num = num; // 设备级联数量
    devStr->name = packet->info->type->name->get();

    devStr->area = packet->info->addr->area->get();
    devStr->group = packet->info->addr->group->get();
    devStr->cab = packet->info->addr->cab->get();

    //    if(devStr->group.isEmpty()) //==== 调试用 默认分组
    //        devStr->group = "PDU";

    db_insert_devList(devStr); // 更新数据库
    delete devStr;
}

static bool dl_save_struct(QString &ip)
{
    bool ret = false;
    PduDevHash *devHash = pdu_hd_get(ip);
    if(devHash)
    {
        PduDataPacket *packet = devHash->getMaster();
        if(packet) {
            int num = devHash->size();
            dl_save_db(num, packet);
            ret = true;
        }
    }
    return ret;
}

/**
 * @brief 保存设备信息
 */
int dl_save_devData(void)
{        
    int size = gDevList->size();
    if(size > 0)
    {
        QString ip = gDevList->first();
        bool ret = dl_save_struct(ip);
        if(ret) {
            QWriteLocker locker(gLock);
            gDevList->removeFirst();
        }
    }

    return gDevList->size();
}


