#ifndef DC_ROOMSTG_H
#define DC_ROOMSTG_H
#include "dc_basictg.h"

class DC_RoomsTg : public DC_BasicTg
{
    DC_RoomsTg();
public:
    static DC_RoomsTg* bulid();
    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("rooms_tg") ;}
    QString ridName(){return QString("rooms_id ") ;}
};

#endif // DC_ROOMSTG_H
