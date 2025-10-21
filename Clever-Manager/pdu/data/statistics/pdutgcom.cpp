/*
 * pdutgcom.cpp
 * PDU设备数据统计公共接口
 *  1、统计结构体的定义
 *  2、提供结构体链表统计接口，sPduTGDataStr *pdu_tg_listData(QList<sPduTGDataStr *> &listData)
 *      此接口，会把链表中的数据进行统计，然后释放内存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdutgcom.h"

/**
 * @brief 释放内存
 * @param listData
 */
static void pdu_tg_clear(QList<sPduTGDataStr *> &listData)
{
    sPduTGDataStr *ptr = NULL;
    int size = listData.size();

    for(int i=0; i<size; ++i) // 释放内存
    {
        ptr = listData.at(i);
        delete ptr;
    }

    listData.clear();
}



/**
 * @brief 统计链表数据
 * @param listData
 * @return
 */
sPduTGDataStr *pdu_tg_listData(QList<sPduTGDataStr *> &listData)
{
    if(listData.size() == 0)
        return NULL;

    sPduTGDataStr *tg = new sPduTGDataStr();
    memset(tg, 0, sizeof(sPduTGDataStr));

    sPduTGDataStr *ptr = NULL;
    int size = listData.size();
    int envSize=0, pfSize=0, volSize=0;

    for(int i=0; i<size; ++i) // 部和
    {
        ptr = listData.at(i);

        tg->vol += ptr->vol;
        if(ptr->vol > 0)
            volSize++;

        tg->cur += ptr->cur;
        tg->pow += ptr->pow;
        tg->ele += ptr->ele;

        tg->pf += ptr->pf;
        if(ptr->pf > 0)
            pfSize++;

        tg->carbon += ptr->carbon;
        tg->rate += ptr->rate;

        tg->tem += ptr->tem;
        if(ptr->tem > 0)
            envSize++;

        tg->hum += ptr->hum;
    }

    if(volSize) tg->vol /= volSize;
    if(size) tg->rate /= size;
    if(envSize)  tg->tem /= envSize;
    if(envSize)   tg->hum /= envSize;
    if(pfSize)  tg->pf /= pfSize;

    if(size > 0) // 取平均值
    {
        pdu_tg_clear(listData); // 内存释放
    }

    return tg;
}

