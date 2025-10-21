#include "dboidtable.h"

DbOidTable::DbOidTable(QObject *parent)
{
    this->setParent(parent);
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "type_id            INTEGER references %2(%3),"
            "name              TEXT not null,"
            "oid               TEXT);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName()).arg(DbPduTypeList::get()->tableName()).arg("id")))
    {
        throwError(query.lastError());
    }
}

DbOidTable *DbOidTable::get()
{
    static DbOidTable* sington = nullptr;
    if(sington == nullptr)
        sington = new DbOidTable();
    return sington;
}

void DbOidTable::insertItem(const OidItem &item)
{
    QString cmd = "insert into %1 (id,type_id,name,oid) values(:id,:type_id,:name,:oid)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbOidTable::updateItem(const OidItem &item)
{
    QString cmd = "update %1 set name = :name,type_id = :type_id,oid = :oid where id = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}

QVector<OidItem> DbOidTable::selectItemsByPduTypeId(int typeId)
{
    return selectItems(QString("where type_id = %1").arg(typeId));
}

QVector<OidItem> DbOidTable::selectItemByOid(const QString &oid)
{
    QStringList list = oid.split('.',QString::SkipEmptyParts);
    QString masterOid = '.'+list.mid(0,8).join('.');
    QString subOid = '.'+list.mid(8).join('.');
    if(subOid.endsWith(".0"))//取出结尾".0"
        subOid.truncate(subOid.size()-2);
    QVector<OidItem> items;
    PduTypeItem pItem = DbPduTypeList::get()->selectItemsByOid(masterOid).value(0);
    if(pItem.id >= 0)
        items = selectItems(QString("where type_id = %1 and oid = \"%2\"").arg(pItem.id).arg(subOid));
    return items;
}

QVector<OidItem> DbOidTable::selectItemByName(int typeId, const QString &name)
{
    return selectItems(QString("where type_id = %1 and name = \"%2\"").arg(typeId).arg(name));
}

QString DbOidTable::map2Name(const QString &oid)
{
    return oid;
}

void DbOidTable::modifyItem(const OidItem &item, const QString &cmd)
{
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":type_id",item.type_id);
    query.bindValue(":name",item.name);
    query.bindValue(":oid",item.oid);
    if(!query.exec())
        throwError(query.lastError());
}

void DbOidTable::selectItem(QSqlQuery &query, OidItem &item)
{
    item.id = query.value("id").toInt();
    item.type_id = query.value("type_id").toInt();
    item.name = query.value("name").toString();
    item.oid = query.value("oid").toString();
}
