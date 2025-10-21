/*
 * ds_devinfotable.cpp
 *  设备信息表格主界面
 *      1、创建统计信息表格界面
 *      2、创建祥细设备数据表格界面
 *      3、创建设备阈值表格界面
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_devinfotable.h"
#include <QGroupBox>
#include <QGridLayout>
#include<qlabel.h>

DS_DevInfoTable::DS_DevInfoTable(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initLayout();
}

DS_DevInfoTable::~DS_DevInfoTable()
{

}

/**
 * @brief 更新
 * @param ip
 * @param num
 */
void DS_DevInfoTable::updateWidget(QString &ip, int num)
{
    m_devState->updateWidget(ip,num);
    m_devData->updateWidget(ip,num);

    m_threshold->updateWidget(ip,num);
//    m_devInfo->updateWidget(ip,num);
}

/**
 * @brief 布局
 */
void DS_DevInfoTable::initLayout(void)
{
    QGroupBox *groupBox = new QGroupBox(this);//tr("设备信息"),
    groupBox->setStyleSheet("border:none");
 groupBox->setStyleSheet("background-color:white");


    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(tabWidget);
    little_widget=new QWidget(this);
   little_widget->setStyleSheet("background-color:rgb(0,0,0)");
    little_layout=new QVBoxLayout(little_widget);
    little_layout->addWidget(groupBox);
    little_layout->setContentsMargins(15,10,15,10);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->setContentsMargins(0,0,0,0);
    QLabel *title=new QLabel(tr("  设备信息"),this);
    title->setStyleSheet("background-color:black");
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    title->setPalette(pal);
    title->setFont(QFont("微软雅黑",12));
    title->setFixedHeight(40);
    pLayout->addWidget(title);
    pLayout->addWidget(little_widget);
}


void DS_DevInfoTable::initWidget(void)
{
    tabWidget = new QTabWidget(this);
    tabWidget->setFont(QFont("微软雅黑",12));

    m_devState = new DS_DevState(tabWidget);
    tabWidget->addTab(m_devState, tr("设备状态"));

    m_devData = new DS_DevData(tabWidget);
//    m_devData->setFont(QFont("微软雅黑",12));
    tabWidget->addTab(m_devData, tr("设备数据"));

    m_threshold = new DS_Threshold(tabWidget);
    tabWidget->addTab(m_threshold, tr("设备阈值"));

//    m_devInfo = new DS_DevInfo(tabWidget);
//    tabWidget->addTab(m_devInfo, tr("设备信息"));

}
