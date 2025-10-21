#ifndef DBDATA_H
#define DBDATA_H

#include "sql/db_export/records/dbtgroom.h"
#include"sql/db_export/records/dbpdudevindex.h"
#include "sql/db_export/records/dbpduelecparams.h"
#include "sql/db_export/records/dbpduenvsensor.h"
#include "chartdata.h"


class DbData
{
public:
    DbData();

    void setDate(QDateTime &startTime, QDateTime &endTime);

    QMap<QDateTime, double> &getCurMap(QString &ip, int type, int num);
     void getData(chartData *packet , QMap<QDateTime, double> *mapData);

private:
    DbPduDevIndex *mIndex;  //索引
    DbPduElecParams *mElc;  //电气设备
    DbPduEnvSensor *mEnv;   //环境


    QMap<QDateTime ,double> mCurMap;

    QDateTime mStartTime;
    QDateTime mEndTime;

signals:
    void newData(QDateTime &datetime,double data);

private:
    void sleep(unsigned int msec);




};

#endif // DBDATA_H
