#ifndef DBOIDTABLE_H
#define DBOIDTABLE_H

#include "basicsql.h"
#include "dbpdutypelist.h"

struct OidItem{
    OidItem():id(-1),type_id(-1){}
    int id,type_id;
    QString name,oid;
};
Q_DECLARE_METATYPE(OidItem)

class DbOidTable :public BasicSql1<OidItem>
{
    Q_OBJECT
    explicit DbOidTable(QObject *parent = 0);
public:
    static DbOidTable *get();
    QString tableName(){return "oid_types";}
    void insertItem(const OidItem& item);
    void updateItem(const OidItem& item);
    QVector<OidItem> selectItemsByPduTypeId(int typeId);
    QVector<OidItem> selectItemByOid(const QString &oid);
    QVector<OidItem> selectItemByName(int typeId,const QString &name);
    QString map2Name(const QString &oid);
protected:
    void modifyItem(const OidItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,OidItem &item);
protected:
    QMap<QString,OidItem> m_map;
};

#endif // DBOIDTABLE_H
