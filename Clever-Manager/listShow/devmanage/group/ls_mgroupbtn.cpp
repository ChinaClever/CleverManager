/*
 * ls_mgroupbtn.cpp
 * 设备组操作按键
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mgroupbtn.h"

LS_MGroupBtn::LS_MGroupBtn(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initBtn();

    groupBox_background_icon(this);
    button_style_sheet(this);
}

/**
 * @brief 控件初始化
 */
void LS_MGroupBtn::initWidget(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备管理"),this);

    addBtn = new QPushButton(groupBox);
    addBtn->setMinimumHeight(28);
    addBtn->setMaximumWidth(100);

    delBtn = new QPushButton(groupBox);
    delBtn->setMinimumHeight(28);
    delBtn->setMaximumWidth(100);

    editBtn = new QPushButton(groupBox);
    editBtn->setMinimumHeight(28);
    editBtn->setMaximumWidth(100);

    moveBtn = new QPushButton(groupBox);
    moveBtn->setMinimumHeight(28);
    moveBtn->setMaximumWidth(100);

    QGridLayout* gridLayout = new QGridLayout(groupBox);
    gridLayout->addWidget(addBtn, 0, 0);
    gridLayout->addWidget(delBtn, 0, 1);
    gridLayout->addWidget(editBtn, 1, 0);
    gridLayout->addWidget(moveBtn, 1, 1);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(groupBox);
}

/**
 * @brief Btn初始化
 */
void LS_MGroupBtn::initBtn(void)
{
    addBtn->setText(tr("新建分组"));
    connect(addBtn,SIGNAL(clicked()),this,SIGNAL(addSig()));

    delBtn->setText(tr("删除分组"));
    connect(delBtn,SIGNAL(clicked()),this,SIGNAL(delSig()));

    editBtn->setText(tr("编辑分组"));
    connect(editBtn,SIGNAL(clicked()),this,SIGNAL(editSig()));

    moveBtn->setText(tr("移动分组"));
    connect(moveBtn,SIGNAL(clicked()),this,SIGNAL(moveSig()));
}
