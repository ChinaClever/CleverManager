#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QDialog>
#include <QtCharts/qsplineseries.h>
#include <QLineSeries>
#include <pdu/data/statistics/pdutgcom.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include"diadata.h"
#include "dbdata.h"



QT_CHARTS_USE_NAMESPACE

class diagram : public QDialog
{
    Q_OBJECT

public:
    diagram(QWidget *parent = 0);
    ~diagram();

    void setDateTime(QDateTime firstTime ,QDateTime secondTime);
    void setIp(QString ip);
    bool setType(int index);
    void dataShow();

private:
    void initChart();
    void addData();//根据关联编号确定具体时间的电流，并将其显示在图表中
    void setTimeRange();
    void resetValue(QMap<QDateTime ,double> &data);
    void sleep(unsigned int msec);


    QDateTimeAxis *axisX;
    QValueAxis *axisY;

    PduDataPacket *current_packet;
    int cur,vol;  //电流电压

    QString ip_str;
    QString date_str;
    QString cur_str;
    QVector<PduDevIndexItem> retItems;
    QVector<PduElecParamsItem> items;
    QDate date1;
    QDate start_date,end_date;
    DiaData *mydata;
    QDateTime mStartTime , mEndTime;
    QString mIp;
    sPduTGDataStr pdu_data;
    //     QSplineSeries *series;
    QLineSeries *series;
    QDateTime current_dt;
    QString time;
    QTime current_time;
    QTimer *mTimer;
    DbData *mDbData;
    QMap<QDateTime , double> mData;
    int mType;

    QChart *mChart;
    QValueAxis *mAxisY;






};

#endif // WIDGET_H
