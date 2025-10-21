#ifndef DBROMELIST_H
#define DBROMELIST_H

#include "basicsql.h"

struct RoomItem: public DbBasicItem
{
    bool unequal(const RoomItem& item) const
    {
        return id!=item.id || room!=item.room || address!=item.address;
    }
    QString room,address;
};
Q_DECLARE_METATYPE(RoomItem)

class DbRoomList : public SqlBasic<RoomItem>
{
    Q_OBJECT
    DbRoomList();
public:
    static DbRoomList* get();
    QString tableName(){return "rooms";}
    void insertItem(RoomItem &item);
    void updateItem(const RoomItem& item);

    QStringList list();
    int contains(const QString &room);
    int findId(const QString& room);
    RoomItem find(const QString& room);
    void removeRoom(const QString& room);

signals:
    void delSig(QString);
    void updateSig(QString, QString);

protected:
    void createTable();
    void modifyItem(const RoomItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,RoomItem &item);
};

#endif // DBROMELIST_H
