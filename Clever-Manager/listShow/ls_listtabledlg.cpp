 /*
 * ls_listtabledlg.cpp
 *
 * 设备列表窗口
 *  提供一个接口 updateWidget
 *  提供一个信号 selectedSig
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_listtabledlg.h"
#include "db_sqlquery.h"
#include "common.h"
#include "pdu/dev/devmanage/pdudmmanage.h"

extern bool usr_land_jur(void);

LS_listTabledlg::LS_listTabledlg(QWidget *parent) : QWidget(parent)
{
    initWidget();

    initLineTree();
    initAlarmTree();
    initOffTree();
    initListTree();

    updateWidget();
    QTimer::singleShot(8*1000,this,SLOT(updateWidget())); // 6S后自动更新一次
}

LS_listTabledlg::~LS_listTabledlg()
{

}

void LS_listTabledlg::initWidget(void)
{
    QWidget *groupBox = new QWidget(this);  //tr("设备列表"),

   groupBox->setStyleSheet("background-color:rgb(255,255,255); border:0px");

    mTabWidget = new QTabWidget(groupBox);
//   mTabWidget->setStyleSheet("QTabBar::tab{height:0};" " border-color:transparent");
     mTabWidget->setStyleSheet("QTabBar::tab{height:0};");

    mTableHead = new LS_listTableHead(groupBox);
    connect(mTableHead, SIGNAL(selectSig(int)), mTabWidget, SLOT(setCurrentIndex(int)));

    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(mTableHead,0,0);
    layout->addWidget(mTabWidget,1,0);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(10);
    QLabel *label=new QLabel(tr("    设备列表"),this);
    label->setStyleSheet("background-color:rgb(0,45,130); color: rgb(255, 255, 255);");

    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    label->setPalette(pal);
    label->setFixedHeight(40);
    label->setFont(QFont("微软雅黑",12));

    QGridLayout *little_layout=new QGridLayout(groupBox);
    little_layout->addWidget(groupBox);
    little_layout->setContentsMargins(18,12,18,0);
    m_layout->addWidget(label,0,0);
    m_layout->addLayout(little_layout,1,0);
}

void LS_listTabledlg::initListTree(void)
{
    mListTree = new LS_treeWidget(mTabWidget);

    QStringList head;
    head << tr("PDU设备列表") << "";
    mListTree->setHeaderLabels(head);
    mTabWidget->addTab(mListTree,  tr("设备"));

    connect(mListTree,SIGNAL(selectedSig(QString &, int)),this,SLOT(listSeletedSlot(QString &, int)));
}

/**
 * @brief 初始化在线设备
 */
void LS_listTabledlg::initLineTree(void)
{
    mLineTree = new LS_treeWidget(mTabWidget);

    QStringList head;
    head << tr("PDU在线列表") << "";
    mLineTree->setHeaderLabels(head);
    mTabWidget->addTab(mLineTree,  tr("在线"));

    connect(mLineTree,SIGNAL(selectedSig(QString &, int)),this,SIGNAL(selectedSig(QString &, int)));
}

/**
 * @brief
 */
void LS_listTabledlg::initAlarmTree(void)
{
    mAlarmTree =new LS_treeWidget(mTabWidget);

    QStringList head;
    head << tr("PDU报警设备") << "";
    mAlarmTree->setHeaderLabels(head);
//    mAlarmTree->setFont(QFont("微软雅黑",12));

    mTabWidget->addTab(mAlarmTree,  tr("报警"));
    mTabWidget->setFont(QFont("微软雅黑",12));
    connect(mAlarmTree,SIGNAL(selectedSig(QString &, int)),this,SIGNAL(selectedSig(QString &, int)));
}


/**
 * @brief 初始化
 */
void LS_listTabledlg::initOffTree(void)
{
    mOffLineTree = new LS_treeWidget(mTabWidget);

    QStringList head;
    head << tr("PDU离线设备") << "";
    mOffLineTree->setHeaderLabels(head);

    mTabWidget->addTab(mOffLineTree,  tr("离线"));
    connect(mOffLineTree,SIGNAL(selectedSig(QString &, int)),this,SLOT(listSeletedSlot(QString &, int)));
}

/**
 * @brief 更新窗口
 */
void LS_listTabledlg::updateWidget(void)
{
    mListTree->updateListDev(false);
    mLineTree->updateline(false);
    mAlarmTree->updateAlarm(false);
    mOffLineTree->updateOffLine(false);
}

void LS_listTabledlg::listSeletedSlot(QString &ip, int num)
{
    PduDataPacket *packet =  pdu_hd_getPacket(ip,  num);
    if(packet->offLine < 1)
    {
        bool ret = usr_land_jur();  //管理权限
        if(ret==false)
            return ;

        QString str =tr("\nIP:%1  %2").arg(ip).arg(packet->info->type->name->get());
        str +=  "\n" + tr("设备已离线！！！") + "\n" + tr("是否删除此设备？");
        WaringMsgBox box(this,str);
        ret = box.Exec();
        if(ret)
        {
            pdu_dm_del(packet->devType, ip);
            updateWidget();

            /*增加日志信息*/
            s_RecordLog log;
            log.title = tr("设备管理");
            log.operation = tr("设备删除");
            str = packet->info->type->name->get();
            log.msg = tr("设备IP：%1 设备名：%2").arg(ip).arg(str);
            sql_insert_record(log);
        }
    }
    else
        emit selectedSig(ip, num);
}

/**
 * @brief 有设备发生报警
 * @param list
 */
void LS_listTabledlg::alarmSlot(void)
{
    mAlarmTree->updateAlarm(true);
}


void LS_listTabledlg::updateSlot(void)
{
    updateWidget();
}
