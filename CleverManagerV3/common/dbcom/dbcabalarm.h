#ifndef DBCABALARM_H
#define DBCABALARM_H

#include "dbcabhrs.h"

struct sDbCabAlarmItem : public DbBasicItem
{
    QString room, modular, cab;
    QString item, msg;
};

class DbCabAlarm : public SqlBasic<sDbCabAlarmItem>
{
    DbCabAlarm();
public:
    static DbCabAlarm* bulid();
    QString tableName(){return "cabalarms";}
    bool insertItem(sDbCabAlarmItem& item);

protected:
    void createTable();
    bool modifyItem(const sDbCabAlarmItem &item, const QString &cmd);
};

#endif // DBCABALARM_H
