#include "diadata.h"

/**
 * @brief DiaData::DiaData
 * @param ip
 * @param start_date
 * @param end_date
 */
DiaData::DiaData(QString ip,QDate &start_date,QDate &end_date)
{

    mIndex=DbPduDevIndex::get();
    mElc= DbPduElecParams::get();
    mEnv= DbPduEnvSensor::get();

    startdate=start_date;
    enddate=end_date;
    ipstr=ip;

    init();
    getItems(); //已包含get_map;
    //    get_map();

    //    QMapIterator<QDateTime,double> i(time_data);
    //    for(;i.hasNext();) //
    //    {
    //       qDebug()<<i.key()<<"   "<<i.next().value();
    //    }
}


/**
 * @brief DiaData::get_map
 */
void DiaData::get_map()
{
    QDateTime datetime;
    QTime mytime;
    double data;
    //    qDebug()<<"success";


    for(int i=0;i<retItems.size();++i) //一天之内
    {

        mytime=QTime::fromString(retItems.at(i).time,"hh:mm:ss");
        datetime.setDate(mydate);
        datetime.setTime(mytime);

        int rid=retItems.at(i).id;

        items=mElc->selItemsByRid(rid);
        for(int j=0;j<items.size();j++) //根据rid返回四个电流值，分别为统计和三相，根据type取出统计值
        {
            if(items.at(j).type==0)
            {
                data=items.at(j).cur;
                time_data.insert(datetime,data);  //将时间点和数据插入键值对
            }

        }
    }

    time_data.insert(datetime.addSecs(1),0);  //一天结束时插入0
}


/*
 * 通过时间和ip筛选符合条件的项
 */
void DiaData::getItems()
{

    getDays(startdate,enddate);


    QString ip_str=ipstr;

    QMutableListIterator<QDate> i(date_list);


    for(i.toFront();i.hasNext();) //每一天
    {
        mydate=i.next();

        QString date_str=mydate.toString("yyyy-MM-dd");
        //        qDebug()<<date_str;
        retItems=mIndex->query(QString("where ip = \"%1\" and date =\"%2\" ").arg(ip_str).arg(date_str));
        //        qDebug()<<retItems.size();
        get_map();//根据返回的retitems，分别计算rid，并将符合条件的项插入
    }


}


/*
 * 如果有起始时间和结束时间，那么计算这之间有多少天,并存入qlist中
 */

/**
 * @brief DiaData::get_days
 * @param startdate
 * @param enddate
 */
void DiaData::getDays(const QDate &startdate,const QDate &enddate)
{
    QDate date;
    QMutableListIterator<QDate> i(date_list);

    for(date=startdate;date.operator <=(enddate);date=date.addDays(1))
    {
        i.insert(date);
        //        qDebug()<<date;
    }
    //    i.toFront();
    //     for(;i.hasNext();)
    //         qDebug()<<i.next();


}


void DiaData::init()
{
    QString str="1970-01-01 00:00:00";
    mpreviousdatetime=QDateTime::fromString(str,"yyyy-MM-dd hh:mm:ss");
    //    qDebug()<<mpreviousdatetime;
}
