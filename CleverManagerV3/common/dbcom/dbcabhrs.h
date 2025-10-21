#ifndef DBCABHRS_H
#define DBCABHRS_H
#include "basicsql.h"

struct sDbCabHrsItem : public DbBasicItem
{
    QString room, modular, cab, road;
    double vol, cur, pow, pf, ele;
};

class DbCabHrs: public SqlBasic<sDbCabHrsItem>
{
    DbCabHrs();
public:
    static DbCabHrs* bulid();
    QString tableName(){return "cabhrs";}
    bool insertItem(sDbCabHrsItem& item);

protected:
    void createTable();
    void selectItem(QSqlQuery &query,sDbCabHrsItem &item);
    bool modifyItem(const sDbCabHrsItem &item, const QString &cmd);
};

#endif // DBCABHRS_H
