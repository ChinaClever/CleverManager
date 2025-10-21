/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dp_roompackets.h"

Dp_RoomPackets::Dp_RoomPackets(QObject *parent) : Dp_BasicThread(parent)
{
    mDb = DbRoomList::get();
    mPdus = Dp_PduPackets::bulid(this);
    mCabs = Dp_CabPackets::bulid(this);
    mRoomHrs = Dp_RoomHrsSave::bulid(this);
    connect(mDb,SIGNAL(itemChanged(int,int)),SLOT(roomItemChange(int,int)));
}


Dp_RoomPackets *Dp_RoomPackets::bulid(QObject *parent)
{
    static Dp_RoomPackets* sington = NULL;
    if(sington == NULL) {
        sington = new Dp_RoomPackets(parent);
    }
    return sington;
}

sRoomPacket *Dp_RoomPackets::get(const QString &room)
{
    QString key = room;
    return insert(key);
}

void Dp_RoomPackets::setPacket(RoomItem &item)
{
    QVector<sCabPacket *> cabs = mCabs->getByRoom(item.id);
    for(int i=0; i<cabs.size(); ++i) {
        sCabPacket *cab = cabs.at(i);
        cab->room = item.room;
        cab->room_id = item.id;
    }

    QVector<sDataPacket *> ps = mPdus->getByRoom(item.id);
    for(int i=0; i<ps.size(); ++i) {
        sDataPacket *p = ps.at(i);
        p->room = item.room;
        p->room_id = item.id;
    }
}

void Dp_RoomPackets::initPacket(RoomItem &it)
{
    sRoomPacket *room = get(it.room);
    if(room) {
        room->en = 1;
        room->count = 0;
        room->room_id = it.id;
        room->room = it.room;
        setPacket(it);
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents,1);
}

void Dp_RoomPackets::roomItemChange(int id, int type)
{
    if(type > 0) {
        RoomItem item = mDb->findById(id);
        initPacket(item);
    } else {
        delRoom(id);
    }
}


void Dp_RoomPackets::initFun()
{
    QVector<int> ids = mDb->selectIds();
    for(int i=0; i<ids.size(); ++i) {
        roomItemChange(ids.at(i), 1);
    }
}


sRoomPacket *Dp_RoomPackets::getByRoom(uint id)
{
    sRoomPacket *pack = nullptr;
    QHashIterator<QString, sRoomPacket *> iter(mHash);
    while(iter.hasNext()) {
        pack = iter.next().value();
        if(pack) {
            if(pack->room_id == id) {
                if(pack->en)
                    break;
            }
        }
    }

    return pack;
}

void Dp_RoomPackets::delRoom(uint id)
{
    sRoomPacket *pack = getByRoom(id);
    if(pack) {
        pack->en = 0;
    }
    mCabs->delRoom(id);
}

void Dp_RoomPackets::tgDevsCount(sRoomPacket *room, QVector<sDataPacket *> &packs)
{
    room->line = 0;
    room->alarm = 0;
    room->offline = 0;
    room->pdus = packs.size();

    for(uint i=0; i<room->pdus; ++i) {
        sDataPacket *p = packs.at(i);
        if(p->offLine > 0) {
            room->line++;
            if(p->alarm) room->alarm++;
        } else {
            room->offline++;
        }
    }
    room->cabs = mCabs->getByRoom(room->room_id).size();
}

void Dp_RoomPackets::tgRoomData(sRoomPacket *room)
{
    sTgObjData *tg = &(room->tg);
    QVector<sDataPacket *> packs = mPdus->getByRoom(room->room_id);
    room->status = mPdus->tgDataPackts(tg, packs);
    tgDevsCount(room, packs);
}

void Dp_RoomPackets::workDown(sRoomPacket *pack)
{
    if(pack->count % 4 == 0) {
        tgRoomData(pack);
        mRoomHrs->save(pack);
    }
}
