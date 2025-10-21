#ifndef DBDEVALARM_H
#define DBDEVALARM_H
#include "basicsql.h"

struct sDbAlarmItem : public DbBasicItem
{
    QString room, modular, cab, road;
    QString dev_type, ip;
    QString dev_num;
    QString item, msg;
};

class DbPduAlarm : public SqlBasic<sDbAlarmItem>
{
    DbPduAlarm();
public:
    static DbPduAlarm* bulid();
    QString tableName(){return "pdualarmlogs";}
    bool insertItem(sDbAlarmItem& item);

protected:
    void createTable();
    bool modifyItem(const sDbAlarmItem &item, const QString &cmd);
};

#endif // DBDEVALARM_H
