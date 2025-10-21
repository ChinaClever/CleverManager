/*
 * ls_mgrouplist.cpp
 * 设备组列表显示
 *  1、根据设备种类，显示相应的设备组
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mgrouplist.h"

LS_MgroupList::LS_MgroupList(QWidget *parent) : QWidget(parent)
{
    mGroupList = new PduDGList();
    initTableWidget();
}

LS_MgroupList::~LS_MgroupList()
{
    delete mGroupList;
}

/**
 * @brief 初始化表格
 */
void LS_MgroupList::initTableWidget(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("组列表"),this);
    mTableWidget = new QTableWidget(groupBox);

    QGridLayout *groupLayout = new QGridLayout(groupBox);
    groupLayout->addWidget(mTableWidget);

    layout = new QGridLayout(this);
    layout->addWidget(groupBox);

    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTableWidget->setSortingEnabled(true);

    connect(mTableWidget,  SIGNAL(itemClicked(QTableWidgetItem*)),
            this, SLOT(selectSlot(QTableWidgetItem *)));
}

/**
 * @brief 初始化窗口
 */
void LS_MgroupList::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("设备组");

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
void LS_MgroupList::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    //    item->setTextAlignment(Qt::AlignCenter);
    mTableWidget->setItem(row, column, item);
}


void LS_MgroupList::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
}

/**
 * @brief 更新窗口
 */
void LS_MgroupList::updateWidget(void)
{
    initWidget();

    QStringList groupList;
    mGroupList->listGroup(groupList);
    for(int i=0; i< groupList.size(); ++i)
    {
        QStringList listStr;
        listStr << groupList.at(i);
        addRowContent(listStr);
    }

    if(groupList.size() > 0)
    {
        mTableWidget->setCurrentCell(0,0);
        selectSlot(mTableWidget->currentItem());
    }
}

/**
 * @brief 更新窗口
 * @return
 */
void LS_MgroupList::updateWidget(int type)
{    
    mDevType = type;

    mGroupList->startListGroup(mDevType);
    updateWidget();
}


/**
 * @brief 点击响应函数
 * @param it
 */
void LS_MgroupList::selectSlot(QTableWidgetItem *it)
{
    QString str = it->text();
    if(str != mStr)
    {
        mStr = str;
        emit selectSig(mStr);
    }
}

/**
 * @brief 所选组名
 * @param group
 */
int LS_MgroupList::selectGroup(QString &group)
{
    int ret = -1;

    if(!mStr.isEmpty())
    {
        group = mStr;
        ret = mDevType;
    }

    return ret;
}
