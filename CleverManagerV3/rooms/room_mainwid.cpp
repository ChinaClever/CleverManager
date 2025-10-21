/*
 * tg_roomwidget.cpp
 * 统计界面机房列表处理
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "room_mainwid.h"
#include "ui_room_mainwid.h"
#include "dp_roompackets.h"

Room_MainWid::Room_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room_MainWid)
{
    ui->setupUi(this);
    QTimer::singleShot(5*1000,this,SLOT(initFunSLot())); //延时初始化
    this->setStyleSheet("background-color: rgb(0,45,130);color: rgb(0, 0, 0);");
    set_background_color(this, QColor(0,45,130));
    initWid();
}

Room_MainWid::~Room_MainWid()
{
    delete ui;
}


void Room_MainWid::initFunSLot()
{
    ui->comboBox->clear();
    QStringList rooms = DbRoomList::get()->list();
    ui->comboBox->addItems(rooms);
    if(rooms.size()) ui->comboBox->setCurrentIndex(0);
}


void Room_MainWid::initWid()
{
    mBar = new Room_BarGraph(ui->barWid);
    connect(this, SIGNAL(roomSelectSig(sRoomPacket*)), mBar, SLOT(packetSlot(sRoomPacket*)));

    mData = new Room_DataWid(ui->dataWid);
    connect(this, SIGNAL(roomSelectSig(sRoomPacket*)), mData, SLOT(packetSlot(sRoomPacket*)));

    mPow = new Room_PowerGraph(ui->powWid);
    connect(this, SIGNAL(roomSelectSig(sRoomPacket*)), mPow, SLOT(packetSlot(sRoomPacket*)));

    mVc = new Room_VcGraph(ui->vcWid);
    connect(this, SIGNAL(roomSelectSig(sRoomPacket*)), mVc, SLOT(packetSlot(sRoomPacket*)));

    connect(DbRoomList::get(), SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
}

/**
 * @brief 当机房发生改变时 响应函数
 * @param id
 * @param type
 */
void Room_MainWid::itemChangedSlot(int,int)
{
    initFunSLot();
}

/**
 * @brief 上一机房
 */
void Room_MainWid::on_upBtn_clicked()
{
    int index = ui->comboBox->currentIndex()-1;
    if(index < 0) index = ui->comboBox->count() -1;
    ui->comboBox->setCurrentIndex(index);
}

/**
 * @brief 下一机房
 */
void Room_MainWid::on_downBtn_clicked()
{
    int index = ui->comboBox->currentIndex()+1;
    if(index >= ui->comboBox->count())
        index = 0;
    ui->comboBox->setCurrentIndex(index);
}



void Room_MainWid::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1.isEmpty()) return;
    ui->roomLabel->setText(tr("机房:") + arg1);
    sRoomPacket *p = Dp_RoomPackets::bulid(this)->get(arg1);
    if(p)  emit roomSelectSig(p);
}
