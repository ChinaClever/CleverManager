#ifndef DBTGROOM_H
#define DBTGROOM_H
#include "database/basicsql.h"
#include "dbtgcabinet.h"

#define DB_TG_ROOM_TABLE    "tgRoom"

struct TgRoomItem{
    TgRoomItem():id(-1),
        cur(0),vol(0),pow(0),ele(0),pf(0),tem(0),hum(0),
        allNum(0),lineNum(0),alarmNum(0),offLineNum(0)
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        date = dateTime.toString("yyyy-MM-dd");
        time = dateTime.toString("hh:mm:ss");
    }

    int id;
    QString date,time;
    QString roomName; // 机房名
    double cur,vol,pow,ele,pf,tem,hum;
    int allNum,lineNum,alarmNum,offLineNum;
};
Q_DECLARE_METATYPE(TgRoomItem)


class DbTgRoom : public BasicSql1<TgRoomItem>
{
    Q_OBJECT

public:
    DbTgRoom();
    static DbTgRoom* get();
    virtual QString tableName(){return DB_TG_ROOM_TABLE;}
    void insertItem(TgRoomItem &item);

    QVector<TgRoomItem> query(const QString&);
    QVector<TgRoomItem> selItemsByRoomName(const QString& roomName);
    void removeItemsByRoomName(const QString &roomName);

protected:
    void createTable();
    bool modifyItem(const TgRoomItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,TgRoomItem &item);
    void initItem(TgRoomItem &item);
};

#endif // DBTGROOM_H
