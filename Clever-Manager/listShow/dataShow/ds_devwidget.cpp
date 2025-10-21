/*
 * ds_devwidget.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_devwidget.h"
#include "common.h"

DS_DevWidget::DS_DevWidget(QWidget *parent) : QWidget(parent)
{
    initSplitter();
    initLayout();

    QColor color(0,17,55);
    set_background_color(this,color);
    //    this->setStyleSheet("background-color:rgb(0,0,30)");

}


/**
 * @brief 初始化布局
 */
void DS_DevWidget::initLayout(void)
{
    layout = new QGridLayout(this);
    layout->addWidget(splitter);
    layout->setSpacing(10);
    layout->setContentsMargins(0, 0, 0, 0);

    splitter->setStretchFactor(0,38);
    splitter->setStretchFactor(1,62);
}

void DS_DevWidget::initSplitter(void)
{
    splitter=new QSplitter(Qt::Vertical,this);  //上下
    splitter->setHandleWidth(0);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    splitter->setMaximumHeight(desktopWidget->height()-237-10);
    m_dataShow = new DS_DataShowWid(splitter);
    //    m_devInfo = new DS_DevInfoTable(splitter);  //设备信息即阈值相关

    m_graph = new DS_DataGraph(splitter);
    connect(m_dataShow,SIGNAL(selectDevSig(DS_DsyDev &)),m_graph,SLOT(updateGraph(DS_DsyDev &)));
}


/**
 * @brief
 * @param ip 设备IP
 * @param num 设备号
 */
void DS_DevWidget::selectDev(QString &ip, int num)
{
    m_dataShow->updateData(ip, num);
    //    m_devInfo->updateWidget(ip, num);
}
