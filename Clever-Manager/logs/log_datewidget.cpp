/*
 * log_datewidget.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_datewidget.h"
#include "ui_log_datewidget.h"
#include "common.h"
#include "log_exportdlg.h"

LOG_DateWidget::LOG_DateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LOG_DateWidget)
{
    ui->setupUi(this);
    //    ui->gridLayout->setContentsMargins(18, 3, 18, 18);

    mDateHash = new LOG_DateHash();
    //mTreeWidget->setSortingEnabled(true);
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(selectedItemSlot(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this,SLOT(clickedSLot(QModelIndex)));

    QStringList headers;
    headers << tr("日期列表");
    setHeaderLabels(headers);

    set_background_color(this, QColor(0,17,55));
    btnBlue_style_sheet(ui->exportBtn);
    btnBlue_style_sheet(ui->updateBtn);
}

LOG_DateWidget::~LOG_DateWidget()
{
    delete ui;
    delete mDateHash;
}

void LOG_DateWidget::setTable(const QString &name)
{
    mTableName = name;
    mDateHash->updateTab(name); //查数据库 -- 填写年月日list
    updateWidget(); //更新datewidget显示
}



/**
 * @brief 设置头的标题
 * @param headers
 */
void LOG_DateWidget::setHeaderLabels(QStringList &headers)
{
    ui->treeWidget->setHeaderLabels(headers);
}

void LOG_DateWidget::initWidget(void)
{
    ui->treeWidget->clear();  //清空
    ui->treeWidget->setColumnCount(1); //设置列数
}


int LOG_DateWidget::dayShow(QString &month, QTreeWidgetItem *item)
{
    QStringList dayList;
    mDateHash->getDayList(month, dayList);

    for(int j=0; j<dayList.size(); ++j)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, dayList[j]);
        item->addChild(subItem);
    }

    return dayList.size();
}

int LOG_DateWidget::monthShow(QString &year, QTreeWidgetItem *item)
{
    QStringList monthList;
    mDateHash->getMonthList(year, monthList);
    for(int j=0; j<monthList.size(); ++j)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, monthList[j]);

        dayShow(monthList[j],subItem);
        item->addChild(subItem);
    }

    return monthList.size();
}


void LOG_DateWidget::yearShow(void) //年-->月-->日
{
    QStringList yearList;
    mDateHash->getYearList(yearList);

    initWidget();
    QList<QTreeWidgetItem *> rootList;
    for(int d=0; d<yearList.size(); ++d)
    {
        QString year = yearList.at(d);
        QTreeWidgetItem *yearItem = new QTreeWidgetItem(ui->treeWidget);
        yearItem->setText(0, year+ QObject::tr("年"));
        rootList.append(yearItem);

        int rtn = monthShow(year, yearItem);
        if(rtn == 0) {
            rootList.removeLast();
            delete yearItem;
        }
    }

    ui->treeWidget->insertTopLevelItems(0,rootList);  //将结点插入部件中
}

void LOG_DateWidget::updateWidget()
{
    yearShow();
}

bool LOG_DateWidget::isDate(const QString &date)
{
    bool ret = false;
    QStringList list = date.split("-",QString::SkipEmptyParts);
    if(list.size() == 3) {
        ret = true;
    }
    return ret;
}

bool LOG_DateWidget::selectedItemSlot(QTreeWidgetItem *item,int column)
{
    if(item->parent())
    {
        QString str = item->text(column);
        bool ret = isDate(str);
        if(ret)  {
            mDate = str;
            emit selectedSig(mDate);
        }
    }

    return true;
}

/**
 * @brief 查找定位
 */
void LOG_DateWidget::seekDate(const QString &date)
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it)
    {
        if((*it)->text(0) == date)
        {
            ui->treeWidget->collapseAll(); //全部关闭
            ui->treeWidget->setCurrentIndex(ui->treeWidget->rootIndex());
            ui->treeWidget->setCurrentItem((*it));

            QModelIndex index = ui->treeWidget->currentIndex();
            ui->treeWidget->expand(index);
        }
        ++it;
    }
}


/**
 * @brief 实现单击点开操作
 * @param index
 */
void LOG_DateWidget::clickedSLot(QModelIndex index)
{
    bool ret = ui->treeWidget->isExpanded(index);
    if(ret)
        ui->treeWidget->collapse(index);
    else
        ui->treeWidget->expand(index);
}

void LOG_DateWidget::on_updateBtn_clicked()
{
    mDateHash->updateTab(mTableName);
    updateWidget();
}

void LOG_DateWidget::querySlot(QString str)
{
    if(str.contains("date")) return;
    if(str.contains("time")) return;
    mQueryCmd = str;
}

/**
 * @brief 日志导出
 */
void LOG_DateWidget::on_exportBtn_clicked()
{
    LOG_ExportDlg dlg(this);
    dlg.set(mTableName, mHeadList, mTableTile, mQueryCmd);
    dlg.exec();
}
