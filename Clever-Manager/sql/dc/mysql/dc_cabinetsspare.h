#ifndef DC_CABINETSSPARE_H
#define DC_CABINETSSPARE_H
#include "dc_cabinetsmain.h"

class DC_CabinetsSpare : public DC_CabinetsMain
{
    DC_CabinetsSpare();
public:
    static DC_CabinetsSpare* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("cabinets_pdu_spare") ;}
};

#endif // DC_CABINETSSPARE_H
