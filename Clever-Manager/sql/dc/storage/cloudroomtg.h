#ifndef CLOUDROOMTG_H
#define CLOUDROOMTG_H

#include "mysql/dc_roomstg.h"
#include "db_export/records/dbtgroom.h"
#include "cloudroom.h"

class CloudRoomTg
{
public:
    CloudRoomTg();

    bool save(TgRoomItem &item);
    bool update(TgRoomItem &item);

protected:
    void changeTo(TgRoomItem &item, DC_TgItem &dcItem);
    bool updateStatus(int rid, int status);

private:
    CloudRoom *mRoom;
    DC_RoomsTg *mTg;
};

#endif // CLOUDROOMTG_H
