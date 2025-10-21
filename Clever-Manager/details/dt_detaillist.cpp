/*
 * dt_detaillist.cpp
 *  设备列表明细信息
 *    1、按照设备种类、根据分组，显示列表
 *    2、根据IP实时显示每一个设备的统计数据
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_detaillist.h"
#include "listShow/devmanage/ls_managedevlistdlg.h"
#include "common/msgBox/msgbox.h"
#include<qcombobox.h>

DT_DetailList::DT_DetailList(QWidget *parent) : QWidget(parent)
{
    groupBox = new QGroupBox(this);
    mDetailTree = new DT_DetailTree(this); //详细信息表
    mDetailTree->setStyleSheet("background-color: rgb(255,255,255)");
    //双击信号
    connect(mDetailTree,SIGNAL(selectSig(QString,short)),this,SIGNAL(selectSig(QString,short)));

    initWidget();
    initLayout();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    //  timer->start(5*1000);
}

DT_DetailList::~DT_DetailList()
{

}

/**
 * @brief 根据机房键值，来初始化树信息
 * @param id 机房键值
 * @param name  机房各
 */
void DT_DetailList::initData(int id, QString &name)
{
    mDetailTree->initData(id, name);
    QTimer::singleShot(7*1000,this,SLOT(timeoutDone()));
}


/**
 * @brief 初始化窗口
 */
void DT_DetailList::initWidget(void)
{

    //--[刷新按钮]--
    QPalette pal; //调色板
    pal.setColor(QPalette::ButtonText,Qt::white);
    refreshBtn = new QPushButton(tr("刷新"),this);
    refreshBtn->setFont(QFont("微软雅黑",12));
    //    refreshBtn->setMaximumSize(120,40);
    refreshBtn->setFixedHeight(30);
    refreshBtn->setFixedWidth(90);
    //    refreshBtn->setStyleSheet("background-color:rgb(0,90,173)");

    refreshBtn->setStyleSheet("QPushButton{background-color:rgb(12,62,180);}"
                              "QPushButton:hover{background-color:rgb(91, 237, 238);}"
                              "QPushButton:pressed{background-color:rgb(143, 185, 181);}");
    refreshBtn->setPalette(pal);
    connect(refreshBtn,SIGNAL(clicked()), this,SLOT(refreshBtnSlot())); //刷新

    widget = new QWidget(this);
    widget->setMaximumSize(500,40);

    //--[查找区]--
    edit = new QLineEdit(this);
    edit->setPlaceholderText(tr("请输入设备类型或IP查找"));
    edit->setFont(QFont("微软雅黑",12));
    edit->setMaximumSize(180,40);
    edit->setStyleSheet("border-color: rgb(0, 0, 0)");
    edit->setStyleSheet("border-style:solid");
    edit->setStyleSheet("border-width:3px");
    edit->setStyleSheet("background-color:rgb(200,200,200)");

    seekBtn = new QPushButton(tr("查找"),this);
    seekBtn->setFixedHeight(30);
    seekBtn->setFixedWidth(90);
    seekBtn->setFont(QFont("微软雅黑",12));
    //    seekBtn->setStyleSheet("background-color:rgb(0,90,173)");
    seekBtn->setStyleSheet("QPushButton{background-color:rgb(12,62,180);}" "QPushButton:hover{background-color:rgb(91, 237, 238);}" "QPushButton:pressed{background-color:rgb(143, 185, 181);}");

    seekBtn->setPalette(pal);
    seekBtn->setMaximumSize(60,40);
    seekBtn->setFocusPolicy(Qt::NoFocus);
    connect(seekBtn,SIGNAL(clicked()), this,SLOT(seekBtnSlot())); //查找

    //    manageBtn = new QPushButton(tr("设备管理"),this);  // 分组功能未使用
    //    manageBtn->setMaximumSize(100,40);
    //    connect(manageBtn,SIGNAL(clicked()), this,SLOT(manageBtnSlot()));

}

/**
 * @brief 初始化布局
 */
void DT_DetailList::initLayout(void)
{
    QHBoxLayout *hLayout = new QHBoxLayout(widget); //水平布局
    //    hLayout->setSpacing(5);
    hLayout->setContentsMargins(0,20,0,0); //无效[?]
    //    hLayout->addWidget(lab);
    //   hLayout->addWidget(box);
    hLayout->addWidget(edit);
    hLayout->addWidget(seekBtn);
    hLayout->addStretch();
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *phLayout = new QHBoxLayout; //水平布局
    phLayout->addWidget(refreshBtn);
    phLayout->addWidget(widget);
    phLayout->setSpacing(0);
    phLayout->setContentsMargins(0, 10, 0, 10);
    //    phLayout->addWidget(manageBtn);

    QGridLayout *layout = new QGridLayout(groupBox); //网格布局
    layout->addWidget(mDetailTree,0,0);
    layout->addLayout(phLayout,1,0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QGridLayout *pLayout = new QGridLayout(this); //布局到当前界面
    pLayout->addWidget(groupBox);
    groupBox->setStyleSheet("background-color:white");
    pLayout->setSpacing(3);

    pLayout->setContentsMargins(18, 21, 18, 18); //四周留空

}

void DT_DetailList::timeoutDone(void)
{
    mDetailTree->updateWidget(false);
}

/**
 * @brief 刷新
 */
void DT_DetailList::refreshBtnSlot(void)
{
    QTimer::singleShot(5,this,SLOT(timeoutDone()));
}

/**
 * @brief 设备定位
 */
void DT_DetailList::seekBtnSlot(void)
{
    QString str = edit->text();
    if(!str.isEmpty())
        mDetailTree->seekDev(str);
}

/**
 * @brief 设备管理
 */
void DT_DetailList::manageBtnSlot(void)
{
    extern bool usr_land_jur(void);

    bool ret = usr_land_jur();
    if(ret){
        LS_ManageDevListDlg manageDev;
        manageDev.exec();
    }
    else
        InfoMsgBox box(this,tr("您权限不够，无法管理设备"));
}
