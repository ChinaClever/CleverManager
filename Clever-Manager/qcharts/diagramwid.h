#ifndef DIAGRAMWID_H
#define DIAGRAMWID_H

#include <QWidget>
#include <QDateTime>
#include <QtCharts/QChart>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QChartView>
#include <QLineSeries>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDesktopWidget>
#include <QDebug>
#include <QTimer>
#include "logs/log_historywid.h"
#include "chartdata.h"
#include "chartreadatathread.h"


namespace Ui {
class DiagramWid;
}
QT_CHARTS_USE_NAMESPACE


class DiagramWid : public QWidget
{
    Q_OBJECT

public:
    explicit DiagramWid(QWidget *parent = 0);
    ~DiagramWid();

private:
    Ui::DiagramWid *ui;
    QChart *mChart;
    QValueAxis *mAxisY;
    QDateTimeAxis *mAxisX;
    QChartView *mView;
    QWidget *mWid;
    chartData *mData;
    QMap<QDateTime ,double> mapData;
    QLineSeries *series;

private slots:
    void getSignalChart(chartData *data);
    void timeoutDone();

protected:
    void mousePressEvent(QMouseEvent *event);  //点击事件

private:
    void initChart();
    void initScrollArea();
    void addData();
    void initDataTime();

};

#endif // DIAGRAMWID_H
