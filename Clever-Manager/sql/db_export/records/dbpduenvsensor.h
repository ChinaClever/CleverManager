#ifndef DBPDUENVSENSOR_H
#define DBPDUENVSENSOR_H
#include "database/basicsql.h"

// 环境状态表
struct PduEnvSensorItem{
    PduEnvSensorItem():id(-1),rid(-1),type(-1),
        sensor1(-1),sensor2(-1),sensor3(-1),sensor4(-1){}

    int id, rid, type; // (类型 1:温度 2:湿度 3门禁 4水濅 5烟雾)
    double sensor1,sensor2,sensor3,sensor4;
};
Q_DECLARE_METATYPE(PduEnvSensorItem)


class DbPduEnvSensor : public BasicSql1<PduEnvSensorItem>
{
    Q_OBJECT
public:
    DbPduEnvSensor();
    static DbPduEnvSensor* get();
    virtual QString tableName(){return "pduEnvSensor";}

    void insertItem(PduEnvSensorItem &item);
    QVector<PduEnvSensorItem> selItemsByRid(int rid);
    void removeItemsByRid(int rid);

protected:
    void createTable();
    bool modifyItem(const PduEnvSensorItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,PduEnvSensorItem &item);
};

#endif // DBPDUENVSENSOR_H
