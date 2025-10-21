#include "dbpdudevices.h"
#include "dbcabinetlist.h"

DbPduDevices::DbPduDevices()
{
    QString cmd = "create table if not exists %1("
                  "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "cabinet_id        INTEGER references %2(%3),"
                  "name              TEXT,"
                  "pdu_address       TEXT,"
                  "pdu_index         INTEGER,"
                  "channel_id        TEXT,"
                  "socket_count      INTEGER,"
                  "zebra_channel_state BOOLEAN,"
                  "clever_channel_state BOOLEAN,"
                  "l1_ampere         NUMERIC,"
                  "l1_voltag         NUMERIC,"
                  "l2_ampere         NUMERIC,"
                  "l2_voltag         NUMERIC,"
                  "l3_ampere         NUMERIC,"
                  "l3_voltag         NUMERIC,"
                  "update_time       DATETIME,"
                  "device_type        INTEGER)";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName()).arg(DbCabinetList::get()->tableName()).arg("id")))
    {
        throwError(query.lastError());
    }
    connect(DbCabinetList::get(),SIGNAL(itemChanged(int,int)),SLOT(cabinetItemChange(int,int)));
}

DbPduDevices *DbPduDevices::get()
{
    static DbPduDevices* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduDevices();
    return sington;
}

void DbPduDevices::insertItem(const PduDeviceItem &item)
{
    QString cmd = "insert into %1 ("
                  "id,cabinet_id,name,pdu_address,pdu_index,channel_id,socket_count,zebra_channel_state,clever_channel_state,"
                  "l1_ampere,l1_voltag,l2_ampere,l2_voltag,l3_ampere,l3_voltag,update_time,device_type"
                  ")values("
                  ":id,:cabinet_id,:name,:pdu_address,:pdu_index,:channel_id,:socket_count,:zebra_channel_state,:clever_channel_state,"
                  ":l1_ampere,:l1_voltag,:l2_ampere,:l2_voltag,:l3_ampere,:l3_voltag,:update_time,:device_type)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbPduDevices::updateItem(const PduDeviceItem &item)
{
    QString cmd = "update %1 set "
                  "cabinet_id        = :cabinet_id,"
                  "name              = :name,"
                  "pdu_address       = :pdu_address,"
                  "pdu_index         = :pdu_index,"
                  "channel_id        = :channel_id,"
                  "socket_count      = :socket_count,"
                  "zebra_channel_state = :zebra_channel_state,"
                  "clever_channel_state = :clever_channel_state,"
                  "l1_ampere         = :l1_ampere,"
                  "l1_voltag         = :l1_voltag,"
                  "l2_ampere         = :l2_ampere,"
                  "l2_voltag         = :l2_voltag,"
                  "l3_ampere         = :l3_ampere,"
                  "l3_voltag         = :l3_voltag,"
                  "update_time       = :update_time,"
                  "device_type        = :device_type"
                  " where id = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}

void DbPduDevices::removeItemsByCabinetId(int id)
{
    QVector<PduDeviceItem> items = selectItemsByCabinetId(id);
    foreach(const PduDeviceItem& item,items)
        removeItem(item);

}

QVector<PduDeviceItem> DbPduDevices::selectItemsByCabinetId(int id)
{
    return selectItems(QString("where cabinet_id = %1").arg(id));
}

void DbPduDevices::cabinetItemChange(int cabinet_id, int type)
{
    if(type == Remove)
        removeItemsByCabinetId(cabinet_id);
}

void DbPduDevices::modifyItem(const PduDeviceItem &item, const QString &cmd)
{
    QSqlQuery query;
    query.prepare( cmd );
    query.bindValue(":id",item.id);
    query.bindValue(":cabinet_id",item.cabinet_id);
    query.bindValue(":pdu_index",item.pdu_index);
    query.bindValue(":socket_count",item.socket_count);
    query.bindValue(":l1_ampere",item.l1_ampere);
    query.bindValue(":l2_ampere",item.l2_ampere);
    query.bindValue(":l3_ampere",item.l3_ampere);
    query.bindValue(":l1_voltag",item.l1_voltag);
    query.bindValue(":l2_voltag",item.l2_voltag);
    query.bindValue(":l3_voltag",item.l3_voltag);
    query.bindValue(":name",item.name);
    query.bindValue(":pdu_address",item.pdu_address);
    query.bindValue(":channel_id",item.channel_id);
    query.bindValue(":zebra_channel_state",item.zebra_channel_state);
    query.bindValue(":clever_channel_state",item.clever_channel_state);
    query.bindValue(":update_time",item.update_time);
    query.bindValue(":device_type",item.device_type);
    if(!query.exec())
        throwError(query.lastError());
}

void DbPduDevices::selectItem(QSqlQuery &query, PduDeviceItem &item)
{
    item.id = query.value("id").toInt();
    item.cabinet_id = query.value("cabinet_id").toInt();
    item.pdu_index  = query.value("pdu_index").toInt();
    item.socket_count=query.value("socket_count").toInt();
    item.l1_ampere  = query.value("l1_ampere").toFloat();
    item.l2_ampere  = query.value("l2_ampere").toFloat();
    item.l3_ampere  = query.value("l3_ampere").toFloat();
    item.l1_voltag  = query.value("l1_voltag").toFloat();
    item.l2_voltag  = query.value("l2_voltag").toFloat();
    item.l3_voltag  = query.value("l3_voltag").toFloat();
    item.name       = query.value("name").toString();
    item.pdu_address= query.value("pdu_address").toString();
    item.channel_id = query.value("channel_id").toString();
    item.zebra_channel_state = query.value("zebra_channel_state").toBool();
    item.clever_channel_state = query.value("clever_channel_state").toBool();
    item.update_time = query.value("update_time").toDateTime();
    item.device_type = query.value("device_type").toInt();
}
