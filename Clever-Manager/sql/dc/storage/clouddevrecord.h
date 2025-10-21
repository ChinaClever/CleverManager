#ifndef CLOUDDEVRECORD_H
#define CLOUDDEVRECORD_H

#include <QObject>
#include "cloudpdudev.h"
#include "sql/db_export/records/dbpdudevrecord.h"
#include "sql/db_export/realTime/dbrtpdudevrecord.h"
#include "sql/db_export/records/dbtgroom.h"


class CloudDevRecord : public QObject
{
    Q_OBJECT
    explicit CloudDevRecord(QObject *parent = 0);
public:
    static CloudDevRecord* bulid();

    bool devTg(PduElecParamsItem &item, bool isSave);
    bool saveElecItem(PduElecParamsItem &item, bool isSave);
    bool saveEnv(int type, int num, double value, bool isSave);

    void saveThresholdItem(PduThresholdItem &item);
};

#endif // CLOUDDEVRECORD_H
