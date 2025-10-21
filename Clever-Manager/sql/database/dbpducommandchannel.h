#ifndef DBPDUCOMMANDCHANNEL_H
#define DBPDUCOMMANDCHANNEL_H
#include "basicsql.h"

#define CHANNEL_SNMP 1
#define CHANNEL_ZEBRA 2
#define CHANNEL_CLEVER 4
struct DbPduCommandChannelItem{
    DbPduCommandChannelItem():id(-1),get_channel(0),set_channel(0){}
    int id,get_channel,set_channel;
    QString  ip;
};
Q_DECLARE_METATYPE(DbPduCommandChannelItem)



class DbPduCommandChannel : public BasicSql1<DbPduCommandChannelItem>
{
    Q_OBJECT

    explicit DbPduCommandChannel();

public:
    static DbPduCommandChannel *get();
    QString tableName(){return "pdu_command_channel";}
    void insertItem(const DbPduCommandChannelItem& item);
    void updateItem(const DbPduCommandChannelItem& item);
    void removeItem(const DbPduCommandChannelItem& item);
    DbPduCommandChannelItem selectItemByIp(const QString &Ip);
protected:
    void modifyItem(const DbPduCommandChannelItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DbPduCommandChannelItem &item);
    QMap<QString,DbPduCommandChannelItem> m_channels;
};

#endif // DBPDUCOMMANDCHANNEL_H
