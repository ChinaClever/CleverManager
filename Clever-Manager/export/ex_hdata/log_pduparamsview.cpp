/*
 * log_pduparamsview.cpp
 * 设备电气记录数据TabView
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_pduparamsview.h"
#include "ui_log_pduparamsview.h"
#include "common.h"
#include "db_export/records/dbpduelecparams.h"

Log_PduParamsVIew::Log_PduParamsVIew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log_PduParamsVIew)
{
    ui->setupUi(this);
    initWid();
    initTable();
    tableview_style_png(ui->tableView);

    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(this);

    mDb = DbPduElecParams::get();
    timer = new QTimer(this);
    timer->start(4*60*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(autoDelSlot()));
    QTimer::singleShot(60*1000,this,SLOT(autoDelSlot()));
}

Log_PduParamsVIew::~Log_PduParamsVIew()
{
    delete ui;
}

void Log_PduParamsVIew::initWid()
{
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    model = new DB_SqlTableModel(this);
    ui->tableView->setModel(model->model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));
}

/**
 * @brief 初始化报警日志界面
 */
void Log_PduParamsVIew::initTable()
{
    QStringList headList;
    headList << tr("编号")<< tr("关联编号") << tr("类型") << tr("类型")
             << "Line / " + tr("输出位") << tr("电压") << tr("电流")<< tr("功率")
             << tr("电能")<< tr("功率因数")<< tr("开关状态")<< tr("开关状态");
    model->setHeaders(headList);
    ui->tableView->setColumnHidden(0,true);
    //  ui->tableView->setColumnWidth(7,60);
    // ui->tableView->setColumnWidth(8,300);
}


/**
 * @brief 刷新表
 * @param table
 * @return
 */
bool Log_PduParamsVIew::refreshInitTable(const QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret) {
        // ui->tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->setColumnHidden(1, true);
        ui->tableView->setColumnHidden(2, true);
        ui->tableView->setColumnHidden(10, true);
        initTable();
    }

    return  ret;
}

bool Log_PduParamsVIew::refreshTable(const QString &table)
{
    bool ret = refreshInitTable(table);
    QString cmd = QString("rid = \'%1\' ").arg(-1);
    model->queryFilter(cmd);

    return  ret;
}


void Log_PduParamsVIew::selectedSlot(int rid)
{
    QString cmd = QString("rid = \'%1\' ").arg(rid);
    model->queryFilter(cmd);
}

void Log_PduParamsVIew::autoDelSlot()
{
    int size = LOG_MAX_COUNT;
    int count = mDb->counts();
    if(count > size) {
        refreshTable();
        mDb->countsRemove(size);
    }
}

void Log_PduParamsVIew::getList(QList<QStringList> &list)
{
    QStringList headList;
    headList << tr("编号") << tr("类型")
             << tr("输出位") << tr("电压") << tr("电流")<< tr("功率")
             << tr("电能")<< tr("功率因数") << tr("开关状态");
    list.append(headList);

    int row = model->model->rowCount();
    for(int j=0; j<row; ++j) {
        QStringList strList;
        int column = model->model->columnCount();
        for(int i=0; i<column; ++i)
        {
            if(((i>0) && (i<3)) || (i==10)) continue;
            QModelIndex index = model->model->index(j, i);
            strList << model->model->data(index, Qt::DisplayRole).toString();
        }
        list.append(strList);
    }
    QStringList strLis2t;
    list.append(strLis2t); // 空行
}

void Log_PduParamsVIew::clearTableSlot()
{
    if(mDb) {
        mDb->clear();
        refreshTable();
    }

//    QString table = "pduElecParams";
//    if(model->removeRow(0)) {
//        QTimer::singleShot(10,this,SLOT(clearTableSlot()));
//    } else {
//        DB_SqlQuery sql;
//        sql.clearTable(table);
//        sql.clearTable("pduenvsensor");
//    }
}
