/*
 * drawgraphic.cpp
 * 画布的基本操作
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "line_drawgraphic.h"


Line_DrawGraphic::Line_DrawGraphic(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    layout = new QGridLayout(this);
    layout->addWidget(customPlot);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    customPlot->setMinimumHeight(150);

    m_xRange = 60;
    m_startTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000; //获取当前时间
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    //  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

Line_DrawGraphic::~Line_DrawGraphic()
{

}

/**
  * @brief 默认初始化
  */
 void Line_DrawGraphic::initCurVolFun(void)
 {
     initxAxis();  //初始化时间轴
     setxAxis(60); //设置时间轴的范围为60S

     QString curStr;
//     curStr = tr("电流"); ////====
     inityAxis(curStr);  //初始化电流轴
     setyAxis(0,10); //默认电流范围为20A

     QString volStr;
//     volStr = tr("电压"); /////====
     inityAxis2(volStr); //初始化电压轴
     setyAxis2(160,300);

     initxAxis2();

     initLegende();
 }


 /**
   * @brief 增加功率画布
   * @param name
   * @return
   */
 QCPGraph *Line_DrawGraphic::addPowerGraph(const QString &name)
 {
     QCPGraph *graph = customPlot->addGraph();

     QPen pen;
//     pen.setStyle(Qt::DashLine); //虚线
     pen.setWidth(3);
     pen.setColor(QColor(55,40,255));
//     graph->setBrush(QBrush(QColor(255,50,30,20)));

     graph->setPen(pen);
     graph->setName(name);

     return graph;
 }



/**
 * @brief 画数据
 * @param graph
 * @param value
 */
void Line_DrawGraphic::addData(QCPGraph *graph, double value,bool onlyEnlarge)
{
    int key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000; //获取当前时间

    graph->addData(key,value);
    graph->rescaleValueAxis(onlyEnlarge);

    if(key - m_startTime > m_xRange) /*最开始与现在时间差大于范围*/
    {
        graph->removeDataBefore(key-m_xRange-2); /*移除过时数据*/
        customPlot->xAxis->setRange(key, m_xRange,Qt::AlignRight);/*调整范围*/
    }

    customPlot->replot();
}

/**
  * @brief 清除所有数据
  * @param graph
  */
 void Line_DrawGraphic::clearData(QCPGraph *graph)
 {
     graph->clearData();
 }

/**
 * @brief 增加电压画布
 */
QCPGraph *Line_DrawGraphic::addVoltageGraph(const QString &name)
{
    QCPGraph *graph = customPlot->addGraph(customPlot->xAxis, customPlot->yAxis2);

    QPen pen;
//    pen.setStyle(Qt::DashLine); //点线
    pen.setWidth(3);
    pen.setColor(QColor(0,204,255));
//    graph->setBrush(QBrush(QColor(216,231,224)));

    graph->setPen(pen);
    graph->setName(name);

    return graph;
}

/**
  * @brief 增加电压报警画布
  * @param name
  * @return
  */
 QCPGraph *Line_DrawGraphic::addVolAlarmGraph(void)
 {
     QCPGraph *graph = customPlot->addGraph(customPlot->xAxis, customPlot->yAxis2);

     graph->setPen(QPen(Qt::blue));
     setScatterStyle(graph,5);
     customPlot->legend->removeItem(customPlot->legend->itemCount()-1);

     return graph;
 }

/**
 * @brief 增加电流画布
 */
QCPGraph *Line_DrawGraphic::addCurrentGraph(const QString &name)
{
    QCPGraph *graph = customPlot->addGraph();

    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(55,3,152));
//    graph->setBrush(QBrush(QColor(221,213,255)));

    graph->setPen(pen);
    graph->setName(name);

    return graph;
}

/**
  * @brief 增加电流报警画布
  * @return
  */
 QCPGraph *Line_DrawGraphic::addCurAlarmtGraph(void)
 {
     QCPGraph *graph = customPlot->addGraph();

     graph->setPen(QPen(Qt::red));
     setScatterStyle(graph,6);
     customPlot->legend->removeItem(customPlot->legend->itemCount()-1);

     return graph;
 }


/**
  * @brief 显示legend
  */
void Line_DrawGraphic::initLegende(void)
{
    customPlot->legend->setVisible(true);
    // set locale to english, so we get english decimal separator:
    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    QFont legendFont = font();
    legendFont.setPointSize(9);
    customPlot->legend->setFont(legendFont);
    //customPlot->legend->setBrush(QBrush(QColor(255,255,255,230))); // 背景色
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    //customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
    //customPlot->legend->setIconSize(50, 20); // 大小
}


/**
 * @brief 报警时选择形状
 *
 */
void Line_DrawGraphic::setScatterStyle(QCPGraph *graph, int id)
{
    QVector<QCPScatterStyle::ScatterShape> shapes;
    shapes << QCPScatterStyle::ssCross;
    shapes << QCPScatterStyle::ssPlus;
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond; // 5 电压报警图标
    shapes << QCPScatterStyle::ssStar; // 6 电流报警图标
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssTriangleInverted;
    shapes << QCPScatterStyle::ssCrossSquare;
    shapes << QCPScatterStyle::ssPlusSquare;
    shapes << QCPScatterStyle::ssCrossCircle;
    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;
    shapes << QCPScatterStyle::ssCustom;

    graph->setLineStyle(QCPGraph::lsNone);
    if (shapes.at(id) != QCPScatterStyle::ssCustom)
        graph->setScatterStyle(QCPScatterStyle(shapes.at(id), 10));  // set scatter style:
}


/**
 * @brief X轴初始化
 */
void Line_DrawGraphic::initxAxis(void)
{
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
//   customPlot->setFont(QFont("微软雅黑",12));
//    customPlot->xAxis->setLabel("Time");

    //customPlot->xAxis->setTickLabelRotation(30); //字体角度

    // customPlot->xAxis->setAutoTickCount(10); //分配刻度数量
    // customPlot->xAxis->setSubTickCount(4);
    // customPlot->xAxis->setTickStep(1);
    // customPlot->xAxis->setAutoTickStep(false); //设置是不自动分配刻度间距
}

/**
 * @brief 时间轴范围增大
 * @param range
 */
void Line_DrawGraphic::addxAxis(int range)
{
    m_xRange += range;
    customPlot->xAxis->setRange(m_startTime, m_xRange,Qt::AlignLeft);
}


/**
 * @brief X轴花园设置 以时间 S为单位
 */
void Line_DrawGraphic::setxAxis(int range)
{
    m_startTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000; //获取当前时间

    m_xRange = range;
    customPlot->xAxis->setRange(m_startTime, m_xRange,Qt::AlignLeft);
}

/**
 * @brief Y轴初始化
 */
void Line_DrawGraphic::inityAxis(const QString &lab)
{
    customPlot->yAxis->setLabel(lab);
}

void Line_DrawGraphic::setyAxis(double lower, double upper)
{
    customPlot->yAxis->setRange(lower, upper);
}

/**
  * @brief 上轴初始化
  */
void Line_DrawGraphic::initxAxis2(void)
{
    customPlot->xAxis2->setVisible(false);
    customPlot->xAxis2->setTickLabels(false);
    //  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

/**
  * @brief 右轴初始化
  */
void Line_DrawGraphic::inityAxis2(const QString &lab)
{
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setLabel(lab);
}


/**
  * @brief 设置右轴
  */
void Line_DrawGraphic::setyAxis2(double lower, double upper)
{
    customPlot->yAxis2->setRange(lower, upper);
}


/**
 * @brief 设置标题
 */
void Line_DrawGraphic::setTitle(const QString &title)
{
    customPlot->plotLayout()->insertRow(0);  // set title of plot:
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, title));

}

/**
 * @brief 设置画布名字
 */
void Line_DrawGraphic::setName(const QString &name)
{
    customPlot->graph()->setName(name); // set graph name, will show up in legend next to icon:
}

/**
 * @brief 初始化Axis 设置刻度显示方式
 */
void Line_DrawGraphic::initAxis(QCPAxis *Axis)
{
    Axis->setBasePen(QPen(Qt::white, 1));
    Axis->setTickPen(QPen(Qt::white, 1));
    Axis->setSubTickPen(QPen(Qt::white, 1));
    Axis->setTickLabelColor(Qt::white);
    Axis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    Axis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    Axis->grid()->setSubGridVisible(true);
    Axis->grid()->setZeroLinePen(Qt::NoPen);
    Axis->setUpperEnding(QCPLineEnding::esSpikeArrow);
}


/**
 * @brief 设置背景
 */
void Line_DrawGraphic::setBackground(void)
{
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);

    //customPlot->axisRect()->setBackground(QPixmap("./solarpanels.jpg"));
}
