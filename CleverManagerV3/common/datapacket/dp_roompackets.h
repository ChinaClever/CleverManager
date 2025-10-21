#ifndef DP_ROOMPACKETS_H
#define DP_ROOMPACKETS_H

#include "dp_roomhrssave.h"

class Dp_RoomPackets : public Dp_BasicThread<sRoomPacket *>
{
    Q_OBJECT
    explicit Dp_RoomPackets(QObject *parent = nullptr);
public:
    static Dp_RoomPackets *bulid(QObject *parent);

    sRoomPacket *get(const QString &room);
    sRoomPacket *getByRoom(uint id);
    void delRoom(uint id);

protected:
    void initFun();
    void setPacket(RoomItem &item);
    void initPacket(RoomItem &it);
    void tgRoomData(sRoomPacket *room);
    void workDown(sRoomPacket *pack);
    void tgDevsCount(sRoomPacket *room, QVector<sDataPacket *> &packs);
    sRoomPacket *newDataPacket() {return new sRoomPacket;}

private slots:
    void roomItemChange(int, int);

private:
    DbRoomList *mDb;
    Dp_PduPackets *mPdus;
    Dp_CabPackets *mCabs;
    Dp_RoomHrsSave *mRoomHrs;
};

#endif // DP_ROOMPACKETS_H
