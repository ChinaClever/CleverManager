#ifndef PDUDATATG_H
#define PDUDATATG_H
#include "pdupackettg.h"

class PduDataTG: public QThread
{
public:
    PduDataTG(QObject *parent = 0);
    ~PduDataTG();

    sPduTGDataStr *getCasTgData(PduDevHash *devHash);
    sPduTGDataStr *getDevTgData(PduHashIP *hashIP);
    sPduTGDataStr *getTgData(void);

private:
    PduPacketTG *mPack;
};

#endif // PDUDATATG_H
