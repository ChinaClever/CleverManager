/*
 * log_eletabview.cpp
 * 电能TabView
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_eletabview.h"
#include "ui_log_eletabview.h"
#include "common.h"

extern bool usr_land_jur(void);

Log_EleTabView::Log_EleTabView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log_EleTabView)
{
    ui->setupUi(this);
    initWid();
    initTable();
    tableview_style_png(ui->tableView);

    QGridLayout *layout = new QGridLayout(parent);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(this);
}

Log_EleTabView::~Log_EleTabView()
{
    delete ui;
}

void Log_EleTabView::initWid()
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

    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));
}

/**
 * @brief 初始化报警日志界面
 */
void Log_EleTabView::initTable()
{
    QStringList headList;
    headList << tr("编号")<< tr("名称") << tr("开始时间") << tr("电能(kWh)")
             << tr("结束时间") << tr("电能(kWh)") << tr("差值(kWh)");
    model->setHeaders(headList);
    ui->tableView->setColumnHidden(0,true);
    //  ui->tableView->setColumnWidth(7,60);
    // ui->tableView->setColumnWidth(8,300);
// ui->tableView->verticalHeader()->setFixedWidth(40);
// ui->tableView->verticalHeader()->setStyleSheet("color:transparent");
}


/**
 * @brief 刷新表
 * @param table
 * @return
 */
bool Log_EleTabView::refreshTable(const QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret) {
        //        ui->tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        //        ui->tableView->setColumnHidden(0, true);
        initTable();
        QString str = tr("查询数据成功！");
        InfoMsgBox box(this,str);
    }

    return  ret;
}

/**
 * @brief 双击删除
 */
void Log_EleTabView::doubleSlot(QModelIndex)
{
    bool ret = usr_land_jur();
    if(ret==false){
        return ;
    }

    QString str = tr("是否删除这条记录?");
    QuMsgBox box(this, str);
    if(box.Exec()) {
        int curRow = ui->tableView->currentIndex().row();
        model->removeRow(curRow);
    }
}

void Log_EleTabView::clearTableSlot()
{
    DB_SqlQuery sql;
    int id = sql.tableSize(LOG_TEMP_ELE);
    if(id > 0) {
        model->removeRow(0);
        QTimer::singleShot(100,this,SLOT(clearTableSlot()));
    }
}

