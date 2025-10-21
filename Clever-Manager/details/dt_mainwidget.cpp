/*
 * dt_mainwidget.cpp
 * 设备列表的主窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_mainwidget.h"

DT_MainWidget::DT_MainWidget(QWidget *parent) : QWidget(parent)
{
    mSplitter = new QSplitter(Qt::Horizontal,this); //分束器
    mSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(12,62,180)}");
    mSplitter->setHandleWidth(10); //宽度

    mRommList = new DT_RommListWidget(mSplitter); //机房切换界面
    mStackedWidget = new DT_StackedWidget(mSplitter); //详细信息界面

    connect(mRommList, SIGNAL(roomSelectSig(int,QString)),mStackedWidget,SLOT(roomSelectSlot(int,QString))); //房间选择[单机触发]
    connect(mStackedWidget,SIGNAL(selectSig(QString,short)),this,SIGNAL(selectSig(QString,short))); //选择[双击触发]

    initLayout();
}

void DT_MainWidget::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(mSplitter);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0,0); //左 上 右 下

    mSplitter->setStretchFactor(0,3);
    mSplitter->setStretchFactor(1,11);
}
