#include "dbpducommandchannel.h"

DbPduCommandChannel::DbPduCommandChannel()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER primary key autoincrement not null,"
            "ip                TEXT not null,"
            "get_channel       INTEGER,"
            "set_channel       INTEGER);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

DbPduCommandChannel *DbPduCommandChannel::get()
{
    static DbPduCommandChannel* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduCommandChannel();
    return sington;
}

void DbPduCommandChannel::insertItem(const DbPduCommandChannelItem &item)
{
    m_channels[item.ip] = item;
    QString cmd = "insert into %1 (ip,get_channel,set_channel) values(:ip,:get_channel,:set_channel)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbPduCommandChannel::updateItem(const DbPduCommandChannelItem &item)
{
    m_channels[item.ip] = item;
    QString cmd = "update %1 set ip = :ip,get_channel = :get_channel, set_channel = :set_channel where id = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}

void DbPduCommandChannel::removeItem(const DbPduCommandChannelItem& item)
{
    m_channels.remove(item.ip);
    remove(QString("id = %1").arg(item.id));
}

DbPduCommandChannelItem DbPduCommandChannel::selectItemByIp(const QString &ip)
{
    DbPduCommandChannelItem & item = m_channels[ip];
    if(item.id < 0)
        item = selectItems(QString("where ip = '%1' ").arg(ip)).value(0);
    return item;
}

void DbPduCommandChannel::modifyItem(const DbPduCommandChannelItem &item, const QString &cmd)
{
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":ip",item.ip);
    query.bindValue(":get_channel",item.get_channel);
    query.bindValue(":set_channel",item.set_channel);
    if(!query.exec())
        throwError(query.lastError());
}

void DbPduCommandChannel::selectItem(QSqlQuery &query, DbPduCommandChannelItem &item)
{
    item.id = query.value("id").toInt();
    item.ip = query.value("ip").toString();
    item.get_channel = query.value("get_channel").toInt();
    item.set_channel = query.value("set_channel").toInt();
}
