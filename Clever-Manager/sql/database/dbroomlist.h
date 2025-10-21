#ifndef DBROMELIST_H
#define DBROMELIST_H

#include "basicsql.h"

struct RoomItem{
    RoomItem():id(-1){}
    bool unequal(const RoomItem& item) const
    {
        return id!=item.id || name!=item.name || address!=item.address;
    }
    int id;
    QString name,address;
};
Q_DECLARE_METATYPE(RoomItem)

class DbRoomList : public BasicSql1<RoomItem>
{
    Q_OBJECT
    DbRoomList();
public:
    static DbRoomList* get();
    QString tableName(){return "rooms";}
    void insertItem(const RoomItem& item);
    void updateItem(const RoomItem& item);
signals:
    void updateSig(QString, QString);

protected:
    void modifyItem(const RoomItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,RoomItem &item);
};

#endif // DBROMELIST_H
