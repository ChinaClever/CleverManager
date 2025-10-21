/*
 * tg_curvolgraph.cpp
 * 总电流、电压统计图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "room_vcgraph.h"

Room_VcGraph::Room_VcGraph(QWidget *parent) : QWidget(parent)
{
    initView();
    clearSlot();
    QTimer::singleShot(4*1240,this,SLOT(clearSlot()));

    isBig = false; // 大小端模式，false 为小端模式，  true 为大端模式
    mPacket = nullptr;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(5*1000);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Room_VcGraph::~Room_VcGraph()
{
    delete m_pGraph;
}


void Room_VcGraph::initView()
{
    mBar = new Room_TitleBar(this);
    mBar->setTitle(tr("总电流(A)、电压(V)曲线"));
    m_pGraph = new Line_VcGraphic(this);

    QWidget *out_widget=new QWidget(this);
    out_widget->setStyleSheet("background-color:rgb(0,17,55)");
    QVBoxLayout *out_layout=new QVBoxLayout(out_widget);
    out_layout->addWidget(mBar);
    out_layout->addWidget(m_pGraph);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(out_widget, 0,0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}


void Room_VcGraph::clearSlot()
{
    m_pGraph->clearData();
}


/**
 * @brief 增加电压数据
 * @param cur
 */
void Room_VcGraph::addCurData(double cur)
{
    if(cur >= 0)
        m_pGraph->addCurData(cur);
}

/**
 * @brief 增加电流数据
 * @param vol
 */
void Room_VcGraph::addVolData(int vol)
{
    if(vol >= 0)
        m_pGraph->addVolData(vol);
}

/**
 * @brief 获取电流数据，换算电流倍数
 * @param cur
 * @return
 */
double Room_VcGraph::getCurData(qint64 cur)
{
    if(cur > 10*1000) { // 大于1千A时，电流单位就变为KA
        cur /= 1000;
        if(isBig != true) { // 如果不处在大端模式，开启大端模式
            isBig = true;
            clearSlot();
            mBar->setTitle(tr("总电流(KA)、电压(V)曲线"));
        }
    } else {
        if(isBig != false) { // 如果不处在小端模式，开启小端模式
            isBig = false;
            clearSlot();
            mBar->setTitle(tr("总电流(A)、电压(V)曲线"));
        }
    }

    return cur/COM_RATE_CUR;
}

/**
 * @brief 增加数据
 * @param vol
 * @param cur
 * @param step
 */
void Room_VcGraph::addData(int vol,qint64 cur)
{
    m_pGraph->setAutoRange(5 ,24*60*60);
    addCurData(getCurData(cur));
    addVolData(vol / COM_RATE_VOL);
}

bool Room_VcGraph::checkPack()
{
    bool ret = false;

    if(mPacket) {
        if(mPacket->en)
            ret = true;
    }

    return ret;
}

void Room_VcGraph::timeoutDone()
{
    bool ret = checkPack();
    if(ret) {
        addData(mPacket->tg.vol, mPacket->tg.cur);
    } else {
        clearSlot();
    }
}

void Room_VcGraph::packetSlot(sRoomPacket *p)
{
    mPacket=p;
    clearSlot();
}
