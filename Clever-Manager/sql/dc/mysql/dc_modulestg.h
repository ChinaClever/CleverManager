#ifndef DC_MODULESTG_H
#define DC_MODULESTG_H
#include "dc_basictg.h"

class DC_ModulesTg : public DC_BasicTg
{
    DC_ModulesTg();
public:
    static DC_ModulesTg* bulid();
    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("modules_tg") ;}
    QString ridName(){return QString("modules_id") ;}
};

#endif // DC_MODULESTG_H
