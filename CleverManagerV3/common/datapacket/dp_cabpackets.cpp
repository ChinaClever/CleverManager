/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dp_cabpackets.h"
#define MAX(a,b) (a>b?a:b)

Dp_CabPackets::Dp_CabPackets(QObject *parent) : Dp_BasicThread(parent)
{
    mCount = 0;
    mDb = DbCabinetList::get();
    mPdus = Dp_PduPackets::bulid(this);
    mCabHrs = Dp_CabHrsSave::bulid(this);
    mAlarm = Dp_CabAlarm::bulid(this);
    connect(mDb,SIGNAL(itemChanged(int,int)),SLOT(cabinetItemChange(int,int)));
}


Dp_CabPackets *Dp_CabPackets::bulid(QObject *parent)
{
    static Dp_CabPackets* sington = NULL;
    if(sington == NULL) {
        sington = new Dp_CabPackets(parent);
    }
    return sington;
}



sCabPacket *Dp_CabPackets::get(const QString &room, const QString &cab)
{
    QString key = room +";"+ cab;
    return insert(key);
}

void Dp_CabPackets::setPacket(CabinetItem &it, sDataPacket *pack)
{
    pack->en = 1;
    pack->cab = it.cab;
    pack->cab_id = it.id;
    pack->room = it.room;
    pack->room_id = it.room_id;
}


void Dp_CabPackets::initPacket(CabinetItem &it)
{
    sCabPacket *cab = get(it.room, it.cab);
    if(cab) {
        cab->en = 1;
        cab->count = 0;
        cab->room_id = it.room_id;
        cab->room = it.room;
        cab->cab_id = it.id;
        cab->cab = it.cab;
        cab->pow = it.pow;
        cab->powAlarm = 0;
        memset(&(cab->tg), 0, sizeof(sTgObjData));

        if(!it.main_ip.isEmpty()) {
            cab->m = mPdus->get(it.main_ip, it.main_num);
            setPacket(it, cab->m);
        } else {
            cab->m = nullptr;
        }

        if(!it.spare_ip.isEmpty()) {
            cab->s = mPdus->get(it.spare_ip, it.spare_num);
            setPacket(it, cab->s);
        } else {
            cab->s = nullptr;
        }
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents,1);
}

void Dp_CabPackets::cabinetItemChange(int id, int type)
{
    if(type > 0) {
        CabinetItem item = mDb->findById(id);
        initPacket(item);
    } else {
        delCab(id);
    }
}

void Dp_CabPackets::initFun()
{
    QVector<int> ids = mDb->selectIds();
    for(int i=0; i<ids.size(); ++i) {
        cabinetItemChange(ids.at(i), 1);
    }
}

sCabPacket *Dp_CabPackets::getByCab(uint id)
{
    sCabPacket *pack = nullptr;
    QHashIterator<QString, sCabPacket *> iter(mHash);
    while(iter.hasNext()) {
        pack = iter.next().value();
        if(pack) {
            if(pack->cab_id == id) {
                if(pack->en)
                    break;
            }
        }
    }

    return pack;
}

QVector<sCabPacket *> Dp_CabPackets::getByRoom(uint id)
{
    sCabPacket *pack = nullptr;
    QVector<sCabPacket *> packs;
    QHashIterator<QString, sCabPacket *> iter(mHash);

    while(iter.hasNext()) {
        pack = iter.next().value();
        if(pack) {
            if(pack->room_id == id) {
                if(pack->en)
                    packs.append(pack);
            }
        }
    }

    return packs;
}

void Dp_CabPackets::dels(QVector<sCabPacket *> &packs)
{
    sCabPacket *pack = nullptr;
    for(int i=0; i<packs.size(); ++i) {
        pack = packs.at(i);
        pack->en = 0;
    }
}

void Dp_CabPackets::delCab(uint id)
{
    sCabPacket *pack = getByCab(id);
    if(pack) {
        pack->en = 0;
    }
    mPdus->delCab(id);
}

void Dp_CabPackets::delRoom(uint id)
{
    QVector<sCabPacket *> packs = getByRoom(id);
    dels(packs);
    mPdus->delRoom(id);
}

int Dp_CabPackets::getStatus(sDataPacket *pack)
{
    int ret = -1; //-1表示没有Pdu设备,0表示正常, 1表示离线，2表示警告
    if(pack)
    {
        if((pack->offLine > 0) && pack->en)
            ret = pack->alarm == 0?0:2;
        else
            ret = 1;
    }

    return ret;
}


QColor Dp_CabPackets::getColor(int status)
{
    QColor color(cm::gray);

    switch (status) {
    case -1: color = cm::gray; break;
    case 0: color =  cm::green; break;
    case 1: color =  cm::blue; break;
    case 2: color =  cm::orange; break;
    case 3: color =  cm::yellow; break;
    }

    return color;
}



void Dp_CabPackets::tgCabData(sCabPacket *cab)
{
    sTgObjData *tg = &(cab->tg);
    QVector<sDataPacket *> packs;
    packs << cab->m << cab->s;
    mPdus->tgDataPackts(tg, packs);

    cab->status = MAX(getStatus(cab->m), getStatus(cab->s));
    if(cab->powAlarm) cab->status = 2;

    cab->color = getColor(cab->status);
    cab->tmpColor = cm_temp2Color(cab->tg.tem/COM_RATE_TEM);
}

void Dp_CabPackets::workDown(sCabPacket *pack)
{
    if(mCount++ % 3 == 0) {
        tgCabData(pack);
        mCabHrs->save(pack);
        mAlarm->alarm(pack);
    }
}

