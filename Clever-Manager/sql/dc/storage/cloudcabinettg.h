#ifndef CLOUDCABINETTG_H
#define CLOUDCABINETTG_H
#include "cloudroomtg.h"
#include "cloudcabinet.h"
#include "mysql/dc_cabinetstg.h"

class CloudCabinetTg
{
public:
    CloudCabinetTg();

    bool save(const QString &name, TgRoomItem &item);
    bool update(const QString &name,TgRoomItem &item);

protected:
    void changeTo(const QString &name, TgRoomItem &item, DC_TgItem &dcItem);
    bool updateStatus(int rid, int status);

private:
    CloudCabinet *mCabinet;
    DC_CabinetsTg *mTg;
};

#endif // CLOUDCABINETTG_H
