/*
 * pdupackettg.cpp
 * PDU设备数据包统计类
 *  1、针对某一设备进行数据统计，
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdupackettg.h"
extern PduDataPacket *pdu_hd_getPacket(QString &ip, int num);

PduPacketTG::PduPacketTG()
{

}

PduPacketTG::~PduPacketTG()
{
}

/**
 * @brief 累积和
 * @param data
 * @return
 */
int PduPacketTG::addData(PduDataBase *data)
{
    int value=0;

    int size = data->size();
    if(size > 0)
    {
        for(int i=0; i<size; ++i)
            value += data->get(i);
    }

    return value;
}

/**
 * @brief 获取平均数据 0视无效数据
 * @param data
 */
int PduPacketTG::averData(PduDataBase *data)
{
    int len = data->size();
    int size=0, value=0,temp;

    for(int i=0; i< len; ++i)
    {
        temp = data->get(i);
        if(temp > 0)
        {
            size++;
            value += temp;
        } else
            temp = 0;
    }

    if(size > 0)
        value /= size;

    return value;

//    int value = addData(data);

//    int size = data->size();
//    if(size > 0)
//        value /= size;

//    return value;
}


/**
 * @brief 每一个PDU的统计数据
 * @param data
 * @return
 */
sPduTGDataStr *PduPacketTG::getTgData(PduDataPacket *packet)
{
    sPduTGDataStr *tg = NULL;

    if(packet->offLine > 0) // 必须在线
    {
        tg = new sPduTGDataStr;
        memset(tg, 0, sizeof(sPduTGDataStr));

        PduDevData *data = packet->data;
        tg->cur = addData(data->line->cur->value); // 总电流值
        tg->vol = averData(data->line->vol->value); // 平均电压

        tg->pow = addData(data->line->pow); // 总功率
        tg->ele = addData(data->line->ele); // 总功率

        tg->pf = averData(data->line->pf); // 平均功率因数
        tg->carbon = addData(data->line->carbon); // 总排C量
        tg->rate = averData(data->line->rate); // 平均电压频率

        tg->tem = averData(data->env->tem->value); // 平均温度
        tg->hum = averData(data->env->hum->value); // 平均湿度
    }

    return tg;
}

sPduTGDataStr *PduPacketTG::getTgData(QString &ip, int num)
{
    PduDataPacket *packet = pdu_hd_getPacket(ip,num);
    if(packet)
        return getTgData(packet);

    return NULL;
}
