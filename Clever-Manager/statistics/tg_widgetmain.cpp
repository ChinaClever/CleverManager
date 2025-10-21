/*
 * tg_widgetmain.cpp
 * 统计界面主类
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_widgetmain.h"
#include "ui_tg_widgetmain.h"

TG_WidgetMain::TG_WidgetMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TG_WidgetMain)
{
    ui->setupUi(this);
    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setContentsMargins(0, 0, 0, 0);

    mRoomWidget = new TG_RoomWidget(ui->widget);
    mStatistics = new TG_StatisticsWidget(ui->widget_2);
    connect(mRoomWidget, SIGNAL(roomSelectSIg(int)), mStatistics,SLOT(roomSelectSlot(int)));
    initLayout();
}

TG_WidgetMain::~TG_WidgetMain()
{
    delete ui;
}


/**
 * @brief 初始化布局
 */
void TG_WidgetMain::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(ui->widget);
    layout->addWidget(mRoomWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QGridLayout *gridLayout = new QGridLayout(ui->widget_2);
    gridLayout->addWidget(mStatistics);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
}
