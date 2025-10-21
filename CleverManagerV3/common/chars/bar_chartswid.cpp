/*
 * barcharts.cpp
 * 设备工作状态柱形图
 *  设备总线、在线设备数量、报警数量、离线数量的柱形图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "bar_chartswid.h"

Bar_ChartsWid::Bar_ChartsWid(QWidget *parent) : QWidget(parent)
{
    m_drawChart = new Bar_DrawChart(this);
    layout = new QGridLayout(this);
    layout->addWidget(m_drawChart);
//    layout->setSpacing(0);
//    layout->setContentsMargins(0, 0, 0, 0);


    initBars();
    setColors();
    initXAxis();
}

Bar_ChartsWid::~Bar_ChartsWid()
{
}

/**
 * @brief 初始化
 */
void Bar_ChartsWid::initBars(void)
{
    numberBar = m_drawChart->createBar(tr("总数"));
    lineBar = m_drawChart->createBar(tr("在线"));
    alarmBar = m_drawChart->createBar(tr("报警"));
    offLineBar = m_drawChart->createBar(tr("离线"));
}

/**
 * @brief 设置颜色
 */
void Bar_ChartsWid::setColors(void)
{
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QColor(255,0,0));
    alarmBar->setPen(pen);
    alarmBar->setBrush(QColor(255,0,0));

    pen.setColor(QColor(188,188,188));
    offLineBar->setPen(pen);
    offLineBar->setBrush(QColor(188,188,188));

    pen.setColor(QColor(1,255,0));
    lineBar->setPen(pen);
    lineBar->setBrush(QColor(1,255,0));

    pen.setColor(QColor(0,204,255));
    numberBar->setPen(pen);
    numberBar->setBrush(QColor(0,204,255));
}

/**
 * @brief 设置X轴
 */
void Bar_ChartsWid::initXAxis(void)
{
    QVector<QString> labels;
    labels << tr("总数")<< tr("在线")<< tr("报警")<< tr("离线");

    m_drawChart->initXAxis(labels);
}

/**
 * @brief 设置Y轴范围
 */
void Bar_ChartsWid::setRange(int data)
{
    static int range=-1;
    if(range < data)
    {
        range = data + data/5 + 10;
        m_drawChart->setRange(range);
    }
}

void Bar_ChartsWid::setNumData(int data)
{
    QVector<double> ticks;
    ticks << 1;

    QVector<double> vecData;
    vecData << data;

    setRange(data);
  //  numberBar->clearData();
    numberBar->setData(ticks, vecData);
}

void Bar_ChartsWid::setLineData(int data)
{
    QVector<double> ticks;
    ticks << 2;

    QVector<double> vecData;
    vecData << data;
    lineBar->setData(ticks, vecData);
}

void Bar_ChartsWid::setAlarmData(int data)
{
    QVector<double> ticks;
    ticks << 3;

    QVector<double> vecData;
    vecData << data;

    alarmBar->setData(ticks, vecData);
}

void Bar_ChartsWid::setOffLineData(int data)
{
    QVector<double> ticks;
    ticks << 4;

    QVector<double> vecData;
    vecData << data;

    offLineBar->setData(ticks, vecData);
}

/**
 * @brief 设置数据
 */
void Bar_ChartsWid::setData(Bar_sChartData &data)
{
    setNumData(data.number);
    setLineData(data.line);
    setAlarmData(data.alarm);
    setOffLineData(data.offLine);

    m_drawChart->customPlot->replot();
}
