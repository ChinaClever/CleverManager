/*
 * log_elechange.cpp
 * 转化为临时表结构
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_elechange.h"
#include "common.h"

Log_Elechange::Log_Elechange(QObject *parent) : QObject(parent)
{

}

bool Log_Elechange::roomEleChange(QVector<TgRoomItem> &items, EleTempItem &eleItem)
{
    bool ret = true;
    if(items.size() > 0) {
        eleItem.name = items.begin()->roomName;
        eleItem.startTime = items.begin()->date + " " + items.begin()->time;
        eleItem.startEle = items.begin()->ele;

        eleItem.endTime = items.last().date + " " + items.last().time;
        eleItem.endEle = items.last().ele;

        eleItem.ele = eleItem.endEle - eleItem.startEle;
    } else
        ret = false;
    return ret;
}

QString Log_Elechange::getTypeStr(int type, int index)
{
    QString str;

    switch(type) {
    case 0: str = tr("总电能");  break;
    case 1: str = tr("Line %1").arg(index);  break;
    case 2: str = tr("回路 %1").arg(index);  break;
    case 3: str = tr("输出位 %1").arg(index);  break;
    }

    return str;
}

bool Log_Elechange::roomDevChange( QVector<PduDevIndexItem> indexItem, QVector<PduElecParamsItem> &startItem,
                                  QVector<PduElecParamsItem> &endItem, QVector<EleTempItem> &eleItems)
{    
     bool ret = true;

    for(int i=0; i<endItem.size(); ++i)
    {
        EleTempItem eleItem;
        eleItem.startTime = indexItem.begin()->date + " " + indexItem.begin()->time;
        eleItem.endTime = indexItem.last().date + " " + indexItem.last().time;

        eleItem.startEle = startItem.at(i).ele;
        eleItem.endEle = endItem.at(i).ele;
        eleItem.ele = eleItem.endEle - eleItem.startEle;

        int type = endItem.at(i).type;
        int index = endItem.at(i).index;
        eleItem.name =  getTypeStr(type, index);

        eleItems.append(eleItem);
    }

    return ret;
}

bool Log_Elechange::cabEleChange(QVector<TgCabItem> &items, EleTempItem &eleItemA, EleTempItem &eleItemB)
{
    bool ret = true;
    if(items.size() > 0) {
        eleItemA.name = items.begin()->cabName+tr(" A路");
        eleItemA.startTime = items.begin()->date + " " + items.begin()->time;
        eleItemA.startEle = items.begin()->elea;

        eleItemA.endTime = items.last().date + " " + items.last().time;
        eleItemA.endEle = items.last().elea;

        eleItemA.ele = eleItemA.endEle - eleItemA.startEle;

        eleItemB.name = items.begin()->cabName+tr(" B路");
        eleItemB.startTime = items.begin()->date + " " + items.begin()->time;
        eleItemB.startEle = items.begin()->eleb;

        eleItemB.endTime = items.last().date + " " + items.last().time;
        eleItemB.endEle = items.last().eleb;

        eleItemB.ele = eleItemB.endEle - eleItemB.startEle;
    } else
        ret = false;
    return ret;
}
