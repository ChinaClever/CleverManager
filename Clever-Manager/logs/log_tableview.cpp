/*
 * log_tableview.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_tableview.h"
#include "log_recordbtn.h"
#include "log_querybtn.h"
#include "common.h"
#include "db_export/records/dbtgcabinet.h"
#include "db_export/records/dbtgroom.h"

extern bool usr_land_jur(void);

LOG_TableView::LOG_TableView(QWidget *parent) : QWidget(parent)
{    
    mDb = nullptr;
    view = new QTableView(this); //创建QTableView对象作为视图
    tableview_style_png(view);

    QWidget *little_widget=new QWidget;
    QGridLayout *little_layout=new QGridLayout(little_widget);  //网格布局
    little_layout->addWidget(view,0,0); //view 占满little
    little_layout->setContentsMargins(0,0,0,0);
    mLayout = new QGridLayout(this);
    mLayout->addWidget(little_widget);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0, 0, 0);

    view->setSortingEnabled(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);//
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能
    model = new DB_SqlTableModel(this);
    view->setModel(model->model);//界面与模型绑定
    view->horizontalHeader()->setStretchLastSection(true);

    connect(view,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));

    timer = new QTimer(this);
    timer->start(4*60*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(autoDelSlot()));
    QTimer::singleShot(60*1000,this,SLOT(autoDelSlot()));
}

LOG_TableView::~LOG_TableView()
{

}


/**
 * @brief 初始化报警日志界面
 */
void LOG_TableView::initAlarmTable(void)
{
    m_table = LOG_ALARM_TABLE;  //因为数据库表是英文的
    this->refreshTable(m_table); //刷新表

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("设备IP")<< tr("设备名") << tr("类型") << tr("内容") << tr("相数") << tr("描述");
    model->setHeaders(mHeadList);

    view->setColumnWidth(7,60);
    view->setColumnWidth(8,300);
}

/**
 * @brief 增加报警查询按钮
 */
void LOG_TableView::addAlarmQueryBtn(void)
{
    LOG_AlarmBtn *alarmBtn = new LOG_AlarmBtn(this);
    connect(alarmBtn,SIGNAL(refreshSig()),this,SLOT(refreshSlot()));
    connect(alarmBtn,SIGNAL(delBtnSig()),this,SLOT(delBtnSlot()));
    connect(alarmBtn,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(alarmBtn,SIGNAL(querySig(QString)),this,SIGNAL(querySig(QString)));
    mLayout->addWidget(alarmBtn,1,0);
}

/**
 * @brief 初始化离线表
 */
void LOG_TableView::initOffLineTable(void)
{
    m_table = LOG_OFFLINE_TABLE;
    this->refreshTable(m_table);

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("设备IP")<< tr("设备名") << tr("内容") << tr("描述");
    model->setHeaders(mHeadList);
}

void LOG_TableView::addOffLineQueryBtn(void)
{
    LOG_AlarmBtn *alarmBtn = new LOG_AlarmBtn(this);
    alarmBtn->initOffLineBtn();
    connect(alarmBtn,SIGNAL(refreshSig()),this,SLOT(refreshSlot()));
    connect(alarmBtn,SIGNAL(delBtnSig()),this,SLOT(delBtnSlot()));
    connect(alarmBtn,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(alarmBtn,SIGNAL(querySig(QString)),this,SIGNAL(querySig(QString)));
    mLayout->addWidget(alarmBtn,1,0);
}

/**
 * @brief 初始化用户记录日志
 */
void LOG_TableView::initRecordTable(const QString &table)
{
    m_table = table;
    this->refreshTable(m_table);

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("用户") << tr("操作") << tr("内容") << tr("描述");
    model->setHeaders(mHeadList);

    view->setColumnWidth(6,260);
}

/**
 * @brief 增加记录查询按钮
 */
void LOG_TableView::addRecordQueryBtn(void)
{
    LOG_RecordBtn *recordBtn = new LOG_RecordBtn(this);
    connect(recordBtn,SIGNAL(refreshSig()),this,SLOT(refreshSlot()));
    connect(recordBtn,SIGNAL(delBtnSig()),this,SLOT(delBtnSlot()));
    connect(recordBtn,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(recordBtn,SIGNAL(querySig(QString)),this,SIGNAL(querySig(QString)));

    mLayout->addWidget(recordBtn,1,0);
}

void LOG_TableView::initLandTable(void)
{
    initRecordTable(LOG_LAND_TABLE);
}

void LOG_TableView::addLandQueryBtn(void)
{
    addRecordQueryBtn();
}

void LOG_TableView::initRoomTable(void)
{
    m_table = "tgRoom";
    this->refreshTable(m_table);

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("机房名")<< tr("平均电压")
              << tr("总电流") << tr("总功率") << tr("总电能") << tr("功率因数")
              <<  tr("温度") << tr("湿度") << tr("设备总数")
               << tr("在线") << tr("报警") << tr("离线");
    model->setHeaders(mHeadList);
}

void LOG_TableView::addRoomQueryBtn(void)
{
    LOG_QueryBtn *recordBtn = new LOG_QueryBtn(this);
    recordBtn->initRoom(); recordBtn->initWidget();

    connect(recordBtn,SIGNAL(refreshSig()),this,SLOT(refreshSlot()));
    connect(recordBtn,SIGNAL(delBtnSig()),this,SLOT(delBtnSlot()));
    connect(recordBtn,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(recordBtn,SIGNAL(querySig(QString)),this,SIGNAL(querySig(QString)));
    mLayout->addWidget(recordBtn,1,0);
}

void LOG_TableView::initCabTable(void)
{
    m_table = "tgcab";
    this->refreshTable(m_table);

    mHeadList << tr("编号") << tr("日期") << tr("时间") << tr("机房名")<< tr("机柜名")<< tr("主路电流")
              << tr("备路电流") << tr("总电流") << tr("主路电压") << tr("备路电压")
              << tr("主路电能") << tr("备路电能") << tr("总功率");
    model->setHeaders(mHeadList);
}

void LOG_TableView::addCabQueryBtn(void)
{
    LOG_QueryBtn *recordBtn = new LOG_QueryBtn(this);
    recordBtn->initCab(); recordBtn->initWidget();

    connect(recordBtn,SIGNAL(refreshSig()),this,SLOT(refreshSlot()));
    connect(recordBtn,SIGNAL(delBtnSig()),this,SLOT(delBtnSlot()));
    connect(recordBtn,SIGNAL(querySig(QString)),model,SLOT(queryFilter(QString)));
    connect(recordBtn,SIGNAL(querySig(QString)),this,SIGNAL(querySig(QString)));
    mLayout->addWidget(recordBtn,1,0);
}

/**
 * @brief 刷新表
 * @param table
 * @return
 */
bool LOG_TableView::refreshTable(QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret)
    {
        m_table = table;
        view->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        view->setColumnHidden(0, true); //No.信息（隐藏第一列）
    }
    return  ret;
}

/**
 * @brief 双击删除
 */
void LOG_TableView::doubleSlot(QModelIndex)
{
    bool ret = usr_land_jur();
    if(ret==false){
        return ;
    }

    QString str = tr("是否删除这条记录?");
    QuMsgBox box(this, str);
    ret = box.Exec();
    if(ret)
    {
        int curRow = view->currentIndex().row();
        model->removeRow(curRow);
    }
}

void LOG_TableView::refreshSlot(void)
{
    if(LOG_ALARM_TABLE == m_table)
        initAlarmTable();
    else if(LOG_ReCORD_TABLE == m_table)
        initRecordTable();
    else if(LOG_LAND_TABLE == m_table)
        initLandTable();
    else if(LOG_OFFLINE_TABLE == m_table)
        initOffLineTable();
    else if("tgRoom" == m_table) {
        initRoomTable();
        mDb = DbTgRoom::get();
    } else if("tgcab" == m_table) {
        initCabTable();
        mDb = DbTgCabinet::get();
    }
    emit querySig("");
}

void LOG_TableView::autoDelSlot()
{
    if(mDb) {
        int count = mDb->counts();
        if(count > LOG_MAX_COUNT) {
            refreshSlot();
            mDb->countsRemove(LOG_MAX_COUNT);
        }
    }
}

void LOG_TableView::clearTableSlot()
{
    if(mDb) {
        mDb->clear();
        QTimer::singleShot(100,this,SLOT(refreshSlot()));
    } else {
        if(model->removeRow(0))
            QTimer::singleShot(10,this,SLOT(clearTableSlot()));
    }
}

void LOG_TableView::delBtnSlot(void)
{
    bool ret = usr_land_jur();
    if(ret == false)
    {
        InfoMsgBox(this, tr("您无法进行此操作"));
        return ;
    }

    QuMsgBox box(this,tr("确定清空日志信息"));
    ret = box.Exec();
    if(ret)
        QTimer::singleShot(100,this,SLOT(clearTableSlot())); //延时初始化
}

QString LOG_TableView::getTableName()
{
    return m_table;
}

void LOG_TableView::getHeadList(QStringList &list)
{
    list = mHeadList;
}

void LOG_TableView::selectedSlot(QString &date)
{
    QString str = tr("date like '%%1%'").arg(date);
    model->queryFilter(str);
}
