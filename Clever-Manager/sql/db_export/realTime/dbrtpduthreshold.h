#ifndef DBRTPDUTHRESHOLD_H
#define DBRTPDUTHRESHOLD_H
#include "database/basicsql.h"

struct PduThresholdItem{
    PduThresholdItem():id(-1),rid(-1),type(-1),index(-1),
        min(-1),max(-1),alarm(-1),crMin(-1),crMax(-1),crAlarm(-1){}

    int id, rid, type, index; // 类型 1：相阈值  2：输出位阈值	 3：温度	  4：湿度
    double min, max, alarm, crMin,crMax, crAlarm;
};
Q_DECLARE_METATYPE(PduThresholdItem)

class DbRtPduThreshold: BasicSql1<PduThresholdItem>
{
public:
    DbRtPduThreshold();
    static DbRtPduThreshold* get();
    virtual QString tableName(){return "rtPduThreshold";}

    void insertItem(PduThresholdItem &item);
    void updateItem(PduThresholdItem &item);

    int getId(int rid, int type, int index);
    QVector<PduThresholdItem> selItemsByRid(int rid);
    void removeItemsByRid(int rid);

protected:
    bool modifyItem(const PduThresholdItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,PduThresholdItem &item);

};

#endif // DBRTPDUTHRESHOLD_H
