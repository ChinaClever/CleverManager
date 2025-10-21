
/**
  *将数据库中保存的数据以时间-数据键值对的形式保存，传入charts进行显示
  *
  *
  */
#include "dbdata.h"

DbData::DbData()
{
    mIndex = DbPduDevIndex::get();
    mElc = DbPduElecParams::get();
}

/**
 * @brief DbData::setDate 设置起始及结束时间，获取该时间段内的数据
 * @param startTime
 * @param endTime
 */
void DbData::setDate(QDateTime &startTime , QDateTime &endTime)
{
    mStartTime = startTime ;
    mEndTime = endTime ;
}

/**
 * @brief DbData::getCurMap 获取电流map
 * @return
 * type(0 - 电压  1 - 电流)  num（输入相相数 - 1，2，3）
 */
QMap<QDateTime , double> &DbData::getCurMap(QString &ip , int type, int num)
{
    QString startDateStr = mStartTime.toString("yyyy-MM-dd");
    QString startTimeStr = mStartTime.toString("hh:mm:ss");
    QString endDateStr = mEndTime.toString("yyyy-MM-dd");
    QString endTimeStr = mEndTime.toString("hh:mm:ss");
    qDebug() << "startstr" << startDateStr << "endstr" << endDateStr;

    QString cmd = QString("where ip = \"%1\"").arg(ip);
    cmd += QString(" and date Between \'%1\' and  \'%2\' ").arg(startDateStr).arg(endDateStr);
    //cmd += QString(" and time Between \'%1\' and  \'%2\' ").arg(startTimeStr).arg(endTimeStr);

    //    QVector<PduDevIndexItem> items = mIndex->selItemsByIp(ip);
    QVector<PduDevIndexItem> items = mIndex->query(cmd);
    qDebug() << "index.size" << items.size() << type << num;
    for(int i = 0 ; i < items.size() ; i++)
    {
        QDateTime dateTime ;
        dateTime.setDate(QDate::fromString(items.at(i).date,"yyyy-MM-dd"));
        dateTime.setTime(QTime::fromString(items.at(i).time,"hh:mm:ss"));

        int rid = items.at(i).id;
        QString rcmd = QString("where rid = %1 and type = %2 and inde = %3").arg(rid).arg(1).arg(num+1);
        QVector<PduElecParamsItem> rItems =mElc->query(rcmd);
        qDebug() << "ritems.size" << rItems.size();
        for(int j = 0 ; j < rItems.size() ; j++)
        {
            qDebug() << "index---=:" << rItems.at(j).index;
            double data;
            if(type == 0)
                data=rItems.at(j).vol;
            if(type == 1)
                data = rItems.at(j).cur;

            mCurMap.insert(dateTime,data);  //将时间点和数据插入键值对
        }
    }

    qDebug() << "--------" <<mCurMap.size();
    return mCurMap;
}

void DbData::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void DbData::getData( chartData *packet,QMap<QDateTime ,double> *mapData)
{
    QString startDateStr = packet->startDateTime.toString("yyyy-MM-dd");
    QString endDateStr = packet->endDateTime.toString("yyyy-MM-dd");

    QString cmd = QString("where ip = \"%1\"").arg(packet->strIp);
    cmd += QString(" and date Between \'%1\' and  \'%2\' ").arg(startDateStr).arg(endDateStr);

    QVector<PduDevIndexItem> items = mIndex->query(cmd);
    for(int i = 0 ; i < items.size() ; i++)
    {
        QDateTime dateTime ;
        dateTime.setDate(QDate::fromString(items.at(i).date,"yyyy-MM-dd"));
        dateTime.setTime(QTime::fromString(items.at(i).time,"hh:mm:ss"));

        int rid = items.at(i).id;
        QString rcmd = QString("where rid = %1 and type = %2 and inde = %3").arg(rid).arg(packet->type).arg(packet->index);
        QVector<PduElecParamsItem> rItems =mElc->query(rcmd);
        qDebug() << "ritems.size" << rItems.size();
        for(int j = 0 ; j < rItems.size() ; j++)
        {
            double data;
            switch (packet->dataType) {
            case 1:
                data=rItems.at(j).cur;
                break;
            case 2:
                data=rItems.at(j).vol;
                break;
            case 3:
                data=rItems.at(j).pow;
                break;
            case 4:
                data=rItems.at(j).ele;
                break;
            case 5:
                data=rItems.at(j).pf;
                break;
            default:
                break;
            }
            mapData->insert(dateTime,data);  //将时间点和数据插入键值对
            while (mapData->size() > 0) {  //等待数据被读取，读取完之后再进行下一次的数据插入，
                 sleep(1);
            }
        }
    }
}




