#ifndef CLOUDCABINET_H
#define CLOUDCABINET_H

#include <QObject>
#include "cloudroom.h"
#include "mysql/dc_cabinetsspare.h"
#include "dbcabinetlist.h"
#include "mysql/dc_cabinetstatus.h"

class CloudCabinet : public QObject
{
    Q_OBJECT
    explicit CloudCabinet(QObject *parent = 0);
public:
    static CloudCabinet* bulid();

    int getId(const QString &room, const QString &cabinet);
    void del(int id);

protected:
    DC_CabinetItem toDcItem(int id, CabinetItem &item);

    void delMainDev(int rid);
    void delSpareDev(int rid);
    void saveMainDev(int rid, const CabinetItem &item);
    void saveSpareDev(int rid, const CabinetItem &item);
    bool modify(DC_CabinetItem &dcItem, CabinetItem &item, QString &name);

    bool save(int id);

protected slots:
    void itemChangedSlot(int, int);
    void updateSlot(int, QString);

private:
    DC_Cabinets *dc;
    DbCabinetList *mDbCabinet;
    enum{Remove,Insert,Update};
};

#endif // CLOUDCABINET_H
