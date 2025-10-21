/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "room_datawid.h"
#include "ui_room_datawid.h"

Room_DataWid::Room_DataWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room_DataWid)
{
    ui->setupUi(this);

    initView();
    mPacket = nullptr;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(5*1000);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Room_DataWid::~Room_DataWid()
{
    delete ui;
}


void Room_DataWid::initView()
{
    Room_TitleBar *bar = new Room_TitleBar(ui->titleWid);
    bar->setTitle(tr("设备状态信息"));
    bar->setFont(QFont("微软雅黑",12,75));
    bar->setFixedHeight(25);
    bar->setStyleSheet("background-color: rgb(0,17,55)");

    mVolLcd = new Room_LcdWid(ui->volWid);
    mVolLcd->setUnit(tr("电压"), "V");

    mCurLcd = new Room_LcdWid(ui->curWid);
    mCurLcd->setUnit(tr("电流"), "A");

    mPowLcd = new Room_LcdWid(ui->powWid);
    mPowLcd->setUnit(tr("功率"), "kW");

    mPfLcd = new Room_LcdWid(ui->pfWid);
    mPfLcd->setUnit(tr("功数"), "");
}


/**
 * @brief 电流值来调整显示
 * @param value
 */
void Room_DataWid::changeCurMode(long double value)
{
    int dec = 1;
    QString str = "A";

    if(value > 9000) {
        value /= 1000;
        str = "KA";
    } else if(value > 1000){
        dec = 0;
    }

    mCurLcd->setSym(str);
    mCurLcd->display((double)value, dec);
}

/**
 * @brief 功率显示
 * @param value
 */
void Room_DataWid::changePowMode(long double value)
{
    int dec = 0;
    QString str = "kW";

    if(value < 10)
        dec = 3;
    else if(value < 100)
        dec = 2;
    else if(value < 1000)
        dec = 1;
    else if(value < 9000)
        dec = 0;
    else {
        value /= 1000;
        dec = 3;
        str = "MW";
    }

    mPowLcd->setSym(str);
    mPowLcd->display((double)value, dec);
}


/**
 * @brief 刷新数据
 */
void Room_DataWid::updateData(sTgObjData &data)
{
    long double value;

    value = data.vol/COM_RATE_VOL;
    mVolLcd->display((double)value);

    value = data.cur/COM_RATE_CUR;
    changeCurMode(value);

    value = data.pow/COM_RATE_POW;
    changePowMode(value);

    value = data.pf/COM_RATE_PF;
    mPfLcd->display((double)value, 2);
}


bool Room_DataWid::checkPack()
{
    bool ret = false;

    if(mPacket) {
        if(mPacket->en)
            ret = true;
    }

    return ret;
}

void Room_DataWid::timeoutDone()
{
    bool ret = checkPack();
    if(ret) {
        updateData(mPacket->tg);
    } else {
        sTgObjData data;
        memset(&data, 0, sizeof(sTgObjData));
        updateData(data);
    }
}
