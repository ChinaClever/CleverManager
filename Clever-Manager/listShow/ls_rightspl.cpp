#include "ls_rightspl.h"
#include "common.h"

LS_RightSpl::LS_RightSpl(QWidget *parent) : QWidget(parent)
{
    initGroupBox();
    initWidget();

    this->setStyleSheet("QTabWidget::pane{border:0px;};");
    mTcp = TcpSent::bulid(this);
}

void LS_RightSpl::initGroupBox(void)
{
    mHeadWid = new LS_RightHeadWid();
    QWidget *little_widget=new QWidget(this);
    little_widget->setStyleSheet("background-color:rgb(0,45,130);");
    little_widget->setFixedHeight(40);
    QHBoxLayout *little_layout=new QHBoxLayout(little_widget);
    little_layout->addWidget(mHeadWid);
    //    mHeadWid->setStyleSheet("background-color:rgb(0,91,172)");

    mTabWidget = new QTabWidget(this);//最外层的
    mTabWidget->setFont(QFont("微软雅黑",12));
    mTabWidget->setStyleSheet("QTabBar::tab{height:0}");
    mTabWidget->setTabPosition(QTabWidget::South);
    connect(mHeadWid, SIGNAL(selectSig(int)), mTabWidget, SLOT(setCurrentIndex(int)));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(little_widget,0,0);
    layout->addWidget(mTabWidget,1,0);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

void LS_RightSpl::initWidget(void)
{
    mDevWidget = new DS_DevWidget(mTabWidget);
    mTabWidget->addTab(mDevWidget,  tr("设备"));

    mOutput = new UI_OutputWidget(mTabWidget);
    mTabWidget->addTab(mOutput,  tr("输出位"));

//    mLoopWidget=new UI_LoopWidget(mTabWidget);
//    mLoopWidget->setStyleSheet("background-color:rgb(204,232,207)");
//    mTabWidget->addTab(mLoopWidget,tr("test"));

    mEnvTable = new UI_EnvTable(mTabWidget);
    mTabWidget->addTab(mEnvTable,  tr("传感器"));

    mSetDev = new SetDevMainWid(mTabWidget);
    mTabWidget->addTab(mSetDev,  tr("设备配置"));
}


/**
 * @brief 列表树响应函数
 * @param ip 设备IP
 * @param num 设备号
 */
void LS_RightSpl::selectSlot(QString &ip, int num)
{
    mDevWidget->selectDev(ip, num);
    mOutput->updateWidget(ip, num);
    mEnvTable->updateWidget(ip, num);
    mSetDev->updateWidget(ip, num);
//    mLoopWidget->getDataPacket(ip,num); //add

    mTcp->newConnect(ip); // TCP 连接
}
