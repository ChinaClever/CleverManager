/*
 * ls_mtypewidget.cpp
 * 设备类型管理界面
 *  1、设备种类删除操作
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mtypewidget.h"
#include "common.h"

LS_MTypeWidget::LS_MTypeWidget(QWidget *parent) : QWidget(parent)
{
    splitter=new QSplitter(Qt::Vertical,this);
    initWidget();

    layout = new QGridLayout(this);
    layout->addWidget(splitter);

    groupBox_background_icon(this);
}

/**
 * @brief 初始化table
 */
void LS_MTypeWidget::initTable(void)
{
    mTable = new LS_MTypeTable(splitter);
    connect(mTable,SIGNAL(selectSig(int)),this,SIGNAL(selectSig(int)));
}

/**
 * @brief 按键初始化
 */
void LS_MTypeWidget::initBtn(void)
{
    mBtn = new LS_MTypeBtn(splitter);
    connect(mBtn,SIGNAL(delSig()),this,SLOT(delSlot()));
    connect(mBtn,SIGNAL(refreshSIg()),mTable,SLOT(initFunSLot()));
}

void LS_MTypeWidget::initWidget(void)
{
    initTable();
    initBtn();

    splitter->setStretchFactor(0,6);
    splitter->setStretchFactor(1,4);
}

/**
 * @brief 删除设备种类
 */
void LS_MTypeWidget::delSlot()
{
    QString str;
    int num = mTable->getDevType(str);
    /////=====
}


