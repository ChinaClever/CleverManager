/*
 * ls_mtypebtn.cpp
 *  设备类 管理界面  按键显示
 *      1、发出相应的操作信号
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mtypebtn.h"

LS_MTypeBtn::LS_MTypeBtn(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initBtn();

    groupBox_background_icon(this);
    button_style_sheet(this);
}


void LS_MTypeBtn::initWidget(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备种类"),this);

    refreshBtn = new QPushButton(groupBox);
    refreshBtn->setMinimumHeight(28);
    refreshBtn->setMinimumWidth(100);

    delBtn = new QPushButton(groupBox);
    delBtn->setMinimumHeight(28);
    delBtn->setMinimumWidth(100);

    QGridLayout* gridLayout = new QGridLayout(groupBox);
    gridLayout->addWidget(refreshBtn, 0, 0);
    gridLayout->addWidget(delBtn, 0, 1);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(groupBox);
}

/**
 * @brief Btn初始化
 */
void LS_MTypeBtn::initBtn(void)
{
    refreshBtn->setText(tr("刷新设备"));
    connect(refreshBtn,SIGNAL(clicked()),this,SIGNAL(refreshSIg()));

    delBtn->setText(tr("删除种类"));
    connect(delBtn,SIGNAL(clicked()),this,SIGNAL(delSig()));
}
