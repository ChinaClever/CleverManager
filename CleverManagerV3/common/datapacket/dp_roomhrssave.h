#ifndef DP_ROOMHRSSAVE_H
#define DP_ROOMHRSSAVE_H

#include "dp_cabpackets.h"
#include "dbroomhrs.h"

class Dp_RoomHrsSave : public QObject
{
    Q_OBJECT
    explicit Dp_RoomHrsSave(QObject *parent = nullptr);
public:
    static Dp_RoomHrsSave *bulid(QObject *parent);
    void save(sRoomPacket *room);

protected:
    sDbRoomHrsItem initItem();
    bool checkCount(int count);
    bool saveItem(sDbRoomHrsItem &item);
    bool tgObjData(sTgObjData &obj);

private:
    sRoomPacket *mRoom;
};

#endif // DP_ROOMHRSSAVE_H
