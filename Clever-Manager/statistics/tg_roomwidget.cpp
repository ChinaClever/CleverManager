/*
 * tg_roomwidget.cpp
 * 统计界面机房列表处理
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_roomwidget.h"
#include "ui_tg_roomwidget.h"
#include "common.h"
#include"tg_datathread.h"
#include<fstream>

//QString comtext;

TG_RoomWidget::TG_RoomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TG_RoomWidget)
{
    ui->setupUi(this);

    initData();
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this,SLOT(currentItemChangedSlot(int)));
    connect(DbRoomList::get(), SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));

    QTimer::singleShot(1000,this,SLOT(initFunSLot())); //延时初始化
}

TG_RoomWidget::~TG_RoomWidget()
{
    delete ui;
}


void TG_RoomWidget::initFunSLot()
{
    this->setStyleSheet("background-color: rgb(0,45,130);color: rgb(0, 0, 0);");
    set_background_color(this, QColor(0,45,130));

    //为了天津腾讯机房交直流添加 默认显示第一个机房2018-8-25
    ui->comboBox->setCurrentIndex(1);
}

/**
 * @brief 插入数据
 * @param items
 */
void TG_RoomWidget::insertRoomItem(void)
{
    RoomItem it;
    it.name = tr("所有设备");
    it.id = 0;
    createRoomItem(it);
}

/**
 * @brief 机房Item
 * @param rItem
 * @return
 */
void TG_RoomWidget::createRoomItem(const RoomItemExt &rItem)
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/topology/topology/1.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::On);
    ui->comboBox->addItem(icon, rItem.name,QVariant::fromValue(rItem));
}


/**
 * @brief 加载数据库中的机房列表。
 */
void TG_RoomWidget::initData(void)
{

    ui->comboBox->clear();
    insertRoomItem();
    QVector<RoomItem> items = DbRoomList::get()->allItems(); // 获取所有机房列表
    foreach(const RoomItem& item,items)
        createRoomItem(item);
}


/**
 * @brief 选项不同的机房
 * @param current
 */
void TG_RoomWidget::currentItemChangedSlot(int)
{
    RoomItemExt rItem = ui->comboBox->currentData().value<RoomItemExt>();
    emit roomSelectSIg(rItem.id);

    QString str;
    if(rItem.id > 0)
        str = tr("机房:");
    ui->roomLabel->setText(str + rItem.name);
}

/**
 * @brief 根据机房ID键值查找索引
 * @param id 机房ID键值
 * @return
 */
int TG_RoomWidget::findById(int id)
{
    int index = -1;
    int size = ui->comboBox->count();
    for(int i=0; i<size; ++i){
        RoomItemExt rItem = ui->comboBox->itemData(i).value<RoomItemExt>();
        if(rItem.id == id){
            index = i;
            break;
        }
    }

    return index;
}

/**
 * @brief 删除一个机房
 * @param id
 */
void TG_RoomWidget::delRoom(int id)
{
    int index = findById(id);
    if(index > 0)
        ui->comboBox->removeItem(index);
}

/**
 * @brief 根据机房键值，获取机房信息
 * @param id
 * @param it
 * @return
 */
bool  TG_RoomWidget::getRoomById(int id, RoomItem &it)
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
void TG_RoomWidget::addRoom(int id)
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
void TG_RoomWidget::upRoom(int id)
{
    int index = findById(id);
    if(index > 0)
    {
        RoomItem it;
        bool ret = getRoomById(id, it);
        if(ret){
            //            ui->comboBox->setItemData(index,QVariant::fromValue(it));
            ui->comboBox->setItemText(index, it.name);
            RoomItemExt rItem = ui->comboBox->itemData(index).value<RoomItemExt>();
        }
    }
}

/**
 * @brief 当机房发生改变时 响应函数
 * @param id
 * @param type
 */
void TG_RoomWidget::itemChangedSlot(int id,int type)
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

/**
 * @brief 上一机房
 */
void TG_RoomWidget::on_upBtn_clicked()
{
    int index = ui->comboBox->currentIndex()-1;
    if(index < 0)
        index = ui->comboBox->count() -1;
    ui->comboBox->setCurrentIndex(index);
}

/**
 * @brief 下一机房
 */
void TG_RoomWidget::on_downBtn_clicked()
{
    int index = ui->comboBox->currentIndex()+1;
    if(index >= ui->comboBox->count())
        index = 0;
    ui->comboBox->setCurrentIndex(index);
}


