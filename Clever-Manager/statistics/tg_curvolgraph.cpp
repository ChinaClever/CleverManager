/*
 * tg_curvolgraph.cpp
 * 总电流、电压统计图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_curvolgraph.h"

TG_CurVolGraph::TG_CurVolGraph(QWidget *parent) : QWidget(parent)
{
    initView();
    initSlot();
    QTimer::singleShot(4*1240,this,SLOT(initSlot()));
    isBig = false; // 大小端模式，false 为小端模式，  true 为大端模式
}

TG_CurVolGraph::~TG_CurVolGraph()
{
    delete m_pGraph;
}


void TG_CurVolGraph::initView()
{
#if 0
    groupBox = new QGroupBox(tr("总电流、平均电压曲线"),this);

    m_pGraph = new CurVolGraphic(groupBox);
    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(m_pGraph);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_pLayout = new QGridLayout(this);
    m_pLayout->addWidget(groupBox);
#else

    mBar = new TitleBar(this);
    mBar->setTitle(tr("总电流(A)、电压(V)曲线"));
    m_pGraph = new CurVolGraphic(this);

    QWidget *out_widget=new QWidget(this);
    out_widget->setStyleSheet("background-color:rgb(0,17,55)");
    QVBoxLayout *out_layout=new QVBoxLayout(out_widget);
    out_layout->addWidget(mBar);
    out_layout->addWidget(m_pGraph);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(out_widget, 0,0);
//    layout->addWidget(m_pGraph,1,0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#endif
}



void TG_CurVolGraph::initSlot(void)
{
    m_pGraph->clearData();
}

/**
 * @brief 设置时间轴范围
 * @param step
 */
void TG_CurVolGraph::setAutoRange(int step)
{
    m_pGraph->setAutoRange(step,24*60*60);
}

/**
 * @brief 增加电压数据
 * @param cur
 */
void TG_CurVolGraph::addCurData(double cur)
{
    if(cur >= 0)
        m_pGraph->addCurData(cur);
}

/**
 * @brief 增加电流数据
 * @param vol
 */
void TG_CurVolGraph::addVolData(int vol)
{
    if(vol >= 0)
        m_pGraph->addVolData(vol);
}

/**
 * @brief 获取电流数据，换算电流倍数
 * @param cur
 * @return
 */
double TG_CurVolGraph::getCurData(qint64 cur)
{
    if(cur > 10*1000) { // 大于1千A时，电流单位就变为KA
        cur /= 1000;
        if(isBig != true) { // 如果不处在大端模式，开启大端模式
            isBig = true;
            initSlot();
            mBar->setTitle(tr("总电流(KA)、电压(V)曲线"));
        }
    } else {
        if(isBig != false) { // 如果不处在小端模式，开启小端模式
            isBig = false;
            initSlot();
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
void TG_CurVolGraph::addData(int vol,qint64 cur,int step)
{
    setAutoRange(step);

    addCurData(getCurData(cur));
    addVolData(vol);
}
