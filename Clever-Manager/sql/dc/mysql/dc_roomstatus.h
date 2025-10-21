#ifndef DC_ROOMSTATUS_H
#define DC_ROOMSTATUS_H
#include "dc_basicstatus.h"


class DC_RoomStatus: public DC_BasicStatus
{
    Q_OBJECT
    explicit DC_RoomStatus();
public:
    static DC_RoomStatus* bulid();
    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("rooms_status") ;}
    QString ridName(){return QString("rooms_id") ;}
};

#endif // DC_ROOMSTATUS_H
