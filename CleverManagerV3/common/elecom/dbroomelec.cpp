/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbroomelec.h"
#include "dbtemplateelec.h"

DbRoomElec::DbRoomElec()
{
    createTable();
    tableTile = tr("机房电量计算");
    hiddens << 7 ;
    headList.clear();
    headList << tr("编号") << tr("机房")
             << tr("起始时间") << tr("起始电能") << tr("结束时间") << tr("结束电能")
             << tr("耗电量") << tr("单价") << tr("电费");
}


void DbRoomElec::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "room           VCHAR,"
            "startdt        VCHAR,"
            "startele       DOUBLE,"
            "enddt          VCHAR,"
            "endele         DOUBLE,"
            "ele            DOUBLE,"
            "price          DOUBLE,"
            "fees           DOUBLE);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbRoomElec *DbRoomElec::bulid()
{
    static DbRoomElec* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRoomElec();
    return sington;
}


bool DbRoomElec::insertItem(sRoomElecItem &item)
{
    QString cmd = "insert into %1 (room,startdt,startele,enddt,endele,ele,price,fees) "
                  "values(:room,:startdt,:startele,:enddt,:endele,:ele,:price,:fees)";
    return modifyItem(item,cmd.arg(tableName()));
}


bool DbRoomElec::modifyItem(const sRoomElecItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":room",item.room);
    query.bindValue(":startdt",item.startdt);
    query.bindValue(":startele",item.startele);
    query.bindValue(":enddt",item.enddt);
    query.bindValue(":endele",item.endele);
    query.bindValue(":ele",item.ele);
    query.bindValue(":price",item.price);
    query.bindValue(":fees",item.fees);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}

void DbRoomElec::elec(QStringList &list)
{
    Db_Tran tran;
    sDbRoomHrsItem it;
    sRoomElecItem ele;

    DbRoomHrs* db = DbRoomHrs::bulid();
    for(int i=0; i<list.size(); ++i) {
        QString cmd = list.at(i);
        bool ret = elec_bulid_get(db, it, ele, cmd);
        if(ret) {
            ele.room = it.room;
            insertItem(ele);
        }
    }
}
