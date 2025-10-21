#ifndef DB_USR_H
#define DB_USR_H
#include "database/basicsql.h"

struct UsrItem{
    UsrItem():id(-1),jur(-1){}
    int id,jur;
    QString dateTime,name,pwd,email,telephone,remarks;
};
Q_DECLARE_METATYPE(UsrItem)



class DbUsrTable : public BasicSql1<UsrItem>
{
    Q_OBJECT
    DbUsrTable();
public:
    static DbUsrTable* get();
    QString tableName(){return "usrTable";}

    void insertItem(UsrItem& item);
    void updateItem(const UsrItem& item);

    QVector<UsrItem> selItemsByName(const QString& Name);
    void removeItemsByName(const QString& name);

protected:
    void modifyItem(const UsrItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,UsrItem &item);
};

#endif // DB_USR_H
