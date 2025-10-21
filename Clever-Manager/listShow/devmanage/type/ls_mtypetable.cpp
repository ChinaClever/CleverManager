/*
 * ls_mtypetable.cpp
 *  设备类 管理界面
 *      1、以列表的形式、显示所有设备种类名
 *      2、第一次显示列表、刷新列表，都会自动发送，选择信号  void initFunSLot();
 *      3、当点击不同的设备种类时，发会出选择信号 void selectSig(int);
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mtypetable.h"

LS_MTypeTable::LS_MTypeTable(QWidget *parent) : QWidget(parent)
{
    mTypeNum = -1;
    initTableWidget();

    mType = new PduDTName();
    QTimer::singleShot(50,this,SLOT(initFunSLot())); //延时初始化
}

LS_MTypeTable::~LS_MTypeTable()
{
    delete mType;
}

/**
 * @brief table初始化
 */
void LS_MTypeTable::initTableWidget(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备类型"),this);
    mTableWidget = new QTableWidget(groupBox);

    QGridLayout *groupLayout = new QGridLayout(groupBox);
    groupLayout->addWidget(mTableWidget);
    layout = new QGridLayout(this);
    layout->addWidget(groupBox);

    connect(mTableWidget,  SIGNAL(itemClicked(QTableWidgetItem*)),
            this, SLOT(selectSlot(QTableWidgetItem *)));

    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    mTableWidget->setSortingEnabled(true);

}

/**
 * @brief 自动更新列表，并发出信号
 */
void LS_MTypeTable::initFunSLot(void)
{
    mTypeNum = -1;
    updateWidget();
}

/**
 * @brief 初始化窗口
 */
void LS_MTypeTable::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("设备种类");

    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->setHorizontalHeaderLabels(header);
    mTableWidget->horizontalHeader()->setStretchLastSection(true);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void LS_MTypeTable::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    //    item->setTextAlignment(Qt::AlignCenter);
    mTableWidget->setItem(row, column, item);
}


void LS_MTypeTable::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
}

/**
 * @brief 更新窗口
 * @return  第一个种类的设备号
 */
void LS_MTypeTable::updateWidget(void)
{
    QList<int> list;
    mType->list(list);

    QString str;
    initWidget();
    for(int i=0; i<list.size(); ++i)
    {
        int type = list.at(i);
        mType->getName(type, str);

        QStringList listStr;
        listStr << str;
        addRowContent(listStr);
    }

    if(list.size() > 0)
    {
        mTableWidget->setCurrentCell(0,0); // 选择第一个种类
        selectSlot(mTableWidget->currentItem());
    }
}

/**
 * @brief 获取设备类型代号
 * @return
 */
int LS_MTypeTable::getNum(void)
{
    return mTypeNum;
}

int LS_MTypeTable::getDevType(QString &str)
{
    str = mTypeName;

    return getNum();
}

/**
 * @brief 单击响应函数
 * @param it
 */
void LS_MTypeTable::selectSlot(QTableWidgetItem *it)
{
    QString str = it->text();
    int num = mType->getNum(str);
    if(num > 0)
    {
        if(mTypeNum != num)
        {
            mTypeName = str;
            mTypeNum = num;
            emit selectSig(num);
        }
    }
}
