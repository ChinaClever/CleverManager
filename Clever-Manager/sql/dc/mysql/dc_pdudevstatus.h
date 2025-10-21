#ifndef DC_PDUDEVSTATUS_H
#define DC_PDUDEVSTATUS_H

#include "dc_basicstatus.h"


class DC_PduDevStatus : public DC_BasicStatus
{
    Q_OBJECT
    explicit DC_PduDevStatus();
public:
    static DC_PduDevStatus* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("pdu_dev_status") ;}
    QString ridName(){return QString("pdu_devices_id") ;}
};

#endif // DC_PDUDEVSTATUS_H
