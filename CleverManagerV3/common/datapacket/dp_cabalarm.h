#ifndef DP_CABALARM_H
#define DP_CABALARM_H

#include "dp_pdupackets.h"
#include "dbcabalarm.h"

class Dp_CabAlarm : public QObject
{
    Q_OBJECT
    explicit Dp_CabAlarm(QObject *parent = nullptr);
public:
    static Dp_CabAlarm *bulid(QObject *parent);
    void alarm(sCabPacket *cab);
};

#endif // DP_CABALARM_H
