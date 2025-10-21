/*
 * tg_powergraph.cpp
 * 总功率曲线图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "room_powergraph.h"


Room_PowerGraph::Room_PowerGraph(QWidget *parent) : QWidget(parent)
{
    initView();
    initFun();
    QTimer::singleShot(4*1240,this,SLOT(initSlot()));

    isBig = false; // 大小端模式，false 为小端模式，  true 为大端模式
    mPacket = nullptr;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(5*1000);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Room_PowerGraph::~Room_PowerGraph()
{

}

void Room_PowerGraph::initView()
{
    mBar = new Room_TitleBar(this);
    mBar->setTitle(tr("总功率曲线(kW)"));

    m_pDraw = new Line_DrawGraphic(this);
    QWidget *out_widget=new QWidget(this);
    out_widget->setStyleSheet("background-color:rgb(0,17,55)");

    QVBoxLayout *out_layout=new QVBoxLayout(out_widget);
    out_layout->addWidget(mBar);
    out_layout->addWidget(m_pDraw);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(out_widget, 0,0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}




/**
  * @brief 自动调整时间轴
  * @param step 每次绘图时间间隔
  */
void Room_PowerGraph::setAutoRange(int step)
{
    if(mTick < 24*60*60)
    {
        mTick += step;
        if(mTick >= mTrange)
        {
            int temp = mTrange / 6; // 增加的量
            mTrange += temp; // 范围变大
            m_pDraw->addxAxis(temp); // 时间轴范围增大
        }
    }
}

/**
 * @brief 单位换算
 * @param value
 * @return
 */
double Room_PowerGraph::getData(qint64 value)
{
    if(value > 1000*1000) { // 大于1千kW时，功率单位就变为K*kW
        value /= 1000;
        if(isBig != true) { // 如果不处在大端模式，开启大端模式
            isBig = true;
            initFun();
            mBar->setTitle(tr("总功率曲线(K*kW)"));
        }
    } else {
        if(isBig != false) { // 如果不处在小端模式，开启小端模式
            isBig = false;
            initFun();
            mBar->setTitle(tr("总功率曲线(kW)"));
        }
    }

    return value/COM_RATE_POW;
}

/**
  * @brief 增加数据
  * @param value
  * @param step
  */
void Room_PowerGraph::addPowData(qint64 pow,int step)
{
    double value = getData(pow);
    if(value >= 0)
    {
        setAutoRange(step); //自动调整时间轴
        if(value >= mRange) /*自动调整范围*/
        {
            mRange = value + value/4;
            m_pDraw->setyAxis(0,mRange);
        }

        m_pDraw->addData(m_pGraph,value);
    }
}

/**
  * @brief 清除前5S空白
  */
void Room_PowerGraph::initSlot()
{
    mRange = 5;
    mTick = 0;
    mTrange = 60;
    m_pDraw->setxAxis(60); //设置时间轴的范围为60S
    //    m_pDraw->clearData(m_pGraph);
}

/**
  * @brief 初始化
  */
void Room_PowerGraph::initFun()
{
    mRange = 5;
    mTick = 0;
    mTrange = 60;
    m_pGraph = m_pDraw->addPowerGraph("Power"); // 增加画布

    m_pDraw->initxAxis();  //初始化时间轴
    m_pDraw->setxAxis(mTrange); //设置时间轴的范围为60S

    m_pDraw->inityAxis("");  //初始化功率轴
    m_pDraw->setyAxis(0,mRange); //范围为
}


bool Room_PowerGraph::checkPack()
{
    bool ret = false;

    if(mPacket) {
        if(mPacket->en)
            ret = true;
    }

    return ret;
}

void Room_PowerGraph::timeoutDone()
{
    bool ret = checkPack();
    if(ret) {
        addPowData(mPacket->tg.pow, 5);
    } else {
        initSlot();
    }
}

void Room_PowerGraph::packetSlot(sRoomPacket *p)
{
    mPacket=p;
    initSlot();
}
