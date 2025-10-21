/*
 * ds_webbtn.cpp
 *  网络管理按钮界面
 *      点击按钮，打开浏览器，以网页的形式登录设备
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_webbtn.h"
#include <QDesktopServices>
#include "common/msgBox/msgbox.h"

extern bool usr_land_jur(void);

DS_WebBtn::DS_WebBtn(QWidget *parent) : QWidget(parent)
{
    setMaximumSize(110,80);
    QPalette pal;

    this->setStyleSheet("QPushButton{background-color:rgb(102, 199, 249);}"
                        "QPushButton:hover{background-color:rgb(91, 237, 238);}"
                        "QPushButton:pressed{background-color:rgb(143, 185, 181);}"
                    "QComboBox{background-color:rgb(255,255,255);}" );


    pal.setColor(QPalette::ButtonText,Qt::white);
    manageBtn = new QPushButton(tr("设备管理"),this);
    manageBtn->setFixedHeight(25);
//    manageBtn->setStyleSheet("background-color:rgb(88,162,130)");
    manageBtn->setPalette(pal);

    manageBtn->setFont(QFont("微软雅黑",10));
    logBtn = new QPushButton(tr("查看日志"));
    logBtn->setStyleSheet("background-color:rgb(200,200,200)");
    logBtn->setFixedHeight(25);
//    logBtn->setStyleSheet("background-color:rgb(88,162,130)");
    logBtn->setPalette(pal);
    logBtn->setFixedHeight(25);
    logBtn->setFont(QFont("微软雅黑",10));

    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(manageBtn);
    vLayout->addWidget(logBtn);

    //    manageBtn->setEnabled(false);
    logBtn->setEnabled(false);

    connect(manageBtn,SIGNAL(clicked()),this,SLOT(manageBtnSlot()));
    connect(logBtn,SIGNAL(clicked()),this,SLOT(logBtnSlot()));
}

DS_WebBtn::~DS_WebBtn()
{
}

/**
 * @brief 更新地址
 * @param ip
 */
void DS_WebBtn::updateIPAddr(QString &ip)
{
    m_ip = ip;
    //manageBtn->setEnabled(true);
    //logBtn->setEnabled(true);
}

/**
 * @brief 打开管理主页面
 */
void DS_WebBtn::manageBtnSlot()
{
    bool ret = usr_land_jur();
    if(ret==false){
        InfoMsgBox box(this,tr("您权限不够，无法操作"));
        return ;
    }

    if(!m_ip.isEmpty())
    {
        QString addr = "http://"+m_ip+"/";
        QDesktopServices::openUrl(QUrl(addr)); // 打开浏览器
    }
}

/**
 * @brief 打开日志页面
 */
void DS_WebBtn::logBtnSlot()
{
    //    bool ret = usr_land_jur();
    //    if(ret==false){
    //        InfoMsgBox box(this,tr("您权限不够，无法操作"));
    //        return ;
    //    }

    //    QString addr = "http://"+m_ip+"/log.shtml";


}
