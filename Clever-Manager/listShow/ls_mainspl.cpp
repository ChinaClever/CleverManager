/*
 * ls_mainspl.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mainspl.h"
#include "db_sqlquery.h"
#include "common.h"

LS_MainSpl::LS_MainSpl(QWidget *parent) : QWidget(parent)
{
    mainSplitter=new QSplitter(Qt::Horizontal,this);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(12,62,180)}");

    mainSplitter->setHandleWidth(10);
    initFunction();

    layout = new QGridLayout(this);
    layout->addWidget(mainSplitter);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

LS_MainSpl::~LS_MainSpl()
{

}


/**
 * @brief 初始化布局
 */
void LS_MainSpl::initLayout(void)
{
    leftSplitter->setStretchFactor(0,7);
    leftSplitter->setStretchFactor(1,3);

    mainSplitter->setStretchFactor(0,9);
    mainSplitter->setStretchFactor(1,14);
}


/**
 * @brief 左部分分割窗口
 */
void LS_MainSpl::initLeftSpl(void)
{
    leftSplitter=new QSplitter(Qt::Vertical,mainSplitter);
    leftSplitter->setHandleWidth(0);
    mListTab = new LS_listTabledlg(leftSplitter);

    mRefreshBtn = new LS_RefreshBtn(leftSplitter);
    connect(mRefreshBtn,SIGNAL(refreshSig()),mListTab,SLOT(updateWidget())); /*刷新信号*/
}

/**
 * @brief 列表树响应函数
 * @param ip 设备IP
 * @param num 设备号
 */
void LS_MainSpl::selectSlot(QString ip, short num)
{
    mRightSpl->selectSlot(ip, (int)num);
}

/**
 * @brief 右部分分割窗口
 */
void LS_MainSpl::initRightSpl(void)
{
    mRightSpl = new LS_RightSpl(mainSplitter);
    connect(mListTab,SIGNAL(selectedSig(QString &, int)),
            mRightSpl,SLOT(selectSlot(QString &, int))); /*刷新信号*/
}

/**
 * @brief 初始化
 */
void LS_MainSpl::initFunction(void)
{
    initLeftSpl();
    initRightSpl();
    initLayout();
}
