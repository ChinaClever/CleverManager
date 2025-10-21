/*
 * pdudevdel.cpp
 * 从数据库中删除设备
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevdel.h"

static QReadWriteLock *gLock = new QReadWriteLock();
static QStringList *gDevList = new QStringList(); /*新设备 会加入到此链表中*/

/**
 * @brief 需要删除的设备信息加至队列中
 * @param type
 * @param ip
 */
void dl_del_add(const QString &ip)
{
    bool ret = gDevList->contains(ip);
    if(ret == false)
    {
        QWriteLocker locker(gLock);
        gDevList->append(ip);
    }
}



/**
 * @brief 删除设备信息
 */
int dl_del_devData(void)
{
    int size = gDevList->size();
    if(size>0)
    {
        QString ip = gDevList->first();
        db_del_devList(ip);

        QWriteLocker locker(gLock);
        gDevList->removeFirst();
    }
    return gDevList->size();
}
