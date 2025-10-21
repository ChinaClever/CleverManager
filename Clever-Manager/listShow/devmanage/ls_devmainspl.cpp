/*
 * ls_devmainspl.cpp
 * 设备管理界面实现QSplitter效果
 *  1、设备类型管理
 *  2、设备组管理
 *  3、设备链表管理
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_devmainspl.h"
#include "common.h"

LS_DevMainSpl::LS_DevMainSpl(QWidget *parent) : QWidget(parent)
{
    splitter=new QSplitter(Qt::Horizontal,this);
    initSpl();

    layout = new QGridLayout(this);
    layout->addWidget(splitter);

    QTimer::singleShot(5,this,SLOT(initFunSLot())); //延时初始化
}

void LS_DevMainSpl::initFunSLot(void)
{
}

/**
 * @brief 初始化布局
 */
void LS_DevMainSpl::initLayout(void)
{
    splitter->setStretchFactor(0,2);
    splitter->setStretchFactor(1,2);
    splitter->setStretchFactor(2,6);
}



/**
 * @brief 左部分分割窗口
 */
void LS_DevMainSpl::initSpl(void)
{
    mType = new LS_MTypeWidget(splitter); // 初始化设备种类界面

    m_mgroupList = new ls_MGroupWidget(splitter); // 初始化分组界面
    connect(mType,SIGNAL(selectSig(int)),m_mgroupList,SLOT(updateSlot(int)));

    m_mdevTable = new ls_MDevWidget(splitter); // 初始化设备列表界面
    m_mdevTable->mTable->mGroupList = m_mgroupList->mTable->mGroupList; // 共用一个对象
    connect(m_mgroupList,SIGNAL(selectSig(QString &)),m_mdevTable,SLOT(updateSlot(QString &)));
}
