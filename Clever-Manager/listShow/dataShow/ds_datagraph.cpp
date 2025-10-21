/*
 * ds_datagraph.cpp
 *  设备电流、电压曲线图
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_datagraph.h"

DS_DataGraph::DS_DataGraph(QWidget *parent) : QWidget(parent)
{
    QWidget *groupBox = new QWidget(this); //tr("电流、电压曲线"),
    groupBox->setStyleSheet("background-color:rgb(0,0,0)");
    graph = new CurVolGraphic(this);

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

    QTimer::singleShot(2*1000,this,SLOT(initFun())); // 延时
}

DS_DataGraph::~DS_DataGraph()
{
    delete mPduTG;
    delete mPduData;
}

void DS_DataGraph::initFun(void)
{    
    mAlarm = 0;
    mDataPacket = NULL;
    mPduTG = new PduPacketTG();
    mPduData = new sPduTGDataStr;

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2*1000);
}


/**
 * @brief 报警检查
 * @param id
 * @param alarm
 * @return
 */
bool DS_DataGraph::checkAlarm(short id,int alarm)
{
    bool ret = false;

    if(alarm & (1<<id))
        ret = true;

    return ret;
}

/**
 * @brief 电压报警检查
 * @param alarm
 * @return
 */
bool DS_DataGraph::checkVolAlarm(int alarm)
{
    return checkAlarm(1,alarm);
}

/**
 * @brief 电流报警检查
 * @param alarm
 * @return
 */
bool DS_DataGraph::checkCurAlarm(int alarm)
{
    return checkAlarm(0,alarm);;
}

/**
 * @brief 是否是新设备
 * @param data
 * @return
 */
bool DS_DataGraph::isNewDev(DS_DsyDev  &dev)
{
    bool ret = false;

    if((mDeV.ip !=dev.ip) || (mDeV.num != dev.num) || (mDeV.line != dev.line))
        ret = true;

    return ret;
}


/**
 * @brief 检查设备的报警状态
 * @param line
 */
void DS_DataGraph::checkStatus(void)
{
    mAlarm = 0;
    int line = mDeV.line;
    if(line==0)
        return;
    else
        line--;

    PduObjData *lineData = mDataPacket->data->line;
    int ret = pdu_data_checkAlarm(lineData->cur, line); // 电流检查
    if(ret > 0)
        mAlarm |= (1<<0);
    else
        mAlarm &= ~(1<<0);

    ret = pdu_data_checkAlarm(lineData->vol, line);  // 检查电压是否报警
    if(ret > 0)
        mAlarm |= (1<<1);
    else
        mAlarm &= ~(1<<1);
}

/**
 * @brief 读取设备数据信息
 */
void DS_DataGraph::readData(void)
{
    short line = mDeV.line; // 0 表示统计
    if(line)
    {
        line--;
        memset(mPduData, 0, sizeof(sPduTGDataStr));

        PduObjData *lineData = mDataPacket->data->line;
        mPduData->vol = lineData->vol->value->get(line); // 电压数据
        mPduData->cur = lineData->cur->value->get(line); // 电流数据
    }
    else
    {
        sPduTGDataStr *data = mPduTG->getTgData(mDataPacket); // 获取统计数据
        if(data)
        {
            memcpy(mPduData, data, sizeof(sPduTGDataStr));
            delete data;
        }
    }
}


/**
 * @brief 图像更新
 * @param data
 */
void DS_DataGraph::updateGraph(void)
{
    int alarm = mAlarm;

    graph->setAutoRange(2,12*60*60);
    int data = mPduData->vol / COM_RATE_VOL;
    if(data >= 0)
    {
        bool ret = checkVolAlarm(alarm);
        graph->addVolData(data,ret);
    }

    data = mPduData->cur;
    if(data >= 0)
    {
        bool ret = checkCurAlarm(alarm);
        graph->addCurData((data/COM_RATE_CUR),ret);
    }
}

/**
 * @brief 检查设备是否在线
 * @return
 */
bool DS_DataGraph::checkDev(void)
{
    bool ret = false;

    if(mDataPacket)
    {
        if(mDataPacket->offLine > 0)
            ret = true;
    }

    if(ret == false)
    {
        mAlarm = 0;
        memset(mPduData, 0, sizeof(sPduTGDataStr));
    }

    return ret;
}


/**
 * @brief 定时器响应函数，自动刷新数据
 */
void DS_DataGraph::timeoutDone(void)
{
    if(mDeV.ip.isEmpty()) // IP地址为空就返回
        return ;

    bool ret = checkDev();
    if(ret) {
        readData();
        checkStatus();
    }

    updateGraph();
}

/**
 * @brief 更新设备信息
 * @param dev
 */
void DS_DataGraph::updateGraph(DS_DsyDev &dev)
{
    bool ret = isNewDev(dev);
    if(ret)
    {
        graph->clearData();

        mDataPacket = pdu_hd_getPacket(dev.ip, dev.num);
        mDeV = dev;

        timeoutDone();
    }
}
