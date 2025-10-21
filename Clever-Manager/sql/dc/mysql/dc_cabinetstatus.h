#ifndef DC_CABINETSTATUS_H
#define DC_CABINETSTATUS_H

#include "dc_basicstatus.h"


class DC_CabinetStatus : public DC_BasicStatus
{
    Q_OBJECT
    explicit DC_CabinetStatus();
public:
     static DC_CabinetStatus* bulid();

     QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("cabinets_status") ;}
     QString ridName(){return QString("cabinets_id") ;}
};

#endif // DC_CABINETSTATUS_H
