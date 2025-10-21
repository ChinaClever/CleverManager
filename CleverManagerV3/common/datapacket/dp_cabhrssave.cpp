/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dp_cabhrssave.h"


Dp_CabHrsSave::Dp_CabHrsSave(QObject *parent) : QObject(parent)
{

}

Dp_CabHrsSave *Dp_CabHrsSave::bulid(QObject *parent)
{
    static Dp_CabHrsSave* sington = NULL;
    if(sington == NULL) {
        sington = new Dp_CabHrsSave(parent);
    }
    return sington;
}

bool Dp_CabHrsSave::saveItem(sDbCabHrsItem &item)
{
    return DbCabHrs::bulid()->insertItem(item);
}

sDbCabHrsItem Dp_CabHrsSave::initItem()
{
    sDbCabHrsItem item;
    item.room = mCab->room;
    item.modular = mCab->modular;
    item.cab = mCab->cab;
    return item;
}

bool Dp_CabHrsSave::tgObjData(const QString &road, sTgObjData &obj)
{
    bool ret = false;
    sDbCabHrsItem item = initItem();

    item.road = road;
    item.vol = obj.vol / COM_RATE_VOL;
    item.cur = obj.cur/ COM_RATE_CUR;
    item.pow = obj.pow/ COM_RATE_POW;
    item.pf = obj.pf/ COM_RATE_PF;
    item.ele = obj.ele/ COM_RATE_ELE;
    if(obj.vol > 0)
        ret = saveItem(item);

    return ret;
}

void Dp_CabHrsSave::dataPacket(const QString &road, sDataPacket *pack)
{
    if(pack) {
        if((pack->offLine > 0) &&(pack->en)) {
            tgObjData(road, pack->data.tg);
        }
    }
}



bool Dp_CabHrsSave::checkCount(int count)
{
    bool ret = false;
    ConfigBase *con = ConfigBase::bulid();
    int sec = con->item->logTimes * 60 * 60;
    if(count > sec) { // 数据纪录间隔时间
        ret =  true;
    }

    return ret;
}


void Dp_CabHrsSave::save(sCabPacket *cab)
{
    mCab = cab;
    bool ret = checkCount(cab->count);
    if(ret) {
        cab->count = 0;
        tgObjData(tr("统计"), cab->tg);
        dataPacket(tr("主路"), cab->m);
        dataPacket(tr("备路"), cab->s);
    }
}



