/*
 * pduhashdata.cpp
 * 从数据库中读取设备链表
 *  1、从数据库中把设备信息读取出来
 *  2、把设备数据，插入到Hash表中
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevlistread.h"



/**
 * @brief 释放内存
 * @param list
 */
static void dl_read_clear(QList<db_devStruct*> &list)
{
    db_devStruct *dev;
    for(int i=0; i<list.size(); ++i)
    {
        dev = list.at(i);
        delete dev;
    }
    list.clear();
}



/**
 * @brief 把设备插入到数据表中
 * @param devStr
 */
static void dl_read_insert(db_devStruct *devStr)
{
    int devType = devStr->type; // 获取设备类型码
    if(devType > 0)
    {
        for(int i=0; i<devStr->num; ++i)
        {
            PduDataPacket *dev = pdu_hd_get(devType, devStr->IP, i); // 根据不同的设备号 获取对应的设备Hash节点
            if(dev)
            {
                dev->devType = devType; //设备型号
                dev->ip->set(devStr->IP); // 设备IP
                dev->devNum = i; // 设备地址
                dev->offLine = 0; //设备在线标识

                // 设备地址
                dev->info->addr->area->set(devStr->area);
                dev->info->addr->group->set(devStr->group);
                dev->info->addr->cab->set(devStr->cab);

                if(i==0) // 填写主机设备名
                    dev->info->type->name->set(devStr->name);
            }
        }
    }
    else
        qDebug() << "dl_read_insert get pdu dev Code err" << devType;
}


/**
 * @brief 读取数据库中的设备列表
 */
void dl_read_db(void)
{
    QList<db_devStruct*> list;
    bool ret = db_select_devList(list); // 读取数据库中的设备
    if(ret)
    {        
        for(int i=0; i<list.size(); ++i)
            dl_read_insert(list.at(i)); // 获取数据库信息
    }
    dl_read_clear(list);
}
