/*
 * ls_mdevbtn.cpp
 * 设备列表按键操作
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mdevbtn.h"
LS_MDevBtn::LS_MDevBtn(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initBtn();

    groupBox_background_icon(this);
    button_style_sheet(this);
}

/**
 * @brief 初始化窗口
 */
void LS_MDevBtn::initWidget(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备管理"),this);

    addBtn = new QPushButton(groupBox);
    addBtn->setMinimumHeight(28);
    addBtn->setMaximumWidth(100);

    delBtn = new QPushButton(groupBox);
    delBtn->setMinimumHeight(28);
    delBtn->setMaximumWidth(100);

    moveBtn = new QPushButton(groupBox);
    moveBtn->setMinimumHeight(28);
    moveBtn->setMaximumWidth(100);

    reloadBtn = new QPushButton(groupBox);
    reloadBtn->setMinimumHeight(28);
    reloadBtn->setMaximumWidth(100);

    QGridLayout* gridLayout = new QGridLayout(groupBox);
    gridLayout->addWidget(addBtn, 0, 0);
    gridLayout->addWidget(delBtn, 0, 1);
    gridLayout->addWidget(moveBtn, 1, 0);
    gridLayout->addWidget(reloadBtn, 1, 1);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(groupBox);
}

/**
 * @brief Btn初始化
 */
void LS_MDevBtn::initBtn(void)
{
    addBtn->setText(tr("新增设备"));
    connect(addBtn,SIGNAL(clicked()),this,SIGNAL(addSig()));

    delBtn->setText(tr("删除设备"));
    connect(delBtn,SIGNAL(clicked()),this,SIGNAL(delSig()));

    moveBtn->setText(tr("移动设备"));
    connect(moveBtn,SIGNAL(clicked()),this,SIGNAL(moveSig()));

    reloadBtn->setText(tr("重新加载"));
    connect(reloadBtn,SIGNAL(clicked()),this,SIGNAL(reloadSig()));
}
