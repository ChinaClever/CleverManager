/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbpdudevices.h"
#include "dbcabinetlist.h"

DbPduDevices::DbPduDevices()
{
    createTable();
    tableTile = tr("PDU列表");
    hiddens << 1 << 2;
    headList.clear();
    headList << tr("编号") << tr("机房ID") << tr("机柜ID")
             << tr("机房") << tr("机柜") << tr("主备路")
             << tr("设备类型")  << tr("级联编号") << tr("设备IP") ;

    connect(DbRoomList::get(),SIGNAL(itemChanged(int,int)),SLOT(roomItemChange(int,int)));
    connect(DbCabinetList::get(),SIGNAL(itemChanged(int,int)),SLOT(cabinetItemChange(int,int)));
}

void DbPduDevices::createTable()
{
    QString cmd = "create table if not exists %1("
                  "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "room_id           INTEGER references %2(%3),"
                  "cab_id        INTEGER references %4(%5),"
                  "room              VCHAR,"
                  "cab               VCHAR,"
                  "road              VCHAR,"
                  "dev_type          VCHAR,"
                  "dev_num           VCHAR,"
                  "ip                VCHAR)";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()).arg(DbRoomList::get()->tableName()).arg("id")
                   .arg(DbCabinetList::get()->tableName()).arg("id")))
    {
        throwError(query.lastError());
    }
}

DbPduDevices *DbPduDevices::get()
{
    static DbPduDevices* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduDevices();
    return sington;
}

void DbPduDevices::insertItem(PduDeviceItem &item)
{
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id,cab_id,room_id,room,cab,ip,dev_num,dev_type,road) "
                  "values(:id,:cab_id,:room_id,:room,:cab,:ip,:dev_num,:dev_type,:road)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}


void DbPduDevices::insertItem(CabinetItem &item)
{
    PduDeviceItem it;
    it.cab_id = item.id;
    it.room_id = item.room_id;
    it.room = item.room;
    it.cab = item.cab;

    it.ip = item.main_ip;
    if(!it.ip.isEmpty()) {
        it.road = tr("主路");
        it.dev_num = item.main_num;
        it.dev_type = item.main_type;
        insertItem(it);
    }

    it.ip = item.spare_ip;
    if(!it.ip.isEmpty()) {
        it.road = tr("备路");
        it.dev_num = item.spare_num;
        it.dev_type = item.spare_type;
        insertItem(it);
    }
}

void DbPduDevices::updateItem(const PduDeviceItem &item)
{
    QString cmd = "update %1 set "
                  "cab_id        = :cab_id,"
                  "room_id           = :room_id,"
                  "room              = :room,"
                  "cab               = :cab,"
                  "road              = :road,"
                  "ip                = :ip,"
                  "dev_num           = :dev_num,"
                  "dev_type          = :dev_type,"
                  " where id = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}


void DbPduDevices::roomItemChange(int roomId, int type)
{
    QString cmd = QString("room_id=%1").arg(roomId);
    switch (type) {
    case Remove: remove(cmd); break;
    case Update: updateColumn("room", DbRoomList::get()->findById(roomId).room, cmd);  break;
    }
}

void DbPduDevices::cabinetItemChange(int cab_id, int type)
{
    remove(QString("cab_id=%1").arg(cab_id));
    if(type > Remove) {
        CabinetItem item = DbCabinetList::get()->findById(cab_id);
        insertItem(item);
    }
}

void DbPduDevices::removeItemsByCabinetId(int id)
{
    cabinetItemChange(id, Remove);
}

QVector<PduDeviceItem> DbPduDevices::selectItemsByCabinetId(int id)
{
    return selectItems(QString("where cab_id = %1").arg(id));
}

QStringList DbPduDevices::listRoom()
{
    return listColumn("room", "");;
}

QStringList DbPduDevices::listCabinet(const QString &room)
{
    QString condition = QString("where room=\'%1\'").arg(room);
    return listColumn("cab", condition);;
}

QStringList DbPduDevices::listIps()
{
    return listColumn("ip", "");
}

QStringList DbPduDevices::listIps(const QString &dev_type)
{
    QString condition = QString("where dev_type=\'%1\'").arg(dev_type);
    return listColumn("ip", condition);
}

QStringList DbPduDevices::listIps(const QString &room,const QString &cab)
{
    QString condition = QString("where room=\'%1\' and cab=\'%2\'").arg(room).arg(cab);
    return listColumn("ip", condition);
}

QStringList DbPduDevices::listDevTypes()
{
    return listColumn("dev_type", "");
}

QStringList DbPduDevices::listDevNums(const QString &ip)
{
    QString condition = QString("where ip=\'%1\'").arg(ip);
    return listColumn("dev_num", condition);
}

int DbPduDevices::getDevNums(const QString &ip)
{
    QStringList ips = listDevNums(ip);
    return ips.size();
}

void DbPduDevices::modifyItem(const PduDeviceItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare( cmd );
    query.bindValue(":id",item.id);
    query.bindValue(":room_id",item.room_id);
    query.bindValue(":cab_id",item.cab_id);
    query.bindValue(":cab",item.cab);
    query.bindValue(":room",item.room);
    query.bindValue(":road",item.road);
    query.bindValue(":ip",item.ip);
    query.bindValue(":dev_num",item.dev_num);
    query.bindValue(":dev_type",item.dev_type);
    if(!query.exec())
        throwError(query.lastError());
}

void DbPduDevices::selectItem(QSqlQuery &query, PduDeviceItem &item)
{
    item.id = query.value("id").toInt();
    item.room_id  = query.value("room_id").toInt();
    item.cab_id = query.value("cab_id").toInt();
    item.room= query.value("room").toString();
    item.cab  = query.value("cab").toString();
    item.road = query.value("road").toString();
    item.ip = query.value("ip").toString();
    item.dev_num=query.value("dev_num").toString();
    item.dev_type = query.value("dev_type").toString();
}
