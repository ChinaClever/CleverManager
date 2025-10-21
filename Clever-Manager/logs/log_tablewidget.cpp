#include "log_tablewidget.h"

LOG_TableWidget::LOG_TableWidget(QWidget *parent) : QWidget(parent)
{
     mSplitter = new QSplitter(Qt::Horizontal,this);
     mSplitter->setHandleWidth(0); //宽度设置
     initWidet();
     initLayout();
     QTimer::singleShot(300,this,SLOT(initFunSLot())); //延时初始化
}


void LOG_TableWidget::initWidet(void)
{
    mDateWidget = new LOG_DateWidget(mSplitter);
    mDateWidget->setMaximumWidth(250);
    mTableView = new LOG_TableView(mSplitter);
    //查看某天的日志
    connect(mDateWidget,SIGNAL(selectedSig(QString&)), mTableView,SLOT(selectedSlot(QString&)));
    connect(mTableView,SIGNAL(querySig(QString)), mDateWidget,SLOT(querySlot(QString)));
}

void LOG_TableWidget::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(this); //网格布局
    layout->addWidget(mSplitter);  //分离器
    layout->setSpacing(0); //间隔
    layout->setContentsMargins(0, 0, 0, 0);

    mSplitter->setStretchFactor(0,4); //设置弹力因子
    mSplitter->setStretchFactor(1,6);
}

void LOG_TableWidget::initFunSLot()
{
    QString tableName = mTableView->getTableName();
    mDateWidget->setTable(tableName);

    QStringList head;
    mTableView->getHeadList(head); //头行
    mDateWidget->setHeadList(head);
}
