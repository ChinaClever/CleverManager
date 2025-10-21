#ifndef PDUPACKETTG_H
#define PDUPACKETTG_H
#include "pdutgcom.h"
#include "common.h"

class PduPacketTG
{
public:
    PduPacketTG();
    ~PduPacketTG();

    sPduTGDataStr *getTgData(QString &ip, int num);
    sPduTGDataStr *getTgData(PduDataPacket *packet);

private:
    int addData(PduDataBase *data);
    int averData(PduDataBase *data);
};

#endif // PDUPACKETTG_H
