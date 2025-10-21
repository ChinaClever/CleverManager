#ifndef DBPDUHRS_H
#define DBPDUHRS_H
#include "basicsql.h"

struct sDbPduHrsItem : public DbBasicItem
{
    QString room, modular, cab, road;
    QString dev_type, ip, dev_num;
    QString item;
    QString sw;
    double vol, cur, pow, pf, ele;
};


class DbPduHrs : public SqlBasic<sDbPduHrsItem>
{
    DbPduHrs();
public:
    static DbPduHrs* bulid();
    QString tableName(){return "pduhrs";}
    bool insertItem(sDbPduHrsItem& item);

protected:
    void createTable();
    void selectItem(QSqlQuery &query,sDbPduHrsItem &item);
    bool modifyItem(const sDbPduHrsItem &item, const QString &cmd);
};

#endif // DBPDUHRS_H
