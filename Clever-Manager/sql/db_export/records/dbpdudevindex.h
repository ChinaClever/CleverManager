#ifndef DBPDUDEVINDEX_H
#define DBPDUDEVINDEX_H
#include "database/basicsql.h"


struct PduDevIndexItem{
    PduDevIndexItem():id(-1),devNum(-1),devType(-1),status(-1){}

    int id;
    QString date,time;
    QString ip;
    int devNum,devType;
    QString typeStr;
    QString roomName,cabinetName, devName;
    int status;
    QString statusStr;
};
Q_DECLARE_METATYPE(PduDevIndexItem)

class DbPduDevIndex: public BasicSql1<PduDevIndexItem>
{
    Q_OBJECT
public:
    DbPduDevIndex();
    static DbPduDevIndex* get();
    virtual QString tableName(){return "pduDevIndex";}
    bool insertItem(PduDevIndexItem &item);

    QVector<PduDevIndexItem> query(const QString& condition);
    QVector<PduDevIndexItem> selItemsByIp(const QString& ip);
    void removeItemsByIp(const QString &ip);

protected:
    void createTable();
    bool modifyItem(PduDevIndexItem &item, const QString& cmd);
    void selectItem(QSqlQuery &query,PduDevIndexItem &item);
    void initItem(PduDevIndexItem &item);

private:
    QString getTypeStr(int type);
    QString getstatusStr(int status);
    int mKeyId;
};

#endif // DBPDUDEVINDEX_H
