/*
 * dc_logindex.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_logindex.h"

DC_LogIndex::DC_LogIndex()
{

}



DC_LogIndex *DC_LogIndex::bulid()
{
    static DC_LogIndex* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_LogIndex();
    return sington;
}


bool DC_LogIndex::insertItem(const DC_LogIndexItem& item)
{
    QString cmd = "insert into %1 (item_type, name) "
                  "values(:item_type, :name)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_LogIndex::updateItem(const DC_LogIndexItem &item)
{
    QString cmd = "update %1 set "
                  "item_type            = :item_type,"
                  "name                 = :name,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_LogIndex::modifyItem(const DC_LogIndexItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":item_type",item.item_type);
        query.bindValue(":name",item.name);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_LogIndex::selectItem(QSqlQuery &query, DC_LogIndexItem &item)
{
    item.id = query.value("id").toInt();
    item.item_type = query.value("item_type").toInt();
    item.name = query.value("name").toString();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

bool DC_LogIndex::containType(int type)
{
    int ret = count("id", QString("where item_type = %1 ").arg(type));
    return (ret > 0) ? true:false;
}

bool DC_LogIndex::setTypeName(int type, const QString& name)
{
    return updateColumn("name", name, QString("where item_type=%1").arg(type));
}

QString DC_LogIndex::getTypeName(int type)
{
    QStringList list = listColumn("name",QString("where item_type = %1 ").arg(type));
    return list.first();
}

QVector<int> DC_LogIndex::listTypes()
{
    return listColumnToInt("item_type", "");
}


