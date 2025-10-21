/*
 * tg_devnumgraph.cpp
 * 设备数量统计柱形图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "room_bargraph.h"
#include "room_titlebar.h"

Room_BarGraph::Room_BarGraph(QWidget *parent) : QWidget(parent)
{
    initWidget();
    mPacket = nullptr;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(5*1000);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Room_BarGraph::~Room_BarGraph()
{

}

/**
 * @brief 初始化窗口
 */
void Room_BarGraph::initWidget()
{
    Room_TitleBar *bar = new Room_TitleBar(this);
    bar->setTitle(tr("设备工作状态"));

    QGroupBox *groupBox = new QGroupBox("",this);
    m_barChart = new Bar_ChartsWid(groupBox);
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

    QGridLayout *m_pLayout = new QGridLayout(this);
    m_pLayout->addWidget(out_widget,0,0);
    //    m_pLayout->addWidget(groupBox,1,0);
    m_pLayout->setContentsMargins(0, 0, 0, 0);
}




void Room_BarGraph::updateData(Bar_sChartData &data)
{
    lab->setText(tr("总数：%1       在线：%2       报警：%3       离线：%4").arg(data.number)
                 .arg(data.line)
                 .arg(data.alarm)
                 .arg(data.offLine));
    lab->setFont(QFont("微软雅黑",12));
    lab->setStyleSheet("color:black");

    m_barChart->setData(data);
}

void Room_BarGraph::timeoutDone()
{
    Bar_sChartData data;
    if(mPacket) {
        data.number = mPacket->pdus; // 设备总数
        data.line = mPacket->line; // 在线设备数量
        data.alarm = mPacket->alarm; //  报警数量
        data.offLine = mPacket->offline; // 离线数量
    } else {
        memset(&data, 0, sizeof(Bar_sChartData));
    }
    updateData(data);
}
