/*
 * curvolgraphic.cpp
 * 电流、电压画布
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "line_vcgraphic.h"

Line_VcGraphic::Line_VcGraphic(QWidget *parent) : QWidget(parent)
{
    draw = new Line_DrawGraphic(this);
    layout = new QGridLayout(this);
    layout->addWidget(draw);    
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    initCurVolGraph();
    initData();
}

Line_VcGraphic::~Line_VcGraphic()
{

}

void Line_VcGraphic::initData(void)
{
    m_tick = 0;
    mRange = 60;

    mVolRange = 0;
    mCurRange = 0;

    setCurRange(5);
}

/**
  * @brief 自动调整时间轴
  * @param step 每次绘图时间间隔
  */
void Line_VcGraphic::setAutoRange(int step, int range)
{    
    if(m_tick<range)
    {
        m_tick += step;
        if(m_tick >= mRange)
        {
            int temp = mRange/6; // 增加的量
            mRange += temp; // 范围变大

            draw->addxAxis(temp); // 时间轴范围增大
        }
    }
}


/**
 * @brief 新增电流数据
 * @param value
 * @param alarm
 */
void Line_VcGraphic::addCurData(double value,bool alarm)
{
    bool ret =setCurRange(value); //调整范围

    draw->addData(m_pCurGraph,value,ret);
    if(alarm)  // 报警
        draw->addData(m_pCurAlarmGraph,value);
}

/**
 * @brief 清除数据
 */
void Line_VcGraphic::clearData(void)
{
    draw->clearData(m_pCurGraph);
    draw->clearData(m_pCurAlarmGraph);

    draw->clearData(m_pVolGraph);
    draw->clearData(m_pVolAlarmGraph);

    draw->setxAxis(60); //重新调整时间轴
    initData();
}

/**
 * @brief 新增电压数据
 * @param value
 * @param alarm
 */
void Line_VcGraphic::addVolData(int value,bool alarm)
{
    bool ret = setVolRange(value);

    draw->addData(m_pVolGraph,value,ret);
    if(alarm) draw->addData(m_pVolAlarmGraph,value);
}

/**
  * @brief 自动调整电压范围
  * @param range
  */
bool Line_VcGraphic::setVolRange(int value)
{
    bool ret = true;
    int range = mVolRange;

    if(value <= 60)
    {
        if(range != 60)
        {
            range = 60;
            draw->setyAxis2(0,range);
        }
    }
    else if(value < 165)
    {
        if(range != 165)
        {
            range = 165;
            draw->setyAxis2(60,range);
        }
    }
    else if(value <= 300)
    {
        if(range != 300)
        {
            range = 300;
            draw->setyAxis2(150,range);
        }
    }
    else if(value < 450)
    {
        if(range != 450)
        {
            range = 450;
            draw->setyAxis2(280,range);
        }
    }
    else
        ret = false;
    mVolRange = range;

    return ret;
}

/**
 * @brief 自动调整电流范围
 * @param value
 * 电流范围：0~10 0~16 0~20 0~32
 */
bool Line_VcGraphic::setCurRange(double value)
{
    bool ret = true;
    int range = mCurRange;

    if(value < 10)
    {
        if(range != 10)
        {
            range = 10;
            draw->setyAxis(0,10);
        }
    }
    else if(value < 16)
    {
        if(range != 16)
        {
            range = 16;
            draw->setyAxis(0,16);
        }
    }
    else if(value < 20)
    {
        if(range != 20)
        {
            range = 20;
            draw->setyAxis(0,20);
        }
    }
    else if(value < 32)
    {
        if(range != 32)
        {
            range = 32;
            draw->setyAxis(0,32);
        }
    }
    else{
        value += value/4;
        if(value >= range)
        {
            range = value;
            draw->setyAxis(0,range);
        }
        // ret = false;
    }
    mCurRange = range;

    return ret;
}

/**
 * @brief 初始化电流电压画布
 */
void Line_VcGraphic::initCurVolGraph(void)
{
    draw->initCurVolFun();

    m_pCurGraph = draw->addCurrentGraph("Current");
    m_pCurAlarmGraph = draw->addCurAlarmtGraph();

    m_pVolGraph = draw->addVoltageGraph("Voltage");
    m_pVolAlarmGraph = draw->addVolAlarmGraph();
}
