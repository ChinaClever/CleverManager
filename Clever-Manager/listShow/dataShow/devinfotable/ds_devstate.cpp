/*
 * ds_devstate.cpp
 * 设备统计信息界面
 *  1、显示设备的电压、电流、功率、电能、温湿度等统计信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_devstate.h"

DS_DevState::DS_DevState(QWidget *parent) : QWidget(parent)
{
    mTableWidget = new QTableWidget(this);
    mTableWidget->setFont(QFont("微软雅黑",10));
    mTableWidget->horizontalHeader()->setFont(QFont("微软雅黑",12));
    layout = new QGridLayout(this);
    layout->addWidget(mTableWidget);

    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QTimer::singleShot(5,this,SLOT(initFunSlot())); // 延时
}

DS_DevState::~DS_DevState()
{
    delete mPduTG;
    delete mPduData;
}


/**
 * @brief 初始化函数
 */
void DS_DevState::initFunSlot()
{
    mDataPacket = NULL;
    mPduTG = new PduPacketTG();
    mPduData = new sPduTGDataStr;
    initTableWidget();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2*1000);
}

/**
 * @brief 初始化窗口
 */
void DS_DevState::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("项目")<<tr("内容");
    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->setHorizontalHeaderLabels(header);
    mTableWidget->horizontalHeader()->setStyleSheet("background-color:rgb(233,233,233)");
mTableWidget->verticalHeader()->setStyleSheet("background-color:rgb(233,233,233)");

    for(int i=0; i<mTableWidget->columnCount(); ++i)
        mTableWidget->setColumnWidth(i,110);
    mTableWidget->horizontalHeader()->setStretchLastSection(true);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void DS_DevState::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignVCenter);
    mTableWidget->setItem(row, column, item);

    if(column > 0)
        mItemList.append(item);
}

/**
 * @brief 增加一行Content
 * @param list
 */
void DS_DevState::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);
    mTableWidget->setRowHeight(row, 37);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
    list.clear();
}

/**
 * @brief 初始化一行表格
 * @param str
 */
void DS_DevState::initTable(const QString &str)
{
    QStringList list;
    list << str;
    int column = mTableWidget->columnCount();
    for(int j=1; j<column; ++j)
        list << "---";
    addRowContent(list);
}

/**
 * @brief 初始化表格窗口
 */
void DS_DevState::initTableWidget(void)
{
    initWidget();

    initTable(tr("平均电压"));
    initTable(tr("总电流"));

    initTable(tr("功率因数"));
    initTable(tr("总功率"));
    initTable(tr("总电能"));

    initTable(tr("平均温度"));
    initTable(tr("平均湿度"));
}

/**
 * @brief 清空表格
 */
void DS_DevState::clearTable(void)
{
    for(int i=0; i<mItemList.size(); ++i)
        mItemList.at(i)->setText("---");
}

/**
 * @brief 检查设备是否在线
 * @return
 */
bool DS_DevState::checkDev(void)
{
    bool ret = false;
    if(mDataPacket)
    {
        clearTable();
        if(mDataPacket->offLine > 0)
            ret = true;
    }
    return ret;
}

/**
 * @brief 读取设备数据信息
 */
void DS_DevState::readTgData(void)
{
    memset(mPduData, 0, sizeof(sPduTGDataStr));
    sPduTGDataStr *data = mPduTG->getTgData(mDataPacket); // 获取统计数据
    if(data)
    {
        memcpy(mPduData, data, sizeof(sPduTGDataStr));
        delete data;
    }
}

/**
 * @brief 设置数据
 * @param data 数据
 * @param symbol 单位
 */
void DS_DevState::setData(double data, const QString &symbol)
{
    QTableWidgetItem *item = mItemList.at(mOffset++);
    if(data >= 0)
        item->setText(QString::number(data) + symbol);
}

/**
 * @brief 数据更新
 */
void DS_DevState::updateData(void)
{
    bool ret = checkDev();
    if(ret)
    {
        mOffset = 0;
        readTgData();

        setData(mPduData->vol/COM_RATE_VOL, "V");
        setData(mPduData->cur/COM_RATE_CUR, "A");
        setData(mPduData->pf/COM_RATE_PF, "");
        setData(mPduData->pow/COM_RATE_POW, "kW");
        setData(mPduData->ele/COM_RATE_ELE, "kWh");
        setData(mPduData->tem/COM_RATE_TEM, "℃");
        setData(mPduData->hum/COM_RATE_HUM, "%");
    }
}


/**
 * @brief 更新
 * @param ip
 * @param num
 */
void DS_DevState::updateWidget(QString &ip,int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    updateData();
}

/**
 * @brief 定时器响应函数
 */
void DS_DevState::timeoutDone(void)
{
    updateData();
}
