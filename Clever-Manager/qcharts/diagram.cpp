#include "diagram.h"
#include <QtCharts/qsplineseries.h>
#include<qchart.h>
#include<qchartview.h>
#include<QVBoxLayout>
#include<QCategoryAxis>
#include<qdatetime.h>
#include<qtimer.h>
#include"common.h"

QT_CHARTS_USE_NAMESPACE

diagram::diagram(QWidget *parent)
    : QDialog(parent)
{

    this->resize(800,400);
    this->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    mDbData = new DbData ;
}

diagram::~diagram()
{

}

/**
 * @brief diagram::setDateTime 传入时间参数，作为时间轴起始点
 * @param firstTime
 * @param secondTime
 */
void diagram::setDateTime(QDateTime firstTime, QDateTime secondTime)
{
    mStartTime = firstTime ;
    mEndTime = secondTime ;
    mDbData->setDate(firstTime , secondTime); //同时获取该时间段内数据
}

/**
 * @brief diagram::setIp 传入ip，显示该ip相关数据
 * @param ip
 */
void diagram::setIp(QString ip)
{
    //    qDebug() << "---IP---" << ip;
    mIp = ip ;
}


/*
 * 初始化图表
 */
void diagram::initChart()
{
    //    series=new QSplineSeries();
    series=new QLineSeries();
    series->setPen(QPen(Qt::blue,1));

    mChart=new QChart();

    mChart->setTitle(tr("时间电流曲线"));
    mChart->addSeries(series);
    mChart->legend()->hide();
    mChart->setAnimationOptions(QChart::SeriesAnimations);
    //    chart->createDefaultAxes();

    axisX = new QDateTimeAxis;
    axisX->setGridLineVisible(true); //设置是否显示网格线

    axisX->setTickCount(7);
    axisX->setFormat("yyyy-MM-dd hh:mm:ss");
    axisX->setTitleText("Time");
    mChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisX->setRange(mStartTime,mEndTime); //初始化时第一次确定时间轴

    mAxisY = new QValueAxis;
    mAxisY->setRange(0,300);  //如果接近百分之八十，自动设置增加范围
    //    axisY->setLabelFormat("%i");
    mAxisY->setTitleText(tr("电流"));
    mChart->addAxis(mAxisY, Qt::AlignLeft);
    series->attachAxis(mAxisY);

    QChartView *view=new QChartView(mChart);
    view->setRenderHint(QPainter::Antialiasing);  //抗锯齿渲染
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(view);
}

/*
 * 加载数据
 */
void diagram::addData()
{
    qDebug() << "addData" << mData.size();
    resetValue(mData);
    QMapIterator<QDateTime , double> i(mData);
    for(i.toFront();i.hasNext();) //
    {
        //                qDebug() << "datetime" << i.key().toMSecsSinceEpoch() << "value" << i.next().value();
        series->append(i.key().toMSecsSinceEpoch(),i.next().value());
//        sleep(1000);
    }
}

void diagram::dataShow()
{
    addData();
}

bool diagram::setType(int index)
{
    mType = index;
    QString title,titleText;
    int max;
    int type,num;

    if(mType < 3)
    {
        title = tr("时间电压曲线");
        titleText = tr("电压");
        max = 300;
        type = 0 ; //输入相电压
        num = mType; //相数
    }else if((mType > 2) && (mType < 6))
    {
        title = tr("时间电流曲线");
        titleText = tr("电流");
        max = 3;
        type = 1; //输入相电流
        num = mType - 3; //相数
    }

    mData = mDbData->getCurMap(mIp,type,num);

    if(mData.size() != 0)
    {
        mStartTime = mData.begin().key();
        setTimeRange();
    }
    else
    {
        QMessageBox::information(this,"information","该设备无数据，无法生成图表","确定");
        this->close();
        return false;
    }

    qDebug() << "start"<<mStartTime << "end"<< mEndTime;

    initChart();

    mChart->setTitle(title);
    mAxisY->setRange(0,max);
    mAxisY->setTitleText(titleText);

    return true;
    //    mData = mDbData->getCurMap(mIp,type,num);
}

void diagram::setTimeRange()
{
    QMapIterator<QDateTime , double> i(mData);
    for(i.toFront();i.hasNext();) //
    {
        qDebug() << "datetime" << i.key() << i.next().value();
    }
    mEndTime = i.key();
}

void diagram::resetValue(QMap<QDateTime, double> &data)
{
    QDateTime lastTime = mStartTime;
    int ret = 0xffff;
    QMutableMapIterator<QDateTime , double> i(data);
    i.toFront();
    i.next();
    lastTime = i.key();

    for(i.next();i.hasNext();i.next()) //
    {
        int diff = i.key().secsTo(lastTime);
        if(diff<0) diff = abs(diff);
        //        qDebug() << "A" << diff << ret;

        if(diff > 2*ret) //如果两点之间差值比上一次两点差值大，那么修改两次值为0，此时即为离线
        {
            data.insert(i.key(), 0);
            data.insert(lastTime, 0);
        }

        lastTime = i.key();
        ret = diff;
    }
}

void diagram::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

