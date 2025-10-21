/*
 * TG_StatisticsWidget.cpp
 * 统计界面堆栈窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_statisticswidget.h"

TG_StatisticsWidget::TG_StatisticsWidget(QWidget *parent) : QWidget(parent)
{
    mStack = new QStackedWidget(this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(mStack);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    initWidget();
    connect(DbRoomList::get(), SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
}

/**
 * @brief 创建新的窗口
 * @param id 机房键值
 */
void TG_StatisticsWidget::createWidget(int id)
{
    mTGShow = new TG_DataShow(mStack); // 统计信息界面    
    mTGShow->init(id);
    mStack->addWidget(mTGShow);

    sStackWidget *wid = new sStackWidget();
    wid->id = id;
    wid->widget = mTGShow;
    mList.append(wid);
}

/**
 * @brief 初始化窗口
 */
void TG_StatisticsWidget::initWidget(void)
{
    createWidget(0);
    QVector<RoomItem> items = DbRoomList::get()->allItems(); // 获取所有机房列表
    foreach(const RoomItem& item,items)
        createWidget(item.id);
}

/**
 * @brief 选择不同的机房
 * @param id
 */
void TG_StatisticsWidget::roomSelectSlot(int id)
{
    foreach (sStackWidget *wid, mList)
    {
        if(wid->id == id) {
            mStack->setCurrentWidget(wid->widget);
            break;
        }
    }
}

/**
 * @brief 根据机房键值查找索引号
 * @param id
 * @return
 */
int TG_StatisticsWidget::findByID(int id)
{
    sStackWidget *wid;
    for(int i=0; i<mList.size(); ++i)
    {
        wid = mList.at(i);
        if(wid->id == id)
            return i;
    }
    return -1;
}

/**
 * @brief 删除机房
 * @param id
 */
void TG_StatisticsWidget::delRoom(int id)
{
    int index = findByID(id);
    if(index > 0)
    {
        sStackWidget *wid = mList.at(index);
        mStack->removeWidget(wid->widget);

        delete wid->widget;
        mList.removeAt(index);
        delete wid;
    }
}



void TG_StatisticsWidget::itemChangedSlot(int id,int type)
{
    switch (type) {
    case Remove: // 删除机房
        delRoom(id);
        break;

    case Insert: // 增加机房
        createWidget(id);
        break;

    case Update: // 修改机房

        break;

    default:
        break;
    }
}

