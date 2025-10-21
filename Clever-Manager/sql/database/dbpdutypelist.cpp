#include "dbpdutypelist.h"
#include "dboidtable.h"

DbPduTypeList::DbPduTypeList(QObject *parent)
{
    this->setParent(parent);
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "des               TEXT,"
            "oid               TEXT not null);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

DbPduTypeList *DbPduTypeList::get()
{
    static DbPduTypeList* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduTypeList();
    return sington;
}

void DbPduTypeList::insertItem(const PduTypeItem &item)
{
    QString cmd = "insert into %1 (id,des,oid) values(:id,:des,:oid)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbPduTypeList::updateItem(const PduTypeItem &item)
{
    QString cmd = "update %1 set des = :des,oid = :oid where id = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}

void DbPduTypeList::removeItem(const PduTypeItem &item)
{
    remove(QString("id = %1").arg(item.id));
    DbOidTable::get()->remove(QString("type_id = %1").arg(item.id));
}

QVector<PduTypeItem> DbPduTypeList::selectItemsByOid(const QString &oid)
{
    return selectItems(QString("where oid = \"%1\"").arg(oid));
}

void DbPduTypeList::modifyItem(const PduTypeItem &item, const QString &cmd)
{
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":des",item.des);
    query.bindValue(":oid",item.oid);
    if(!query.exec())
        throwError(query.lastError());
}

void DbPduTypeList::selectItem(QSqlQuery &query, PduTypeItem &item)
{
    item.id = query.value("id").toInt();
    item.des = query.value("des").toString();
    item.oid = query.value("oid").toString();
}
