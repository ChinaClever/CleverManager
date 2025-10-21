#include "chartreadatathread.h"
static QMap<QDateTime ,double> gMapData;


ChartReadataThread::ChartReadataThread(chartData *data)
{
    mData = new chartData;
    mDbData = new DbData;

    mData = data;
    isRun = false;
}

void ChartReadataThread::run()
{
//    qDebug() << "进入线程";
    if(!isRun)
    {
        isRun = true;
        mDbData->getData(mData,&gMapData);
        isRun = false;
    }
}

QMap<QDateTime ,double> read_Data()
{
    QMap<QDateTime ,double> map;
    map = gMapData;
    gMapData.clear();

    return map;
}


