/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbcabelec.h"
#include "dbtemplateelec.h"

DbCabElec::DbCabElec()
{
    createTable();
    tableTile = tr("机柜电量计算");
    hiddens << 2 << 10;
    headList.clear();
    headList << tr("编号") << tr("机房") << tr("模块") << tr("机柜")  << tr("主备路")
             << tr("起始时间") << tr("起始电能") << tr("结束时间") << tr("结束电能")
             << tr("耗电量") << tr("单价") << tr("电费");
}


void DbCabElec::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "room           VCHAR,"
            "modular        VCHAR,"
            "cab            VCHAR,"
            "road           VCHAR,"
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


DbCabElec *DbCabElec::bulid()
{
    static DbCabElec* sington = nullptr;
    if(sington == nullptr)
        sington = new DbCabElec();
    return sington;
}


bool DbCabElec::insertItem(sCabElecItem &item)
{
    QString cmd = "insert into %1 (room,modular,cab,road,startdt,startele,enddt,endele,ele,price,fees) "
                  "values(:room,:modular,:cab,:road,:startdt,:startele,:enddt,:endele,:ele,:price,:fees)";
    return modifyItem(item,cmd.arg(tableName()));
}


bool DbCabElec::modifyItem(const sCabElecItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":room",item.room);
    query.bindValue(":modular",item.modular);
    query.bindValue(":cab",item.cab);
    query.bindValue(":road",item.road);
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

void DbCabElec::elec(QStringList &list)
{
    Db_Tran tran;
    sDbCabHrsItem it;
    sCabElecItem ele;

    DbCabHrs* db = DbCabHrs::bulid();
    for(int i=0; i<list.size(); ++i) {
        QString cmd = list.at(i);
        bool ret = elec_bulid_get(db, it, ele, cmd);
        if(ret) {
            ele.room = it.room;
            ele.modular = it.modular;
            ele.cab = it.cab;
            ele.road = it.road;
            insertItem(ele);
        }
    }
}
