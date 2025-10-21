/*
 * dt_rommlistwidget.cpp
 * 设备列表界面的机房列表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_rommlistwidget.h"
#include "ui_dt_rommlistwidget.h"

DT_RommListWidget::DT_RommListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DT_RommListWidget)
{
    ui->setupUi(this);

    ui->listWidget_roomList->setStyleSheet("QListWidget{border:0px solid gray; color:black; }"     //设置边框
                                           "QListWidget::item:selected{background:rgb(12,62,180); color:white; }"  //选中后的背景色
                                           "QListWidget{font:12pt;}"
                                           );
    // insertItem();
    initData();
    connect(ui->listWidget_roomList,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this,SLOT(currentItemChangedSlot(QListWidgetItem*,QListWidgetItem*)));

    connect(DbRoomList::get(), SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
}

DT_RommListWidget::~DT_RommListWidget()
{
    delete ui;
}

void DT_RommListWidget::insertItem(void)
{
    RoomItem item;
    item.id = 0;
    item.name = tr("所有设备");
    createRoomItem(item);
}

/**
 * @brief 创建机房对象
 * @param rItem
 * @return
 */
QListWidgetItem *DT_RommListWidget::createRoomItem(const RoomItemExt &rItem)
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/topology/topology/1.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::On);
    QListWidgetItem* item = new QListWidgetItem(icon,rItem.name,nullptr,rItem.id);
    item->setData(Qt::UserRole,QVariant::fromValue(rItem)); //存放数据
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    item->setSizeHint(QSize(150,35));
    ui->listWidget_roomList->addItem(item); //add
    return item;
}

/**
 * @brief 加载数据库中的机房列表、及指定机柜地图的数据信息。
 */
void DT_RommListWidget::initData(void)
{
    //1.加载数据
    QVector<RoomItem> items = DbRoomList::get()->allItems(); // 获取所有机房列表
    foreach(const RoomItem& item,items)  //遍历
        createRoomItem(RoomItemExt(item));

    if(items.size())
        ui->listWidget_roomList->setCurrentRow(0);
}


/**
 * @brief 选择不同的机房
 * @param current
 */
void DT_RommListWidget::currentItemChangedSlot(QListWidgetItem *current,QListWidgetItem*)
{
    if(current){
        RoomItemExt rItem = current->data(Qt::UserRole).value<RoomItemExt>(); //取出数据
        emit roomSelectSig(rItem.id, rItem.name); //发送信号
    }
}

/**
 * @brief 根据机房ID键值查找索引
 * @param id 机房ID键值
 * @return
 */
QListWidgetItem *DT_RommListWidget::findById(int id)
{
    int size = ui->listWidget_roomList->count();  //项数
    for(int i=0; i<size; ++i){
        QListWidgetItem *current = ui->listWidget_roomList->item(i);
        if(current){
            RoomItemExt rItem = current->data(Qt::UserRole).value<RoomItemExt>();
            if(rItem.id == id){
                return current;
            }
        }
    }

    return NULL;
}


/**
 * @brief 删除一个机房
 * @param id
 */
void DT_RommListWidget::delRoom(int id)
{
    QListWidgetItem *item = findById(id);
    if(item){
        ui->listWidget_roomList->removeItemWidget(item);
        delete item;
    }
}


/**
 * @brief 根据机房键值，获取机房信息
 * @param id
 * @param it
 * @return
 */
bool  DT_RommListWidget::getRoomById(int id, RoomItem &it)
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
void DT_RommListWidget::addRoom(int id)
{
    RoomItem it;
    bool ret = getRoomById(id, it);
    if(ret)
        createRoomItem(it);
    else
        qDebug() << " add room err";
}


/**
 * @brief 机房名修改
 * @param id
 */
void DT_RommListWidget::upRoom(int id)
{
    QListWidgetItem *item = findById(id);
    if(item){
        RoomItem it;
        bool ret = getRoomById(id, it);
        if(ret){
            //            item->setData(Qt::UserRole,QVariant::fromValue(it));
            item->setText(it.name);
        }
    }
}


/**
 * @brief 当机房发生改变时 响应函数
 * @param id
 * @param type
 */
void DT_RommListWidget::itemChangedSlot(int id,int type)
{
    switch (type) {
    case Remove: // 删除机房
        delRoom(id);
        break;

    case Insert: // 增加机房
        addRoom(id);
        break;

    case Update: // 修改机房
        upRoom(id);
        break;

    default:
        break;
    }
}
