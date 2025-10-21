#ifndef CHARTREADATATHREAD_H
#define CHARTREADATATHREAD_H
#include <QThread>
#include "dbdata.h"
#include "chartdata.h"


class ChartReadataThread : public QThread
{
public:
    ChartReadataThread(chartData *data);

protected:
    void run(void);

private:
    DbData *mDbData;
    chartData *mData;
//    QMap<QDateTime ,double> mapData;
    bool isRun;
};


QMap<QDateTime, double> read_Data();
#endif // CHARTREADATATHREAD_H
