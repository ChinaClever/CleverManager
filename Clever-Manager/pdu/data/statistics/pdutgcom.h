#ifndef PDUTGCOM_H
#define PDUTGCOM_H

#include "pduhashdata.h"

/**
 * PDU设备统计数据结构体
 */
typedef struct
{
    int vol; // 电压
    qint64 cur; // 电流
    qint64 pow; // 功率
    qint64 ele; // 电能

    int tem; // 温度
    int hum; // 湿度

    int pf; // 功率因数
    int carbon; // 排C量
    int rate; // 电压频率
}sPduTGDataStr;

sPduTGDataStr *pdu_tg_listData(QList<sPduTGDataStr *> &listData);

#endif // PDUTGCOM_H
