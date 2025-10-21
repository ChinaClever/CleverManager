#include "dbpowerports.h"
#include "dbpdudevices.h"

DbPowerPorts::DbPowerPorts()
{
    QString cmd = "create table if not exists %1("
                  "id                INTEGER primary key not null,"
                  "device_id         INTEGER references %2(%3),"
                  "port_index        INTEGER,"
                  "l1_ampere         NUMERIC,"
                  "l1_voltag         NUMERIC,"
                  "l2_ampere         NUMERIC,"
                  "l2_voltag         NUMERIC,"
                  "l3_ampere         NUMERIC,"
                  "l3_voltag         NUMERIC,"
                  "update_time       DATETIME)";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName()).arg(DbPduDevices::get()->tableName()).arg("id")))
        throwError(query.lastError());
    connect(DbPduDevices::get(),SIGNAL(itemChanged(int,int)),SLOT(pduDevicesItemChange(int,int)));
}

DbPowerPorts *DbPowerPorts::get()
{
    static DbPowerPorts* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPowerPorts();
    return sington;
}

void DbPowerPorts::insertItem(const PowerPortItem &item)
{
    QString cmd = "insert into %1(id,device_id,port_index,l1_ampere,l1_voltag,l2_ampere,l2_voltag,l3_ampere,l3_voltag,update_time) "
                  "values(:id,:device_id,:port_index,:l1_ampere,:l1_voltag,:l2_ampere,:l2_voltag,:l3_ampere,:l3_voltag,:update_time)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbPowerPorts::updateItem(const PowerPortItem &item)
{
    QString cmd = "update %1 set "
                  "device_id         = :device_id,"
                  "port_index        = :port_index,"
                  "l1_ampere         = :l1_ampere,"
                  "l1_voltag         = :l1_voltag,"
                  "l2_ampere         = :l2_ampere,"
                  "l2_voltag         = :l2_voltag,"
                  "l3_ampere         = :l3_ampere,"
                  "l3_voltag         = :l3_voltag,"
                  "update_time       = :update_time"
                  " where id         = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}

void DbPowerPorts::pduDevicesItemChange(int id, int type)
{
    if(type == Remove){

    }
}

void DbPowerPorts::modifyItem(const PowerPortItem &item, const QString &cmd)
{
    QSqlQuery query;
    query.prepare( cmd );
    query.bindValue(":id",item.id);
    query.bindValue(":device_id",item.device_id);
    query.bindValue(":port_index",item.port_index);
    query.bindValue(":l1_ampere",item.l1_ampere);
    query.bindValue(":l1_voltag",item.l1_voltag);
    query.bindValue(":l2_ampere",item.l2_ampere);
    query.bindValue(":l2_voltag",item.l2_voltag);
    query.bindValue(":l3_ampere",item.l3_ampere);
    query.bindValue(":l3_ampere",item.l3_ampere);
    query.bindValue(":update_time",item.update_time);

    if(!query.exec())
        throwError(query.lastError());
}

void DbPowerPorts::selectItem(QSqlQuery &query, PowerPortItem &item)
{
    item.id = query.value("id").toInt();
    item.device_id = query.value("device_id").toInt();
    item.port_index = query.value("port_index").toInt();
    item.l1_ampere = query.value("l1_ampere").toFloat();
    item.l1_voltag = query.value("l1_voltag").toFloat();
    item.l2_ampere = query.value("l2_ampere").toFloat();
    item.l2_voltag = query.value("l2_voltag").toFloat();
    item.l3_ampere = query.value("l3_ampere").toFloat();
    item.l3_ampere = query.value("l3_ampere").toFloat();
    item.update_time=query.value("update_time").toDateTime();
}
