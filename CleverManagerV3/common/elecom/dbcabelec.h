#ifndef DBCABELE_H
#define DBCABELE_H
#include "sqltablewid.h"
#include "dbcabhrs.h"

struct sCabElecItem : public DbBasicElecItem{
    QString room, modular, cab, road;
};


class DbCabElec: public SqlBasic<sCabElecItem>
{
    DbCabElec();
public:
    static DbCabElec* bulid();
    QString tableName(){return "cabelecs";}
    bool insertItem(sCabElecItem& item);

    void elec(QStringList &list);

protected:
    void createTable();
    bool modifyItem(const sCabElecItem &item, const QString &cmd);
};

#endif // DBCABELE_H
