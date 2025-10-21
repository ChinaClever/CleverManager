/*
 * pdudevlist.cpp
 *  PDU分组类 继承自 PduDgHash 类
 *      1、在父类的基础上，提供四个主要的接口，
 *          在线、离线、报警设备分组处理
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudglist.h"

PduDGList::PduDGList()
{
    mDevList = new PduDevList();
}

PduDGList::~PduDGList()
{
    delete mDevList;
}

/**
 * @brief 设备列表分组
 * @param type
 */
void PduDGList::startListGroup(int type)
{
    startGroup(type);
}

/**
 * @brief 在线设备列表
 * @param type
 */
void PduDGList::startLineGroup(int type)
{
    startGroup(type);

    QStringList ipList;
    mDevList->getLine(type,ipList);
    filter(ipList);
}

/**
 * @brief 报警设备列表
 * @param type
 */
void PduDGList::startAlarmGroup(int type)
{
    startGroup(type);

    QStringList ipList;
    mDevList->getAlarm(type,ipList);
    filter(ipList);
}

/**
 * @brief 离线分组
 * @param type
 */
void PduDGList::startOffLineGroup(int type)
{
    startGroup(type);

    QStringList ipList;
    mDevList->getOff(type,ipList);
    filter(ipList);
}


/**
 * @brief 从分组列表中过滤掉多余的IP
 * @param ipList
 */
void PduDGList::filter(QStringList &ipList)
{
    QString str;
    bool ret = true;

    QStringList allList;
    listAllDev(allList);

    QStringList delList;
    for(int i=0; i<allList.size(); ++i)
    {
        str = allList.at(i);
        ret = ipList.contains(str);
        if(ret == false) // 不包含
            delList << str;
    }

    for(int i=0; i<delList.size(); ++i)
    {
        str = delList.at(i);
        delDev(str);
    }
}
