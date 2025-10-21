/*
 * ls_refreshbtn.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_refreshbtn.h"
#include "devmanage/ls_managedevlistdlg.h"
#include "common/msgBox/msgbox.h"
#include <QGroupBox>

LS_RefreshBtn::LS_RefreshBtn(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initBtn();
}

LS_RefreshBtn::~LS_RefreshBtn()
{

}

void LS_RefreshBtn::initWidget(void)
{
    QGroupBox *groupBox = new QGroupBox(this);  //刷新设备按钮
    groupBox->setStyleSheet("border:none");  //设置无边缘线
    set_background_color(groupBox, QColor(0,0,0));

    refreshBtn = new QPushButton(groupBox);
    btnBlue_style_sheet(refreshBtn);
    refreshBtn->setMinimumHeight(28);

    refreshBtn->setMaximumWidth(150);
    //    connect(refreshBtn,SIGNAL(refreshSig()),mListTab,SLOT(updateWidget())); /*刷新信号*/


    //    manageBtn = new QPushButton(groupBox);
    //    manageBtn->setMinimumHeight(28);
    //    manageBtn->setMaximumWidth(100);

    QHBoxLayout* hLzyout = new QHBoxLayout(groupBox);
    hLzyout->addWidget(refreshBtn);
    //    hLzyout->addWidget(manageBtn);
    hLzyout->setSpacing(0);
    hLzyout->setContentsMargins(7, 0, 0, 0);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(groupBox);

}

/**
 * @brief Btn初始化
 */
void LS_RefreshBtn::initBtn(void)
{
    refreshBtn->setText(tr("刷新设备"));
    refreshBtn->setFixedHeight(30);
    refreshBtn->setFont(QFont("微软雅黑",12));

    connect(refreshBtn,SIGNAL(clicked()),this,SIGNAL(refreshSig()));

    //    manageBtn->setText(tr("管理设备"));
    //    connect(manageBtn,SIGNAL(clicked()),this,SLOT(manageBtnSlot()));
}

/**
 * @brief 设备管理
 */
void LS_RefreshBtn::manageBtnSlot(void)
{
    //    extern bool usr_land_jur(void);

    //    bool ret = usr_land_jur();
    //    if(ret){
    //        LS_ManageDevListDlg manageDev;
    //        manageDev.exec();

    //        emit refreshSig();
    //    }
    //    else
    //        InfoMsgBox box(this,tr("您权限不够，无法管理设备"));
}
