#include "diagramwid.h"
#include "ui_diagramwid.h"

DiagramWid::DiagramWid( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiagramWid)
{
    ui->setupUi(this);

    mData = new chartData;
    //    mapData = new QMap<QDateTime ,double>;

    initChart();
    initScrollArea();


}

DiagramWid::~DiagramWid()
{
    delete ui;
}

void DiagramWid::initScrollArea()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    this->resize(desktopWidget->width()-200,desktopWidget->height()-240);

    QScrollArea *s = new QScrollArea(this);
    s->setAutoFillBackground(true);
    s->setGeometry(0, 0, this->width()-10,this->height()-15);
    qDebug() << "this" <<this->width() << "heigth:" << this->height();

    //垂直滚动条不可见，只能通过鼠标滑动
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mWid = new QWidget(s);
    QHBoxLayout *layout = new QHBoxLayout(mWid);
    layout->addWidget(mView);
    mWid->setGeometry(0, 0,this->width()+200 ,this->height());

    //设置滚动区域的窗体
    s->setWidget(mWid);
}

void DiagramWid::initChart()
{
    //    series=new QSplineSeries();
    series=new QLineSeries();
    series->setPen(QPen(Qt::blue,1));

    mChart = new QChart();

    mChart->setTitle(tr("时间电流曲线"));
    mChart->addSeries(series);
    mChart->legend()->hide();
    mChart->setAnimationOptions(QChart::SeriesAnimations);

    mAxisX = new QDateTimeAxis;
    mAxisX->setGridLineVisible(true); //设置是否显示网格线

    mAxisX->setTickCount(25);
    mAxisX->setFormat("yyyy-MM-dd hh:mm:ss");
    mAxisX->setTitleText("Time");
    mChart->addAxis(mAxisX, Qt::AlignBottom);
    series->attachAxis(mAxisX);
    //    mAxisX->setRange(mStartTime,mEndTime); //初始化时第一次确定时间轴

    mAxisY = new QValueAxis;
    mAxisY->setRange(0,300);  //如果接近百分之八十，自动设置增加范围
    //    axisY->setLabelFormat("%i");
    mAxisY->setTitleText(tr("电流"));

    mChart->addAxis(mAxisY, Qt::AlignLeft);
    series->attachAxis(mAxisY);

    mView=new QChartView(mChart);
    mChart->setTitle(tr("时间电流曲线"));
    mView->setRenderHint(QPainter::Antialiasing);  //抗锯齿渲染
}

void DiagramWid::mousePressEvent(QMouseEvent *event)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    int changeWidth = (desktopWidget->width())*0.5;

    int w = mWid->width();
    int h = mWid->height();

    if(event->button()==Qt::LeftButton)
    {
        mWid->resize(w+changeWidth  ,h);
    }else if(event->button() == Qt::RightButton)
        mWid->resize(w-changeWidth ,h);
}

void DiagramWid::getSignalChart(chartData *data)
{
    //    initChart();
    series->clear();

    mData = data;
    //确定时间轴范围及刻度
    QDateTime start = mData->startDateTime;
    QDateTime end = mData->endDateTime;
    int diff = abs(start.daysTo(end))+1;
    mAxisX->setTickCount(1+diff*24);
    mAxisX->setRange(start,start.addDays(diff));

    //确定Y轴名称
    mChart->setTitle(mData->title);

    ChartReadataThread *dataThread = new ChartReadataThread(data);
    dataThread->start();

    QTimer *timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutDone()));
    timer->start(1*50);

}

void DiagramWid::timeoutDone()
{
    mapData = read_Data();
    addData();
}

void DiagramWid::addData()
{
    qDebug() << "adddata" << mapData.size();

    QMapIterator<QDateTime , double> i(mapData);

    if(mapData.size())
    {
        for(i.toFront();i.hasNext();)
        {
            series->append(i.key().toMSecsSinceEpoch(),i.next().value());
        }
    }
}

void DiagramWid::initDataTime()
{

}


