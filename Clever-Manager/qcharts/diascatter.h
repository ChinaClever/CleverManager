#ifndef DIASCATTER_H
#define DIASCATTER_H
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include<qobject.h>
#include<QDialog>
#include<QtCharts/qdatetimeaxis.h>
#include<QtCharts/qvalueaxis.h>
#include<qcharts/diadata.h>
QT_CHARTS_USE_NAMESPACE

class DiaScatter:public QDialog
{
    Q_OBJECT
public:
    DiaScatter();

    DiaScatter(QWidget *parent,QString ip,QDate start_date,QDate end_date);
    ~DiaScatter();

private Q_SLOTS:
    void handleClickedPoint(const QPointF &point);

protected:


private:
    QScatterSeries *m_scatter;
    QScatterSeries *m_scatter2;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    DiaData *mydata;

    void addData();


};

#endif // DIASCATTER_H
