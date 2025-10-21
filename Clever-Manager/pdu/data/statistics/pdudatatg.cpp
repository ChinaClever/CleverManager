/*
 * pdupackettg.cpp
 * PDU设备统计类 提供三种方法进行数据统计
 *  1、统计所有PDU设备数据   getTgData
 *  2、针对某一类PDU设备进行数据统计  getDevTgData
 *  3、针对某一个主机PDU设备进行数据统计，统计主机及级联的所有从机的数据，getCasTgData
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudatatg.h"

PduDataTG::PduDataTG(QObject *parent) : QThread(parent)
{
    mPack = new PduPacketTG();
}

PduDataTG::~PduDataTG()
{
    delete mPack;
}


/**
 * @brief 获取这一级联组的统计数据
 * @param devHash
 * @return
 */
sPduTGDataStr *PduDataTG::getCasTgData(PduDevHash *devHash)
{
    QList<int> devList;
    devHash->list(devList); // 获取副机设备列表 级联设备

    QList<sPduTGDataStr *> tgdataList;
    for(int k=0; k<devList.size(); ++k)
    {
        PduDataPacket *data = devHash->getPacket(devList.at(k)); // 获取设备数据
        if(data)
        {
            sPduTGDataStr *tgData = mPack->getTgData(data);
            if(tgData)
                tgdataList.append(tgData);
            usleep(1);
        }
    }

    return pdu_tg_listData(tgdataList);
}

/**
 * @brief 根据某一种设备类型来统计数据
 * @param hashIP
 * @return
 */
sPduTGDataStr *PduDataTG::getDevTgData(PduHashIP *hashIP)
{
    QStringList ipList;
    hashIP->list(ipList); // 根据设备类型，获取设备IP列表

    QList<sPduTGDataStr *> tgdataList;
    for(int j=0; j<ipList.size(); ++j)
    {
        PduDevHash *devHash = hashIP->getDev(ipList.at(j));
        sPduTGDataStr *tgData = getCasTgData(devHash);
        if(tgData)
            tgdataList.append(tgData);
    }

    return pdu_tg_listData(tgdataList);
}

/**
 * @brief 统计所有设备数据
 * @return
 */
sPduTGDataStr *PduDataTG::getTgData(void)
{
    QList<int> list;
    pdu_hashData_list(list); // 获取设备类型接口

    QList<sPduTGDataStr *> tgdataList;
    for(int i=0; i<list.size(); ++i)
    {
        PduHashIP *hashIP =  pdu_hashData_getHash(list.at(i)); // 获取某种设备类型的对象
        sPduTGDataStr *tgData = getDevTgData(hashIP);
        if(tgData)
            tgdataList.append(tgData);
    }

    return pdu_tg_listData(tgdataList);
}
