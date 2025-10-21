#ifndef DBROOMELEC_H
#define DBROOMELEC_H
#include "sqltablewid.h"
#include "dbroomhrs.h"

struct sRoomElecItem : public DbBasicElecItem{
    QString room;
};


class DbRoomElec: public SqlBasic<sRoomElecItem>
{
    DbRoomElec();
public:
    static DbRoomElec* bulid();
    QString tableName(){return "roomelecs";}
    bool insertItem(sRoomElecItem& item);

    void elec(QStringList &list);

protected:
    void createTable();
    bool modifyItem(const sRoomElecItem &item, const QString &cmd);
};


#endif // DBROOMELEC_H
