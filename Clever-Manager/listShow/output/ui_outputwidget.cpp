/*
 * ui_outputtable.cpp
 * 设备输出位显示主界面
 *  1、实现输出位祥细列表显示
 *  2、输出位分组列表显示
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ui_outputwidget.h"
#include "common.h"
#include<qlabel.h>
#include<qpalette.h>

UI_OutputWidget::UI_OutputWidget(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initLayout();
}


/**
 * @brief 布局
 */
void UI_OutputWidget::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(mGroupBox);

    layout->addWidget(tabWidget);

    mGroupBox->setStyleSheet("border:none");
    little_widget=new QWidget(this);

    set_background_color(little_widget, QColor(0,17,55));
    little_layout=new QVBoxLayout(little_widget);
    little_layout->addWidget(mGroupBox);
    little_layout->setContentsMargins(8,0,15,0);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->setContentsMargins(0,0,0,0);
    QLabel *label_title=new QLabel(tr(" "),this);

    label_title->setStyleSheet("background-color:black");
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    label_title->setPalette(pal);
    label_title->setFont(QFont("微软雅黑",12));
    label_title->setFixedHeight(0);

    pLayout->addWidget(label_title);
    pLayout->addWidget(little_widget);
}


/**
 * @brief 初始化窗口
 */
void UI_OutputWidget::initWidget(void)
{    
    mGroupBox = new QGroupBox(this);//tr("输出位状态--------"),
    mGroupBox->setStyleSheet("border:none");

    tabWidget = new QTabWidget(mGroupBox);
    //    tabWidget->setTabPosition(QTabWidget::South);

    mOutputTable = new UI_OutputTable(tabWidget);
    tabWidget->addTab(mOutputTable, tr("输出位状态"));
    tabWidget->setStyleSheet("QTabBar::tab{height:0}"); // 隐藏标签
}


/**
 * @brief 更新
 * @param ip 设备ip
 * @param num 设备号
 */
void UI_OutputWidget::updateWidget(QString &ip, int num)
{
    mOutputTable->updateWidget(ip,num);
}
