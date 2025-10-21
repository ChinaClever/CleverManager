#ifndef DBROOMHRS_H
#define DBROOMHRS_H
#include "basicsql.h"

struct sDbRoomHrsItem : public DbBasicItem
{
    QString room;
    double vol, cur, pow, pf, ele;
};

class DbRoomHrs: public SqlBasic<sDbRoomHrsItem>
{
    DbRoomHrs();
public:
    static DbRoomHrs* bulid();
    QString tableName(){return "roomhrs";}
    bool insertItem(sDbRoomHrsItem& item);

protected:
    void createTable();
    void selectItem(QSqlQuery &query,sDbRoomHrsItem &item);
    bool modifyItem(const sDbRoomHrsItem &item, const QString &cmd);
};

#endif // DBROOMHRS_H
