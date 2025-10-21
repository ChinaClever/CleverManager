/*
 * ds_threshold.cpp
 *  设备阈值显示界面
 *  主要显示相电流、相电压的最大值，最小值，当前值等实时信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_threshold.h"
#include "listShow/devSetting/setthresholddlg.h"

DS_Threshold::DS_Threshold(QWidget *parent) : QWidget(parent)
{
    mTableWidget = new QTableWidget(this);
    layout = new QGridLayout(this);
    layout->addWidget(mTableWidget);

//    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
//    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(mTableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(updateWidget(QTableWidgetItem *)));

    QTimer::singleShot(5,this,SLOT(initFunSlot())); // 延时
}

DS_Threshold::~DS_Threshold()
{

}

/**
 * @brief 初始化函数
 */
void DS_Threshold::initFunSlot()
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
void DS_Threshold::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/


    QStringList header;
    header << tr("相数") << tr("项目") << tr("当前值") << tr("最小值") << tr("最大值");
    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->horizontalHeader()->setFont(QFont("微软雅黑",12));
    mTableWidget->setFont(QFont("微软雅黑",10));
    mTableWidget->setHorizontalHeaderLabels(header);
    mTableWidget->horizontalHeader()->setStyleSheet("background-color:rgb(233,233,233)");
mTableWidget->verticalHeader()->setStyleSheet("background-color:rgb(233,233,233)");

    //    mTableWidget->resizeColumnsToContents();
    mTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTableWidget->setColumnWidth(0,45);
    mTableWidget->horizontalHeader()->setStretchLastSection(true);
    mTableWidget->setShowGrid(true); //设置显示格子线
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void DS_Threshold::addItemContent(int row, int column,const QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignCenter);
    mTableWidget->setItem(row, column, item);

    if(column > 1)
        mItemList.append(item);
}

/**
 * @brief 增加一行数据
 * @param list
 */
void DS_Threshold::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);
    mTableWidget->setRowHeight(row, 40);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i+1,list[i]);
}

/**
 * @brief 合并单元格
 */
void DS_Threshold::initSpanTable(void)
{
    mTableWidget->setSpan(0, 0, 2, 1);
    addItemContent(0,0,"L 1");

    mTableWidget->setSpan(2, 0, 2, 1);
    addItemContent(2,0,"L 2");

    mTableWidget->setSpan(4, 0, 2, 1);
    addItemContent(4,0,"L 3");
}


void DS_Threshold::initTable(const QString &str)
{
    QStringList list;
    list << str;
    int column = mTableWidget->columnCount();
    for(int j=2; j<column; ++j)
        list << "---";
    addRowContent(list);
}


/**
 * @brief 初始化表格窗口
 */
void DS_Threshold::initTableWidget(void)
{
    QString str;
    int line = 3; // 3相
    initWidget();

    for(int i=0; i<line; ++i)
    {
        str = tr("电压 L%1").arg(i+1);
        initTable(str);

        str = tr("电流 L%1").arg(i+1);
        initTable(str);
    }

    initSpanTable();
}

/**
 * @brief 清空表格
 */
void DS_Threshold::clearTable(void)
{
    for(int i=0; i<mItemList.size(); ++i)
        mItemList.at(i)->setText("---");
}

/**
 * @brief 检查设备是否在线
 * @return
 */
bool DS_Threshold::checkDev(void)
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
 * @brief 数据显示至界面上
 * @param id
 * @param base
 */
void DS_Threshold::setDataBase(int id, PduDataBase *base, double rate, const QString &symbol)
{
    QTableWidgetItem *item = mItemList.at(mOffset++);
    int data = base->get(id);
    if(data >= 0)
        item->setText(QString::number(data/rate) + symbol);
}


/**
 * @brief 检查是否报警
 * @param id
 * @param unit
 */
void DS_Threshold::checkAlarm(int id, PduDataUnit *unit)
{
    QTableWidgetItem *item = mItemList.at(mAlarmOffset);
    int data = unit->alarm->get(id);
    if(data > 0)
        item->setTextColor(Qt::red);
    else
        item->setTextColor(Qt::black);

    mAlarmOffset += 3; // 隔三个
}


/**
 * @brief 显示当前值、最小值、最大值
 * @param id
 * @param unit
 */
void DS_Threshold::setDataUnit(int id, PduDataUnit *unit,  double rate, const QString &symbol)
{
    setDataBase(id, unit->value, rate, symbol);
    setDataBase(id, unit->min, rate, symbol);
    setDataBase(id, unit->max, rate, symbol);

    checkAlarm(id, unit);
}


/**
 * @brief 数据更新
 */
void DS_Threshold::updateData(void)
{
    bool ret = checkDev();
    if(ret)
    {
        PduObjData *lineData = mDataPacket->data->line;
        mOffset = mAlarmOffset = 0;
        for(int i=0; i<3; ++i)
        {
            setDataUnit(i, lineData->vol, COM_RATE_VOL, "V");
            setDataUnit(i, lineData->cur, COM_RATE_CUR, "A");
        }
    }
}

/**
 * @brief 更新窗口
 * @param ip 设备IP
 * @param num 设备号
 */
void DS_Threshold::updateWidget(QString &ip,int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    updateData();
}


/**
 * @brief 定时器响应函数
 */
void DS_Threshold::timeoutDone()
{
   updateData();
}

/**
 * @brief 双击响应函数
 */
void DS_Threshold::updateWidget(QTableWidgetItem *it)
{
    bool ret = usr_land_jur();  //管理权限
    if(ret==false)
        return ;

    int coulumn = mTableWidget->currentColumn();
    if((coulumn==3) || (coulumn==4))
    {
        int row = mTableWidget->currentRow();
//        QString strMin = mTableWidget->item(row, 3)->text(); // 最小值
//        QString strMax = mTableWidget->item(row, 4)->text(); // 最大值

        int line = row / 2; // 获得相数
        int mode = SET_CMD_LINE_VOL;
        if(row % 2 == 1)
               mode = SET_CMD_LINE_CUR;

        if(mDataPacket)
        {
            SetThresholdDlg dlg(0);
            dlg.setMode(mode, line, mDataPacket);
            int ret = dlg.exec();
            if(ret == QDialog::Accepted)
                updateData();
        }

//        qDebug() << coulumn << row << strMin << strMax;
    }
    else
        it->setFlags(it->flags() & (~Qt::ItemIsEditable)); // 单元格不能编辑
}
