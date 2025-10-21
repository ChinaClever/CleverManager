#ifndef DBTGCABINET_H
#define DBTGCABINET_H
#include "database/basicsql.h"

#define DB_TG_CAB_TABLE    "tgcab"

struct TgCabItem{
    TgCabItem():id(-1),
        cur(0),cura(0),curb(0),vol(0),vola(0),volb(0),pow(0),elea(0),eleb(0)
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        date = dateTime.toString("yyyy-MM-dd");
        time = dateTime.toString("hh:mm:ss");
    }

    int id;
    QString date,time;
    QString roomName; // 机房名
    QString cabName; // 机柜名
    double cur,cura,curb, vol, vola,volb, pow,elea,eleb;
};
struct TgCabNameCountItem{
    TgCabNameCountItem():count(-1)
    {

    }

    int count;
    QString cabName; // 机柜名
};
Q_DECLARE_METATYPE(TgCabItem)



class DbTgCabinet : public BasicSql1<TgCabItem>
{
    Q_OBJECT
public:
    explicit DbTgCabinet();
    static DbTgCabinet* get();
    virtual QString tableName(){return DB_TG_CAB_TABLE;}
    void insertItem(TgCabItem &item);
    QVector<TgCabItem> selItemsByRoomName(const QString& condition);
    QVector<TgCabNameCountItem>  selectItemsGroupByCabName(const QString &condition);


protected:
    void createTable();
    void initItem(TgCabItem &item);
    bool modifyItem(const TgCabItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,TgCabItem &item);
    void selectTgCabNameCountItem(QSqlQuery &query,TgCabNameCountItem &item);
};

#endif // DBTGCABINET_H
