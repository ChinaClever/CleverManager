#ifndef DBRTPDUENVSENSOR_H
#define DBRTPDUENVSENSOR_H
#include "db_export/records/dbpduenvsensor.h"

class DbRtPduEnvSensor : public DbPduEnvSensor
{
     Q_OBJECT
    DbRtPduEnvSensor();
public:
    static DbRtPduEnvSensor* get();
    virtual QString tableName(){return "rtPduEnvSensor";}

    void updateItem(PduEnvSensorItem &item);
    int getId(int rid, int type);
};

#endif // DBRTPDUENVSENSOR_H
