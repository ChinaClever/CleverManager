/*
 * tg_datashow.cpp
 * 统计数据，显示主窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_datashow.h"

TG_DataShow::TG_DataShow(QWidget *parent) : QWidget(parent)
{
    mainSplitter=new QSplitter(Qt::Vertical,this);
    mainSplitter->setHandleWidth(20);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(0,0,0)}");
    //    mainSplitter->setStyleSheet("background-color:black");

    //mainSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(22, 117, 185); }");

    layout = new QGridLayout(this);
    layout->addWidget(mainSplitter);
    //    layout->setSpacing(0);
    layout->setContentsMargins(20, 23, 20, 20);

    initFunction();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(5*1000);
    QTimer::singleShot(3*1024,this,SLOT(initSlot()));

    m_pTgThread = new TG_DataThread(this);

    //    set_background_color(this, QColor(0,0,0));
}

TG_DataShow::~TG_DataShow()
{

}

void TG_DataShow::init(int id)
{
    mTGDataSt = m_pTgThread->set(id);
}

void TG_DataShow::initSlot()
{
    timeoutDone();
}

/**
 * @brief 定时更新数据
 */
void TG_DataShow::timeoutDone(void)
{    
    TG_DataType *data = mTGDataSt->data;
    if(data)
    {
        m_pDataDsp->updateData(data);
        m_pCurVOl->addData(data->vol/COM_RATE_VOL,data->cur,5);
        m_pPower->addPowData(data->pow,5);

        m_pDevNum->updateData(mTGDataSt->num);
    }
}

void TG_DataShow::initLayout(void)
{
    upSplitter->setStretchFactor(0,95);
    upSplitter->setStretchFactor(1,5);

    downSplitter->setStretchFactor(0,72);
    downSplitter->setStretchFactor(1,28);

    mainSplitter->setStretchFactor(0,39);
    mainSplitter->setStretchFactor(1,61);
}

/**
 * @brief 上部分分割窗口
 */
void TG_DataShow::initUpSpl(void)
{
    upSplitter=new QSplitter(Qt::Horizontal,mainSplitter);
    upSplitter->setHandleWidth(20);
    //     upSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(0,0,0)}");

    m_pPower = new TG_PowerGraph(upSplitter);
    m_pDataDsp = new TG_DataDsplay(upSplitter);
}

/**
  * @brief 下部分分割窗口
  */
void TG_DataShow::initDownSpl(void)
{
    downSplitter=new QSplitter(Qt::Horizontal,mainSplitter);
    downSplitter->setHandleWidth(20);
    downSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(0,0,0)}");
    m_pCurVOl = new TG_CurVolGraph(downSplitter);
    m_pDevNum = new TG_DevNumGraph(downSplitter);
}

void TG_DataShow::initFunction(void)
{
    initUpSpl();
    initDownSpl();
    initLayout();
}
