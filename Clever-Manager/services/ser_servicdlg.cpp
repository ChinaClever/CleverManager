/*
 * ser_servicdlg.cpp
 * 服务设置主界面
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ser_servicdlg.h"
#include "common/configtool.h"

static int comtext=3;
int getTime();


Ser_ServiceDlg::Ser_ServiceDlg(QWidget *parent) : QWidget(parent)
{
    initTitle();
    mainSplitter=new QSplitter(Qt::Horizontal,mWidget);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(0,90,173)}");
    mainSplitter->setHandleWidth(0);
    initWidget();
}

Ser_ServiceDlg::~Ser_ServiceDlg()
{

}

void Ser_ServiceDlg::initTitle()
{
    mWidget = new QWidget(this);
    QWidget *title=new QWidget(this);
    //     title->setMaximumHeight(38);
    //     title->setMinimumHeight(37);
    title->setFixedHeight(40);
    title->setStyleSheet("background-color: rgb(0,45,130)");

    QHBoxLayout *up_layout=new QHBoxLayout(title);
    //    QLabel *label=new QLabel(tr("  采样频率："));
    QLabel *label=new QLabel(tr("  记录频率："),this);

    label->setFont(QFont("微软雅黑",12));
    //    label->setStyleSheet("color:white; background-color:rgb(0,45,130);");
    label->setStyleSheet("background-color:rgb(0,45,130); color: rgb(255,255,255);");
    combox=new QComboBox;
    int time = ConfigTool::get()->getLogRecordTime();
#if 0
    combox->addItem(tr("1分钟"));
    combox->addItem(tr("5分钟"));
    combox->addItem(tr("10分钟"));
    combox->addItem(tr("30分钟"));
    combox->addItem(tr("1小时"));
    combox->addItem(tr("2小时"));
    combox->addItem(tr("12小时"));
    combox->addItem(tr("24小时"));
    int index = combox->count()-1;
    switch(time)
    {
    case 1: index=0; break;
    case 5: index=1; break;
    case 10: index=2; break;
    case 30: index=3; break;
    case 60: index=4; break;
    case 2*60: index=5; break;
    case 12*60: index=6; break;
    case 24*60: index=7; break;
    default: index = 7; break;
    }
#else
    combox->addItem(tr("1小时"));
    combox->addItem(tr("2小时"));
    combox->addItem(tr("12小时"));
    combox->addItem(tr("24小时"));
    int index = combox->count()-1;
    switch(time)
    {
    case 60: index=0; break;
    case 2*60: index=1; break;
    case 12*60: index=2; break;
    case 24*60: index=3; break;
    default: index = 3; break;
    }
#endif
    combox->setFont(QFont("微软雅黑",12));
    combox->setStyleSheet("color:white;" "background-color:transparent;");

    combox->setCurrentIndex(index);
    comtext=combox->currentIndex();
    up_layout->addWidget(label);
    up_layout->addWidget(combox);
    up_layout->addStretch();
    up_layout->setSpacing(2);

    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->addWidget(title,0,0);

    layout->addWidget(mWidget,1,0);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(combox,SIGNAL(currentIndexChanged(int)),this,SLOT(indexChangeDone(int)));
}


/**
 * @brief 左部分分割窗口
 */
void Ser_ServiceDlg::initLeftSpl(void)
{
    leftSplitter=new QSplitter(Qt::Vertical,mainSplitter);
    leftSplitter->setHandleWidth(0);

    m_emil = new Email_TestDlg(leftSplitter);
    //m_dingTalk = new DT_MainWid(leftSplitter);
    // groupBox_background_icon(m_emil);

    m_serialDlg = new SIM900A_SerialDlg(leftSplitter);
    //groupBox_background_icon(m_serialDlg);

    m_simDlg = new SIM900A_Interface(leftSplitter);
    //groupBox_background_icon(m_simDlg);
}

/**
 * @brief 右部分分割窗口
 */
void Ser_ServiceDlg::initRightSpl(void)
{
    rightSplitter=new QSplitter(Qt::Horizontal,mainSplitter);
    m_usrDlg = new UsrManageDlg(rightSplitter);
}

void Ser_ServiceDlg::initWidget(void)
{
    initLeftSpl();
    initRightSpl();

    initLayout();
}

void Ser_ServiceDlg::initLayout(void)
{
    leftSplitter->setStretchFactor(0,1);//fen
    leftSplitter->setStretchFactor(1,6);
    leftSplitter->setStretchFactor(2,5);

    mainSplitter->setStretchFactor(0,2);
    mainSplitter->setStretchFactor(1,8);

    QGridLayout *layout = new QGridLayout(mWidget);
    layout->addWidget(mainSplitter);
}

void Ser_ServiceDlg::indexChangeDone(int str)
{
    comtext=str;
    ConfigTool::get()->setLogRecordTime(getTime());
}

int getTime()
{
    int time = 60;
    switch (comtext) {
#if 0
    case 0: time=1; break;
    case 1: time=5; break;
    case 2: time=10; break;
    case 3: time=30; break;
    case 4: time=60; break;
    case 5: time=2*60; break;
    case 6: time=12*60; break;
    case 7: time=24*60; break;
#else
    case 0: time=60; break;
    case 1: time=2*60; break;
    case 2: time=12*60; break;
    case 3: time=24*60; break;
#endif
    default: break;
    }
    return time;
}
