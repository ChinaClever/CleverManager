#ifndef CLOUDMODULETG_H
#define CLOUDMODULETG_H
#include "mysql/dc_modulestg.h"
#include "db_export/records/dbtgroom.h"
#include "cloudsetparam.h"

class CloudModule
{
public:
    CloudModule();

    bool save(TgRoomItem &item);
    bool update(TgRoomItem &item);

protected:
    void changeTo(TgRoomItem &item, DC_TgItem &dcItem);
    bool saveTg(DC_TgItem &item);
    bool updateStatus(int status);

private:
    DC_ModulesTg *mTg;
};

#endif // CLOUDMODULETG_H
