/*
 * 传入时间点和pdu ip，返回时间点与数据的键值对
 */
#ifndef DIADATA_H
#define DIADATA_H
#include "sql/db_export/records/dbtgroom.h"
#include"sql/db_export/records/dbpdudevindex.h"
#include "sql/db_export/records/dbpduelecparams.h"
#include "sql/db_export/records/dbpduenvsensor.h"
#include<QList>


class DiaData
{
public:
    DiaData(QString ip,QDate &start_date,QDate &end_date);
   void init();
//    void setTimeData();

     QMap<QDateTime,double> time_data;


protected:
    void getItems(); //获取符合条件的item
    void getDays( const QDate &startdate,const QDate &enddate);




private:
    DbPduDevIndex *mIndex;  //索引
    DbPduElecParams *mElc;  //电气设备

    DbPduEnvSensor *mEnv;   //环境
    QVector<PduDevIndexItem> retItems;
    QVector<PduElecParamsItem> items;
    QList<QDate> date_list;
    QDate startdate,enddate;
    QDateTime mpreviousdatetime;



    QDate mydate;
    QString ipstr;

    void get_map();


};

#endif // DIADATA_H
