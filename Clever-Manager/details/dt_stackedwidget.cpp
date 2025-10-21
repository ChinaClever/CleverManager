/*
 * dt_stackedwidget.cpp
 * 设备列表界面的堆栈窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_stackedwidget.h"
#include "topologywidget.h"

DT_StackedWidget::DT_StackedWidget(QWidget *parent) : QWidget(parent)
{
    mStack = new QStackedWidget(this); //堆叠界面
    mStack->setStyleSheet("border:none");
    initLayout();
    initWidget();
    connect(DbRoomList::get(), SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
}

void DT_StackedWidget::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(this);
        QLabel *title=new QLabel(tr("    设备列表"),this);
        title->setMinimumHeight(40); //最小高度
        title->setStyleSheet("background-color:rgb(0,45,130); color: rgb(255,255,255);");
        QPalette pal; //调色板  -----------------------------------------------[风格统一]
        pal.setColor(QPalette::WindowText,Qt::white);
        title->setPalette(pal);
        title->setFont(QFont("微软雅黑",12));
        layout->addWidget(title);
    layout->addWidget(mStack);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

/**
 * @brief 创建新的窗口
 * @param id 机房键值
 */
void DT_StackedWidget::createWidget(int id, const QString &name)
{
    QString roomName = name;

    DT_DetailList *dtWid= new DT_DetailList(mStack); // 统计信息界面
    dtWid->initData(id, roomName); //很重要
    mStack->addWidget(dtWid);
//    mStack->addWidget(title);
    connect(dtWid,SIGNAL(selectSig(QString,short)),this,SIGNAL(selectSig(QString,short))); //双击信号

    sStackedWidget *wid = new sStackedWidget();
    wid->id = id;
    wid->roomName = name;
    wid->widget = dtWid;
    mList.append(wid); //信息列表
}

/**
 * @brief 初始化窗口
 */
void DT_StackedWidget::initWidget(void)
{
    //createWidget(0, tr("所有设备")); //先初始化所有设备
    QVector<RoomItem> items = DbRoomList::get()->allItems(); // 获取所有机房列表
    foreach(const RoomItem& item,items)
        createWidget(item.id, item.name); //逐一初始化其他房间
}

/**
 * @brief 选择不同的机房
 * @param id
 */
void DT_StackedWidget::roomSelectSlot(int id, QString)
{
    foreach (sStackedWidget *wid, mList)
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
int DT_StackedWidget::findByID(int id)
{
    sStackedWidget *wid;
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
void DT_StackedWidget::delRoom(int id)
{
    int index = findByID(id);
    if(index > 0)
    {
        sStackedWidget *wid = mList.at(index);
        mStack->removeWidget(wid->widget);

        delete wid->widget;
        mList.removeAt(index);
        delete wid;
    }
}

/**
 * @brief 根据机房键值，获取机房信息
 * @param id
 * @param it
 * @return
 */
bool  DT_StackedWidget::getRoomById(int id, RoomItem &it)
{
    bool ret = true;
    QVector<RoomItem> items =  DbRoomList::get()->findItemById(id); // 获取所有机房列表
    if(items.size() == 1)
        it = items.first();
    else
        ret = false;
    return ret;
}

/**
 * @brief 增加机房
 * @param id
 */
void DT_StackedWidget::addRoom(int id)
{
    RoomItem it;
    bool ret = getRoomById(id, it);
    if(ret)
        createWidget(id, it.name);
    else
        qDebug() << " add room err";
}

/**
 * @brief 机房变化 响应函数
 * @param id
 * @param type
 */
void DT_StackedWidget::itemChangedSlot(int id,int type)
{
    switch (type) {
    case Remove: // 删除机房
        delRoom(id);
        break;

    case Insert: // 增加机房
        addRoom(id);
        break;

    case Update: // 修改机房

        break;

    default:
        break;
    }
}

