/*
 * tg_devnumgraph.cpp
 * 设备数量统计柱形图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_devnumgraph.h"
#include "titlebar.h"

TG_DevNumGraph::TG_DevNumGraph(QWidget *parent) : QWidget(parent)
{
    mData = new BarChart_Data;
    initWidget();
    set_background_color(this, Qt::white);

    //    QTimer::singleShot(10*1000,this,SLOT(initSlot()));
}

TG_DevNumGraph::~TG_DevNumGraph()
{
    delete mData;
}

/**
 * @brief 布局
 */
void TG_DevNumGraph::initLayout(void)
{

}

/**
 * @brief 初始化窗口
 */
void TG_DevNumGraph::initWidget(void)
{
#if 0
    groupBox = new QGroupBox(tr("设备工作状态"),this);
    m_barChart = new BarCharts(groupBox);

    lab = new QLabel(groupBox);
    lab->setAlignment(Qt::AlignCenter);
    lab->setMaximumHeight(20);
    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(lab,0,0);
    layout->addWidget(m_barChart,1,0);

    m_pLayout = new QGridLayout(this);
    m_pLayout->addWidget(groupBox);
#else
    TitleBar *bar = new TitleBar(this);
    bar->setTitle(tr("设备工作状态"));

    groupBox = new QGroupBox("",this);
    m_barChart = new BarCharts(groupBox);
    groupBox->setStyleSheet("border:none; background-color:white");

    lab = new QLabel(groupBox);
    lab->setAlignment(Qt::AlignCenter);
    lab->setMaximumHeight(20);

    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(lab,0,0);
    layout->addWidget(m_barChart,1,0);
    layout->setSpacing(0);

    QWidget *out_widget=new QWidget(this);
    out_widget->setStyleSheet("background-color:rgb(0,17,55)");
    QVBoxLayout *out_layout=new QVBoxLayout(out_widget);
    out_layout->addWidget(bar);
    out_layout->addWidget(groupBox);
    out_layout->setSpacing(5);

    m_pLayout = new QGridLayout(this);
    m_pLayout->addWidget(out_widget,0,0);
    //    m_pLayout->addWidget(groupBox,1,0);
    m_pLayout->setContentsMargins(0, 0, 0, 0);
#endif
}

void TG_DevNumGraph::initSlot(void)
{
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(4*1000);
}

void TG_DevNumGraph::updateData(pdu_devNumStr *devStr)
{
    mData->number = devStr->num; // 设备总数
    mData->line = devStr->line; // 在线设备数量
    mData->alarm = devStr->alarm; //  报警数量
    mData->offLine = devStr->off; // 离线数量

    lab->setText(tr("总数：%1       在线：%2       报警：%3       离线：%4").arg(mData->number)
                 .arg(mData->line)
                 .arg(mData->alarm)
                 .arg(mData->offLine));
    lab->setFont(QFont("微软雅黑",12));
    lab->setStyleSheet("color:black");

    m_barChart->setData(mData);
}

void TG_DevNumGraph::timeoutDone(void)
{
    pdu_devNumStr *devStr = get_pdu_num();
    updateData(devStr);
}
