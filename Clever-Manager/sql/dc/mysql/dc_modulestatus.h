#ifndef DC_MODULESTATUS_H
#define DC_MODULESTATUS_H
#include "dc_basicstatus.h"

class DC_ModuleStatus : public DC_BasicStatus
{
    DC_ModuleStatus();
public:
    static DC_ModuleStatus* bulid();
    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("modules_status") ;}
    QString ridName(){return QString("modules_id") ;}
};

#endif // DC_MODULESTATUS_H
