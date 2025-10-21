/*
 * log_showclass.cpp
 * 日志显示类
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_showclass.h"
#include <QLabel>
#include "common.h"

LOG_ShowClass::LOG_ShowClass(QWidget *parent) : QWidget(parent)
{
    initTitle(); //全部布局
    tabWidget = new QTabWidget(mWidget);
    tabWidget->setStyleSheet("QTabBar::tab{height:0px}；" "border:0px;" "QTabWidget::pane{top:0px;};" "background-color:black;");
    connect(mHeadWid, SIGNAL(selectSig(int)), tabWidget, SLOT(setCurrentIndex(int))); //切换打印日志对象

    initLayout(); //tabWidget 内布局
    index = 0;
    QTimer::singleShot(1200,this,SLOT(initFunSLot())); //延时初始化

}

LOG_ShowClass::~LOG_ShowClass()
{

}


void LOG_ShowClass::initTitle()
{
    mWidget = new QWidget(this);
    //     set_background_color(mWidget, QColor(0,45,130));

    mHeadWid = new LOG_HeadWid(this); //切换条
    QWidget *little_widget=new QWidget;

    little_widget->setFixedHeight(40); //固定高度
    QVBoxLayout *little_layout=new QVBoxLayout(little_widget);
    little_widget->setStyleSheet("background-color:rgb(0,45,130)");
    little_layout->addWidget(mHeadWid);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(little_widget,0,0);
    layout->addWidget(mWidget,1,0);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

void LOG_ShowClass::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(mWidget);
    layout->setContentsMargins(0,15,15,0);
    layout->setSpacing(0); //间距
    layout->addWidget(tabWidget);
}

void LOG_ShowClass::initFunSLot()
{
    switch (++index) {
    case 1: initAlarmTable(); break;
    case 2: initOffLineTable(); break;
    case 3: initRecordTable(); break;
    case 4: initLandTable(); break;
    case 5: initRoomTable(); break;
    case 6: initCabTable(); break;
    case 7: initDevWid(); break;
    case 8: initEleWid(); break;
//    case 8:initHistoryWid();break;     ////====    lzy 2018/6/7
    default:  return;
    }
    QTimer::singleShot(1000,this,SLOT(initFunSLot())); //延时初始化 -- 到default自动停止
}


/**
 * @brief 初始化报警日志表
 */
void LOG_ShowClass::initAlarmTable(void)
{
    alarmTable = new LOG_TableWidget(tabWidget);  //延时300ms更新mDateWidget
    alarmTable->mTableView->initAlarmTable(); //--初始化表显示 [区分]
    alarmTable->mTableView->addAlarmQueryBtn();  //查询区

    QString str = tr("报警日志");
    alarmTable->mDateWidget->setTitle(str);
    tabWidget->addTab(alarmTable, tr("报警日志"));
    tabWidget->setFont(QFont("微软雅黑",12));
}

void LOG_ShowClass::initOffLineTable(void)
{
    offLineTable = new LOG_TableWidget(tabWidget);
    offLineTable->mTableView->initOffLineTable();
    offLineTable->mTableView->addOffLineQueryBtn();

    QString str = tr("离线日志");
    offLineTable->mDateWidget->setTitle(str);
    tabWidget->addTab(offLineTable, tr("离线日志"));
}

/**
 * @brief 初始化记录日志表
 */
void LOG_ShowClass::initRecordTable(void)
{
    recordTable = new LOG_TableWidget(tabWidget);
    recordTable->mTableView->initRecordTable();
    recordTable->mTableView->addRecordQueryBtn();

    QString str = tr("操作日志");
    recordTable->mDateWidget->setTitle(str);
    tabWidget->addTab(recordTable, tr("操作日志"));
}

void LOG_ShowClass::initLandTable(void)
{
    landTable = new LOG_TableWidget(tabWidget);
    landTable->mTableView->initLandTable();
    landTable->mTableView->addLandQueryBtn();

    QString str = tr("用户日志");
    landTable->mDateWidget->setTitle(str);
    tabWidget->addTab(landTable, tr("用户日志"));
}

void LOG_ShowClass::initRoomTable(void)
{
    roomTable = new LOG_TableWidget(tabWidget);
    roomTable->mTableView->initRoomTable();
    roomTable->mTableView->addRoomQueryBtn();

    QString str = tr("机房记录");
    roomTable->mDateWidget->setTitle(str);
    tabWidget->addTab(roomTable, tr("机房记录"));
}

void LOG_ShowClass::initCabTable(void)
{
    DbTgCabinet::get();
    cabTable = new LOG_TableWidget(tabWidget);
    cabTable->mTableView->initCabTable();
    cabTable->mTableView->addCabQueryBtn();

    QString str = tr("机柜记录");
    cabTable->mDateWidget->setTitle(str);
    tabWidget->addTab(cabTable, tr("机柜记录"));
}

void LOG_ShowClass::initEleWid()
{
    eleWid = new Log_EleWid(tabWidget);
    tabWidget->addTab(eleWid, tr("电能记录"));
}

void LOG_ShowClass::initDevWid()
{
    devWid = new Log_HistoryDataWid(tabWidget);
    tabWidget->addTab(devWid, tr("设备数据记录"));
}

void LOG_ShowClass::initHistoryWid()
{
    QWidget *mainWid = new QWidget(tabWidget);
    LOG_HistoryWid *historyWid = new LOG_HistoryWid(tabWidget);
    historyWid->setFixedWidth(200);
    QWidget *diaWid = new QWidget(tabWidget);
//    diaWid->setFixedSize(1704,843);

    QHBoxLayout *layout = new QHBoxLayout(mainWid);
    layout->addWidget(historyWid);
    layout->addWidget(diaWid);

    DiagramWid *widget = new DiagramWid(diaWid);
    qDebug() << "diaWid" << diaWid->width() << diaWid->height();
    tabWidget->addTab(mainWid,tr("历史数据查询"));

    connect(historyWid,SIGNAL(signalChart(chartData*)),widget,SLOT(getSignalChart(chartData*)));
}
