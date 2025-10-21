/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dp_pduhrssave.h"

Dp_PduHrsSave::Dp_PduHrsSave(QObject *parent) : QObject(parent)
{

}


Dp_PduHrsSave *Dp_PduHrsSave::bulid(QObject *parent)
{
    static Dp_PduHrsSave* sington = NULL;
    if(sington == NULL) {
        sington = new Dp_PduHrsSave(parent);
    }
    return sington;
}



bool Dp_PduHrsSave::saveItem(sDbPduHrsItem &item)
{
    return DbPduHrs::bulid()->insertItem(item);
}

sDbPduHrsItem Dp_PduHrsSave::initItem()
{
    sDbPduHrsItem item;
    item.room = mPack->room;
    item.modular = mPack->modular;
    item.cab = mPack->cab;
    item.road = mPack->road;
    item.dev_type = mPack->dev_type;
    item.ip = mPack->net.ip;
    item.dev_num = mPack->dev_num;

    return item;
}

QString Dp_PduHrsSave::getSw(int sw)
{
    QString str = tr("打开");
   if(sw) str = tr("关闭");

   return str;
}


bool Dp_PduHrsSave::objData(sObjData &obj)
{
    bool ret = false;
    sDbPduHrsItem item = initItem();
    for(int i=0; i<obj.size; ++i) {
        item.item = obj.name[i];
        item.sw = getSw(obj.sw[i]);
        item.vol = obj.vol.value[i] / COM_RATE_VOL;
        item.cur = obj.cur.value[i] / COM_RATE_CUR;
        item.pow = obj.pow[i]  / COM_RATE_POW;
        item.pf = obj.pf[i]  / COM_RATE_PF;
        item.ele = obj.ele[i]  / COM_RATE_ELE;
        ret = saveItem(item);
    }

    return ret;
}

void Dp_PduHrsSave::devData(sDevData &dev)
{
    objData(dev.line); // 相数据
    objData(dev.loop); // 回路数据
    objData(dev.output); //位数据
}

bool Dp_PduHrsSave::checkCount(int count)
{
    bool ret = false;
    ConfigBase *con = ConfigBase::bulid();
    int sec = con->item->logTimes * 60 * 60;
    if(count > sec) { // 数据纪录间隔时间
        ret =  true;
    }

    return ret;
}

bool Dp_PduHrsSave::save(sDataPacket *pack)
{
    bool ret = false;
    mPack = pack;

    if(pack->offLine > 0) {
        ret = checkCount(pack->count);
        if(ret) {
            devData(pack->data);
            pack->count = 0;
        }
    }

    return ret;
}
