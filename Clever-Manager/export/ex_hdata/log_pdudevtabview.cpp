/*
 * log_pdudevtabview.cpp
 * 设备记录数据TabView
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_pdudevtabview.h"
#include "ui_log_pdudevtabview.h"
#include "common.h"
#include "db_export/records/dbpdudevindex.h"

Log_PduDevTabView::Log_PduDevTabView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log_PduDevTabView)
{
    ui->setupUi(this);
    initWid();
    initTable();
    tableview_style_png(ui->tableView);

    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(this);

    mDb = DbPduDevIndex::get();
    timer = new QTimer(this);
    timer->start(4*60*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(autoDelSlot()));
    QTimer::singleShot(60*1000,this,SLOT(autoDelSlot()));
}

Log_PduDevTabView::~Log_PduDevTabView()
{
    delete ui;
}


void Log_PduDevTabView::initWid()
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

    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));
}

/**
 * @brief 初始化报警日志界面
 */
void Log_PduDevTabView::initTable()
{
    QStringList headList;
    headList << tr("编号")<< tr("日期") << tr("时间") << tr("IP地址")
             << tr("级联号") << tr("设备类型")<< tr("设备类型")
             << tr("机房名")<< tr("机柜名")<< tr("工作状态")<< tr("工作状态");
    model->setHeaders(headList);
    model->setFont(QFont("微软雅黑",10));
    ui->tableView->setColumnHidden(0,true);
    //  ui->tableView->setColumnWidth(7,60);
    // ui->tableView->setColumnWidth(8,300);
}


/**
 * @brief 刷新表
 * @param table
 * @return
 */

bool Log_PduDevTabView::refreshInitTable(const QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret) {
        // ui->tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->setColumnHidden(5, true);
        ui->tableView->setColumnHidden(9, true);
        initTable();
    }

    return  ret;
}

bool Log_PduDevTabView::refreshTable(const QString &table)
{
    bool ret = refreshInitTable(table);
    QString cmd = QString("ip = \'%1\' ").arg("");
    model->queryFilter(cmd);

    return  ret;
}

void Log_PduDevTabView::autoDelSlot()
{
    int size = LOG_MAX_COUNT;
    int count = mDb->counts();
    if(count > size) {
        refreshTable();
        mDb->countsRemove(size);
    }
}

void Log_PduDevTabView::selected(const QString &cmd)
{
    model->queryFilter(cmd);
}

void Log_PduDevTabView::setSelectRow(int row)
{
    ui->tableView->selectRow(row);
    QModelIndex index;
    doubleSlot(index);
}

void Log_PduDevTabView::getList(QList<QStringList> &list)
{
    QStringList headList;
    headList<< tr("编号") << tr("日期") << tr("时间") << tr("IP地址")
            << tr("级联号") << tr("设备类型") << tr("机房名")<< tr("机柜名")<< tr("工作状态");
    list.append(headList);
    QStringList strList;
    int curRow = ui->tableView->currentIndex().row();
    int column = model->model->columnCount();
    for(int i=0; i<column; ++i)
    {
        if((i==5) || (i==9)) continue;
        QModelIndex index = model->model->index(curRow, i);
        strList << model->model->data(index, Qt::DisplayRole).toString();
    }
    list.append(strList);

    QStringList strLis2t;
    list.append(strLis2t); // 空行
}

/**
 * @brief 双击删除
 */
void Log_PduDevTabView::doubleSlot(QModelIndex)
{
    int curRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->model->index(curRow, 0);
    int rid = model->model->data(index, Qt::DisplayRole).toInt();
    emit selectedSig(rid);
}

void Log_PduDevTabView::clearTableSlot()
{
    if(mDb) {
        mDb->clear();
        refreshTable();
    }

//    QString table = "pduDevIndex";
//    if(model->removeRow(0)) {
//        QTimer::singleShot(10,this,SLOT(clearTableSlot()));
//    } else {
//        DB_SqlQuery sql;
//        sql.clearTable(table);
//    }
}
