/*
 * drawchart.cpp
 * 柱形图画布
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "bar_drawchart.h"

Bar_DrawChart::Bar_DrawChart(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    layout = new QGridLayout(this);
    layout->addWidget(customPlot);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    initYAxis();
   // initLegend();
}

Bar_DrawChart::~Bar_DrawChart()
{

}

/**
 * @brief 创建Bar
 * @param name
 * @return
 */
QCPBars *Bar_DrawChart::createBar(QString name)
{
    QCPBars *bar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    bar->setName(name);
    customPlot->addPlottable(bar);

    return bar;
}


/**
 * @brief 初始化X轴
 * @param labels
 */
void Bar_DrawChart::initXAxis(QVector<QString> &labels)
{
    QVector<double> ticks;
    int count = labels.size();

    for(int i=1; i<=count; ++i)
        ticks << i;

    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    //customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, count+1);
}

 void Bar_DrawChart::initYAxis(void)
 {
     customPlot->yAxis->setPadding(5); // a bit more space to the left border
     customPlot->yAxis->grid()->setSubGridVisible(true);

     QPen gridPen;
     gridPen.setStyle(Qt::SolidLine);
     gridPen.setColor(QColor(0, 0, 0, 25));
     customPlot->yAxis->grid()->setPen(gridPen);
     gridPen.setStyle(Qt::DotLine);
     customPlot->yAxis->grid()->setSubGridPen(gridPen);
 }

/**
 * @brief 初始化Y轴
 * @param lab
 */
void Bar_DrawChart::setYLabel(QString lab)
{
    customPlot->yAxis->setLabel(lab);
}

/**
 * @brief Y轴范围
 * @param range
 */
void Bar_DrawChart::setRange(int range)
{
    customPlot->yAxis->setRange(0, range);
}

void Bar_DrawChart::initLegend(void)
{
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));

    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
