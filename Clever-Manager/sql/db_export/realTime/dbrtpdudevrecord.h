#ifndef DBRTPDUDEVRECORD_H
#define DBRTPDUDEVRECORD_H
#include "db_export/records/dbpdudevrecord.h"
#include "dbrtpduthreshold.h"

class DbRtPduDevRecord: public DbPduDevRecord
{
public:
    explicit DbRtPduDevRecord(QObject *parent = 0);

    static DbRtPduDevRecord *get(QObject *parent);
    void rtSlave(int devType, QString &ip, int devNum);
    void slaveThreshold(int index, PduDevData *devData);

protected:
    void insertThItem(PduThresholdItem &item);
    void slaveThDataUnit(int index, int type, PduDataUnit *dataUnit);

protected:
    virtual bool insertItem(PduDevIndexItem &item);
    virtual bool insertItem(PduElecParamsItem &item);
    virtual void insertItem(PduEnvSensorItem &item);

private:
    void changeItem(PduThresholdItem &item);
};

#endif // DBRTPDUDEVRECORD_H
