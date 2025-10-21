/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbconsumerdevices.h"
#include "dbcabinetlist.h"

DbConsumerDevices::DbConsumerDevices()
{
    createTable();
    connect(DbCabinetList::get(),SIGNAL(itemChanged(int,int)),SLOT(cabinetItemChange(int,int)));
}

void DbConsumerDevices::createTable()
{
    QString cmd = "create table if not exists %1("
                  "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "cabinet_id        INTEGER references %2(%3),"
                  "name              TEXT,"
                  "main_pdu_port     INTEGER,"
                  "spare_pdu_port    INTEGER,"
                  "address           INTEGER,"
                  "occupy_height     INTEGER);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()).arg(DbCabinetList::get()->tableName()).arg("id")))
        throwError(query.lastError());
}

DbConsumerDevices *DbConsumerDevices::get()
{
    static DbConsumerDevices* sington = nullptr;
    if(sington == nullptr)
        sington = new DbConsumerDevices();
    return sington;
}

void DbConsumerDevices::insertItem(ConsumerDeviceItem &item)
{
    item.id = maxId()+1;
    QString cmd = "insert into %1(id,cabinet_id,name,main_pdu_port,spare_pdu_port,address,occupy_height) "
                  "values(:id,:cabinet_id,:name,:main_pdu_port,:spare_pdu_port,:address,:occupy_height)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbConsumerDevices::updateItem(const ConsumerDeviceItem &item)
{
    QString name = findById(item.id).name;
    QString cmd = "update %1 set "
                  "cabinet_id        = :cabinet_id,"
                  "name              = :name,"
                  "main_pdu_port     = :main_pdu_port,"
                  "spare_pdu_port    = :spare_pdu_port,"
                  "address           = :address,"
                  "occupy_height     = :occupy_height"
                  " where id         = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
    emit updateSig(item.id,name);
}

void DbConsumerDevices::removeItemsByCabinetId(int cabinetId)
{
    QString cmd = QString("cabinet_id=%1").arg(cabinetId);
    remove(cmd);
}

QVector<ConsumerDeviceItem> DbConsumerDevices::selectItemsByCabinetId(int id)
{
    return selectItems(QString("where cabinet_id = %1").arg(id));
}

QVector<ConsumerDeviceItem> DbConsumerDevices::selectItemsByRackAddress(int address,int cabinetId)
{
    return selectItems(QString("where address = %1 and cabinet_id = %2").arg(address).arg(cabinetId));
}

void DbConsumerDevices::cabinetItemChange(int cabinet_id, int type)
{
    if(type == Remove)
        removeItemsByCabinetId(cabinet_id);
}

void DbConsumerDevices::modifyItem(const ConsumerDeviceItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare( cmd );
    query.bindValue(":id",item.id);
    query.bindValue(":name",item.name);
    query.bindValue(":main_pdu_port",item.main_pdu_port);
    query.bindValue(":spare_pdu_port",item.spare_pdu_port);
    query.bindValue(":cabinet_id",item.cabinet_id);
    query.bindValue(":address",item.address);
    query.bindValue(":occupy_height",item.occupy_height);
    if(!query.exec())
        throwError(query.lastError());
}

void DbConsumerDevices::selectItem(QSqlQuery &query, ConsumerDeviceItem &item)
{
    item.id = query.value("id").toInt();
    item.name = query.value("name").toString();
    item.main_pdu_port = query.value("main_pdu_port").toInt();
    item.spare_pdu_port = query.value("spare_pdu_port").toInt();
    item.cabinet_id = query.value("cabinet_id").toInt();
    item.address = query.value("address").toInt();
    item.occupy_height = query.value("occupy_height").toInt();
}

bool DbConsumerDevices::containName(int cabinetId, const QString &name)
{
    int ret = selectItems(QString("where name = \"%1\" and cabinet_id = %2").arg(name).arg(cabinetId)).size();
    return (ret > 0) ? true:false;
}
