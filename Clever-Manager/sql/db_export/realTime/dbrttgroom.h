#ifndef DBRTTGROOM_H
#define DBRTTGROOM_H
#include "db_export/records/dbtgroom.h"

class DbRtTgRoom: public DbTgRoom
{
    Q_OBJECT
    DbRtTgRoom();

public:
    static DbRtTgRoom* get();
    virtual QString tableName(){return "rtRgRoom";}
    void updateItem(TgRoomItem &item);
    void initTime(TgRoomItem &item);
    int getId(const QString &name);
};

#endif // DBRTTGROOM_H
