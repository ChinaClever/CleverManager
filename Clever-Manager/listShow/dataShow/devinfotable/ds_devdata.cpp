/*
 * ds_devdata.cpp
 * 设备数据显示界面
 *  1、显示输入相的电流、电压、功率、电能、功率因数等实时信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_devdata.h"
#include <QHeaderView>

DS_DevData::DS_DevData(QWidget *parent) : QWidget(parent)
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

DS_DevData::~DS_DevData()
{

}


/**
 * @brief 初始化函数
 */
void DS_DevData::initFunSlot()
{
    mDataPacket = NULL;
    initTableWidget();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2*1000);
}

/**
 * @brief 初始化窗口
 */
void DS_DevData::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("项目")<<tr("Line 1") << "Line 2" << "Line 3";
    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->setHorizontalHeaderLabels(header);
    mTableWidget->horizontalHeader()->setStyleSheet("background-color:rgb(233,233,233)");
mTableWidget->verticalHeader()->setStyleSheet("background-color:rgb(233,233,233)");
    mTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void DS_DevData::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignCenter);
    mTableWidget->setItem(row, column, item);

    if(column > 0)
        mItemList.append(item);
}

/**
 * @brief 增加一行Content
 * @param list
 */
void DS_DevData::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);
    mTableWidget->setRowHeight(row, 40);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);

    list.clear();
}

/**
 * @brief 初始化一行表格
 * @param str
 */
void DS_DevData::initTable(const QString &str)
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
void DS_DevData::initTableWidget(void)
{
    initWidget();

    initTable(tr("电压(V)"));
    initTable(tr("电流(A)"));

    initTable(tr("功率因数"));
    initTable(tr("功率(kW)"));
    initTable(tr("电能(kWh)"));
}

/**
 * @brief 清空表格
 */
void DS_DevData::clearTable(void)
{
    for(int i=0; i<mItemList.size(); ++i)
        mItemList.at(i)->setText("---");
}

/**
 * @brief 检查设备是否在线
 * @return
 */
bool DS_DevData::checkDev(void)
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
 * @brief 设置基本数据
 * @param id 相数
 * @param base 数据
 * @param symbol 单位
 * @param alarm 是否报警
 */
void DS_DevData::setDataBase(int id, PduDataBase *base, double rate,const QString &symbol, bool alarm)
{
    QTableWidgetItem *item = mItemList.at(mOffset++);
    int data = base->get(id);
    if(data >= 0)
        item->setText(QString::number(data/rate) + symbol);

    if(alarm)
        item->setTextColor(Qt::red);
    else
        item->setTextColor(Qt::black);
}

/**
 * @brief 检查是否报警
 * @param id 相数
 * @param unit 数据单元
 * @return  true 报警
 */
bool DS_DevData::checkAlarm(int id, PduDataUnit *unit)
{
    bool ret = false;
    int data = unit->alarm->get(id);
    if(data > 0)
        ret = true;

    return ret;
}

/**
 * @brief 设置数据单元
 * @param unit 数据
 * @param symbol 单位
 */
void DS_DevData::setDataUnit(PduDataUnit *unit, double rate,const QString &symbol)
{
    bool alarm;
    int line = 3;

    for(int i=0; i<line; ++i)
    {
        alarm = checkAlarm(i, unit);
        setDataBase(i,unit->value, rate,symbol, alarm);
    }

}

/**
 * @brief 设置数据
 * @param base 数据
 * @param symbol 单位
 */
void DS_DevData::setBase(PduDataBase *base, double rate,const QString &symbol)
{
    int line = 3;
    for(int i=0; i<line; ++i)
        setDataBase(i,base, rate,symbol);
}

/**
 * @brief 数据更新
 */
void DS_DevData::updateData(void)
{
    bool ret = checkDev();
    if(ret)
    {
        mOffset = 0;
        PduObjData *lineData = mDataPacket->data->line;
        setDataUnit(lineData->vol,COM_RATE_VOL, "V");
        setDataUnit(lineData->cur,COM_RATE_CUR,"A");

        setBase(lineData->pf,COM_RATE_PF,"");
        setBase(lineData->pow,COM_RATE_POW,"kW");
        setBase(lineData->ele, COM_RATE_ELE,"kWh");
    }
}

/**
 * @brief 更新
 * @param ip
 * @param num
 */
void DS_DevData::updateWidget(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    updateData();
}

/**
 * @brief 定时器响应函数
 */
void DS_DevData::timeoutDone(void)
{
    updateData();
}
