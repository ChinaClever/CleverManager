/*
 * ds_datagraph.cpp
 *  设备电流、电压曲线图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_linegraph.h"

Pdu_LineGraph::Pdu_LineGraph(QWidget *parent) : QWidget(parent)
{
    QWidget *groupBox = new QWidget(this); //tr("电流、电压曲线"),
    groupBox->setStyleSheet("background-color:rgb(0,0,0)");
    graph = new Line_VcGraphic(this);

    QGridLayout *layout = new QGridLayout(groupBox);
    QLabel *title=new QLabel(tr("  电流电压曲线"),this);
    title->setFixedHeight(40);
    title->setFont(QFont("微软雅黑",12));
    title->setStyleSheet("background-color:rgb(255,255,255);" "color:black");
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    title->setPalette(pal);

    layout->addWidget(title);
    layout->addWidget(graph);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    mLayout = new QGridLayout(this);
    mLayout->addWidget(groupBox);
    mLayout->setContentsMargins(15, 0, 15, 10);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2*1000);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Pdu_LineGraph::~Pdu_LineGraph()
{

}


/**
 * @brief 图像更新
 * @param data
 */
void Pdu_LineGraph::updateGraph()
{
    graph->setAutoRange(2,12*60*60);

    int line = mLine;
    sDataUnit *unit = &(mPacket->data.line.cur);
    int data = unit->value[line]/ COM_RATE_CUR;
    if(data >= 0) {
        graph->addCurData(data, unit->alarm[line]);
    }

    unit = &(mPacket->data.line.vol);
    data = unit->value[line]/ COM_RATE_VOL;
    if(data >= 0) {
        graph->addVolData((data),unit->alarm[line]);
    }
}

/**
 * @brief 检查设备是否在线
 * @return
 */
bool Pdu_LineGraph::checkDev()
{
    bool ret = false;

    if(mPacket) {
         if((mPacket->offLine > 0) &&(mPacket->en))
            ret = true;
    }

    return ret;
}


/**
 * @brief 定时器响应函数，自动刷新数据
 */
void Pdu_LineGraph::timeoutDone()
{
    bool ret = checkDev();
    if(ret) {
        updateGraph();
    } else {
        graph->clearData();
    }
}

void Pdu_LineGraph::packetSlot(sDataPacket *p, int line)
{
    mPacket = p;
    mLine = line;
    graph->clearData();
}
