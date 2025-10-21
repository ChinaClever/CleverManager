/*
 * pdudevlist.cpp
 *  PDU分组Hash类
 *      1、设备组的操作接口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudghash.h"
#include "ipsort.h"

PduDgHash::PduDgHash()
{
    mLock = new QReadWriteLock();
    mGroupHash = new dg_groupHash;
}

PduDgHash::~PduDgHash()
{
    delete mLock;
    delete mGroupHash;
}

/**
 * @brief 根据分组，IP增加数据
 * @param group
 * @param ip
 */
void PduDgHash::add(QString &group, QString &ip)
{
    QWriteLocker locker(mLock);
    mGroupHash->insert(group, ip);
}


/**
 * @brief 根据设备类型、进行分组
 * @param type
 * @param id
 */
void PduDgHash::startGroup(int type)
{
    mGroupHash->clear();

    QString group, ip;
    PduHashIP *hashIP = pdu_hashData_getHash(type); // 获取某种设备类型的对象
    if(hashIP)
    {
        QStringList ipList;
        hashIP->list(ipList); // 根据设备类型，获取设备IP列表

        for(int j=0; j<ipList.size(); ++j)
        {
            PduDevHash *devHash = hashIP->getDev(ipList.at(j));// 获取设备对象
            if(devHash)
            {
                PduDataPacket *data = devHash->getMaster(); // 获取设备数据
                if(data)
                {
                    group = data->info->addr->group->get();
                    ip = data->ip->get();
                    add(group, ip);
                }
            }
        }
    }
}


/**
 * @brief 组列表
 * @param name
 * @return
 */
int PduDgHash::listGroup(QStringList &name)
{
    int num = mGroupHash->size();
    if(num>0)
    {
        QReadLocker locker(mLock);
        dg_groupHash::const_iterator i;
        for(i=mGroupHash->constBegin(); i!=mGroupHash->constEnd(); ++i)
        {
            if(!name.contains(i.key()))
                name << i.key();
        }

        qSort(name);
    }

    return num;
}

/**
 * @brief 根据组来获取设备列表
 * @param group
 * @param devList
 * @return
 */
int PduDgHash::listDev(QString &group,QStringList &devList)
{
    QReadLocker locker(mLock);
    devList = mGroupHash->values(group);
    ip_sort_method(devList); // ip 地址进行排序

    return devList.size();
}

/**
 * @brief 组重命名
 * @param oldName
 * @param newName
 * @return
 */
bool PduDgHash::renameGroup(QString &oldName,QString &newName)
{
    bool ret = mGroupHash->contains(oldName);
    if(ret) /*存在*/
    {
        QStringList list = mGroupHash->values(oldName);
        for(int i=0; i<list.size(); ++i)
            add(newName,list[i]);
        delGroup(oldName);
    }

    return ret;
}


/**
 * @brief 根据IP查找组
 * @param ip
 * @param group
 * @return
 */
bool PduDgHash::findGroup(QString &ip,QString &group)
{
    bool ret = false;

    QReadLocker locker(mLock);
    dg_groupHash::const_iterator it;
    for(it=mGroupHash->constBegin(); it!=mGroupHash->constEnd(); ++it)
    {
        if(ip == it.value())
        {
            group = it.key();
            ret = true;
            break;
        }
    }

    return ret;
}

/**
 * @brief 是否包含此设备IP
 * @param str
 * @return
 */
bool PduDgHash::containsDev(QString &ip)
{
    bool ret = false;

    QReadLocker locker(mLock);
    dg_groupHash::const_iterator it;
    for(it=mGroupHash->constBegin(); it!=mGroupHash->constEnd(); ++it)
    {
        if(ip == it.value())
        {
            ret = true;
            break;
        }
    }

    return ret;
}

/**
 * @brief 在某一组中是否包含 这个设备
 * @param group
 * @param ip
 * @return
 */
bool PduDgHash::containsDev(QString &group, QString &ip)
{
    QStringList devList;
    listDev(group, devList);

    return devList.contains(ip);
}

/**
 * @brief 是否包含此设备组
 * @param group
 * @return
 */
bool PduDgHash::containsGroup(QString &group)
{
    QReadLocker locker(mLock);

    return mGroupHash->contains(group);
}

/**
 * @brief 插入一个空组
 * @param group
 */
void PduDgHash::insertGroup(QString &group)
{
    QString ip;
    add(group, ip); // 增加一个空组
}

/**
 * @brief 移动设备
 * @param ip
 * @param group
 * @return
 */
void PduDgHash::moveDev(QString &ip, QString &group)
{
    delDev(ip);
    add(group,ip);
}

/**
 * @brief 获取所有设备
 * @param list
 * @return
 */
int PduDgHash::listAllDev(QStringList &list)
{
    QReadLocker locker(mLock);
    dg_groupHash::const_iterator it;
    for(it=mGroupHash->constBegin(); it!=mGroupHash->constEnd(); ++it)
        list << it.value();
    ip_sort_method(list);

    return list.size();
}

/**
 * @brief 删除一个设备
 * @param group
 * @param ip
 */
void PduDgHash::del(QString &group, QString &ip)
{
    //    QWriteLocker locker(mLock);
    //    mGroupHash->remove(group, ip);

    delDev(ip);
}

void PduDgHash::delDev(QString &ip)
{
    QMutableHashIterator<QString,QString> it(*mGroupHash);
    while(it.hasNext())
    {
        if(ip == it.next().value())
        {
            QWriteLocker locker(mLock);
            it.remove();
        }
    }
}

void PduDgHash::delGroup(QString &group)
{
    QWriteLocker locker(mLock);
    mGroupHash->remove(group);
}

void PduDgHash::clear(void)
{
    QWriteLocker locker(mLock);
    mGroupHash->clear();
}




