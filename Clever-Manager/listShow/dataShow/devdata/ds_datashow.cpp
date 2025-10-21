/*
 * ds_datashow.cpp
 * 设备数据显示界面
 *  1、创建数据显示界面
 *  2、创建副机显示界面
 *  3、创建相数显示界面
 *  4、创建网页按键界面
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_datashow.h"
#include "common.h"

DS_DataShow::DS_DataShow(QWidget *parent) : QWidget(parent)
{
    initFunction();

    /*初始化数据*/
    m_dsyDeV.line = 0;
    m_dsyDeV.num = 0;

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    //    timer->start(1000);
}

DS_DataShow::~DS_DataShow()
{
}

/**
 * @brief 初始化入口函数
 */
void DS_DataShow::initFunction(void)
{
    createWidget();
    initLayout();
    initClass();
}

/**
 * @brief 初始化类
 */
void DS_DataShow::initClass(void)
{
    m_dataDsy = new DS_DataDsplay(mainWidget);

    m_lineBtn = new DS_LineBtn(lineWidget);
    connect(m_lineBtn,SIGNAL(btnClickedSig(int)),this,SLOT(lineBtnSlot(int)));

    //    m_slaveBtn = new DS_SlaveBtn(slaveWidget);
    //    connect(m_slaveBtn,SIGNAL(btnClickedSig(int)),this,SLOT(slaveBtnSlot(int)));

    m_webBtn = new DS_WebBtn(webWidget);

    createLayout(); // 给子窗口布局
}

/**
  * @brief 子窗口布局
  */
void DS_DataShow::createLayout(void)
{
    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    mainLayout->addWidget(m_dataDsy);

    //      mainLayout->setContentsMargins(10,10,10,10);


    //    QGridLayout *slaveLayout = new QGridLayout(slaveWidget);
    //    slaveLayout->addWidget(m_slaveBtn);

    QGridLayout *lineLayout = new QGridLayout(lineWidget);

    lineLayout->addWidget(m_lineBtn);

    QGridLayout *webBtnLayout = new QGridLayout(webWidget);
    webBtnLayout->addWidget(m_webBtn);

}

/**
 * @brief 布局
 */
void DS_DataShow::initLayout(void)
{
    QVBoxLayout *rightLayout = new QVBoxLayout;  //主界面

    rightLayout->addWidget(lineWidget);
    rightLayout->addWidget(webWidget);

    QGridLayout *mainLayout = new QGridLayout(groupBox);  //上半部分主界面

    mainLayout->setContentsMargins(10,10,10,10);

    mainLayout->addWidget(mainWidget,0,0);
    mainLayout->addLayout(rightLayout,0,1);


    QGridLayout *layout = new QGridLayout(this);//上左
    layout->setContentsMargins(0,0,0,0);
    QLabel *title=new QLabel(tr("  设备状态信息"));
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    title->setPalette(pal);
    title->setStyleSheet("background-color:black");
    title->setFixedHeight(40);
    title->setFont(QFont("微软雅黑",12));
    layout->addWidget(title);
    //    layout->addWidget(groupBox);
    layout->addWidget(my_widget);
}

/**
 * @brief 窗口创建
 */
void DS_DataShow::createWidget(void)
{
    groupBox = new QGroupBox(this);//tr("设备状态信息"),
    groupBox->setStyleSheet("background-color:white");

    my_widget=new QWidget(this);
    QGridLayout *little_layout=new QGridLayout(my_widget);
    little_layout->addWidget(groupBox);
    my_widget->setStyleSheet("background-color:rgb(0,0,0)");
    little_layout->setContentsMargins(15,10,15,10);

    mainWidget = new QWidget(this);

    lineWidget = new QWidget(this);
    lineWidget->setMaximumWidth(125);

    webWidget = new QWidget(this);
    webWidget->setMaximumWidth(125);
}


void DS_DataShow::timeoutDone(void)
{


}

void DS_DataShow::devAlarmInfo(DS_DsyDev &dev, QString &name)
{
    //    dev = m_dsyDeV;

    //    devData.readSlaveName(dev.ip,dev.num,name);
}

/**
 * @brief 报警检查
 */
bool DS_DataShow::checkAlarm(void)
{
    //    bool ret = devData.isAlarm(m_dsyDeV.ip);
    //    if(ret)
    //    {
    //        m_dsyDeV.num = devData.alarmDevNum(m_dsyDeV.ip); /*报警副机*/
    //        m_dsyDeV.line = devData.alarmLine(m_dsyDeV.alarm);
    //    }
    //    else
    //        m_dsyDeV.alarm = 0;

    //    return ret;
    return false;
}

/**
 * @brief 数据更新
 * @param ip
 */
void DS_DataShow::updateData(QString &ip, int num)
{
    m_dsyDeV.ip = ip;
    m_dsyDeV.num = num; //获取第一个机器编号：0 -1
    m_dsyDeV.line = 0;

    updateData();
}

void DS_DataShow::updateData(void)
{
    if(!m_dsyDeV.ip.isEmpty())
    {
        m_lineBtn->setLine(m_dsyDeV.line);
        //        m_slaveBtn->updateBtn(m_dsyDeV.ip,m_dsyDeV.num);
        m_dataDsy->updateDsp(m_dsyDeV);
        m_webBtn->updateIPAddr(m_dsyDeV.ip);

        emit selectDevSig(m_dsyDeV);
    }
}

/**
 * @brief 选择不同副机
 * @param num
 */
void DS_DataShow::slaveBtnSlot(int num)
{
    m_dsyDeV.num = num;
    updateData();
}

/**
 * @brief 选择不同相数
 * @param line
 */
void DS_DataShow::lineBtnSlot(int line)
{
    if(!m_dsyDeV.ip.isEmpty())
    {
        m_dsyDeV.line = line;
        updateData();
    }
}
