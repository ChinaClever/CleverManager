#ifndef DC_CABINETSTG_H
#define DC_CABINETSTG_H
#include "dc_basictg.h"

class DC_CabinetsTg: public DC_BasicTg
{
    DC_CabinetsTg();
public:
    static DC_CabinetsTg* bulid();
    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("cabinets_tg") ;}
    QString ridName(){return QString("cabinets_id") ;}
};

#endif // DC_CABINETSTG_H
