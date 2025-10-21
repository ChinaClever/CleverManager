#include "diascatter.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include<qchart.h>
#include<qobject.h>
#include<QVBoxLayout>
#include<qdatetime.h>
#include<qcharts/diadata.h>

QT_CHARTS_USE_NAMESPACE


DiaScatter::DiaScatter(QWidget *parent ,QString ip,QDate start_date,QDate end_date):QDialog(parent)


{
     this->resize(800,400);

    mydata=new DiaData(ip,start_date,end_date);
    qDebug()<<"success0+++++++++++++++++++++++++++++++++++++++++++";

    QChart *chart=new QChart();
    //    chart->setRenderHint(QPainter::Antialiasing);

    chart->setTitle("Click to interact with scatter points");

    m_scatter = new QScatterSeries();
    m_scatter->setName("scatter1");
 chart->addSeries(m_scatter);

    //    for (qreal x(0.5); x <= 4.0; x += 0.5) {
    //        for (qreal y(0.5); y <= 4.0; y += 0.5) {
    //            *m_scatter << QPointF(x, y);
    //        }
    //    }
    //    m_scatter2 = new QScatterSeries();
    //    m_scatter2->setName("scatter2");

    //    chart->addSeries(m_scatter2);
    axisX = new QDateTimeAxis;
    axisX->setGridLineVisible(true); //设置是否显示网格线

    axisX->setTickCount(7);
    axisX->setFormat("yyyy-MM-dd hh:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    m_scatter->attachAxis(axisX);


    //    chart->createDefaultAxes();
    addData();
     qDebug()<<"success1+++++++++++++++++++++++++++++++++++++++++++";

    QDateTime mindatetime,maxdatetime;
    QString starttimestr="00:00:00";
    QString endtimestr="24:00:00";
    QTime starttime=QTime::fromString(starttimestr,"hh:mm:ss");
    QTime endttime=QTime::fromString(endtimestr,"hh:mm:ss");
//    QDate start_date=QDate::fromString(QString("2016-12-12"),"yyyy-MM-dd");
//    QDate end_date=QDate::fromString(QString("2016-12-15"),"yyyy-MM-dd");
    mindatetime.setDate(start_date);
    mindatetime.setTime(starttime);
    maxdatetime.setDate(end_date.addDays(1));
    maxdatetime.setTime(endttime);

    qDebug()<<mindatetime<<maxdatetime;
    axisX->setRange(mindatetime,maxdatetime); //初始化时第一次确定时间轴



    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,10);  //如果接近百分之八十，自动设置增加范围
    //    axisY->setLabelFormat("%i");
    axisY->setTitleText("电流");
    chart->addAxis(axisY, Qt::AlignLeft);
    m_scatter->attachAxis(axisY);


    connect(m_scatter, SIGNAL(clicked(QPointF)), this, SLOT(handleClickedPoint(QPointF)));

    QChartView *view=new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);  //抗锯齿渲染
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(view);
}

DiaScatter::~DiaScatter()
{
}

void DiaScatter::handleClickedPoint(const QPointF &point)
{
    QPointF clickedPoint = point;
    // Find the closest point from series 1
    QPointF closest(INT_MAX, INT_MAX);
    qreal distance(INT_MAX);
    foreach (QPointF currentPoint, m_scatter->points()) {
        qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
                                      * (currentPoint.x() - clickedPoint.x())
                                      + (currentPoint.y() - clickedPoint.y())
                                      * (currentPoint.y() - clickedPoint.y()));
        if (currentDistance < distance) {
            distance = currentDistance;
            closest = currentPoint;
        }
    }

    // Remove the closes point from series 1 and append it to series 2
    m_scatter->remove(closest);
    //    m_scatter2->append(closest);
}

void DiaScatter::addData()
{

    QDateTime datetime;
    double data;
    QMutableMapIterator<QDateTime,double> i(mydata->time_data);

    for(i.toFront();i.hasNext();) //
    {

        //       datetime=i.key();
        //       data=i.next().value();
//             qDebug()<<i.key()<<"helloworld"<<i.next().value();
        //              qDebug()<<datetime<<"helloworld"<<data;
        //              series->append(datetime.toMSecsSinceEpoch(),data);
        //     series->append(i.key().toMSecsSinceEpoch(),i.next().value());
      *m_scatter << QPointF(i.key().toMSecsSinceEpoch(),i.next().value());

    }

}
