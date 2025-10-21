/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_treewid.h"
#include "ui_cab_treewid.h"
#include <QGridLayout>

Cab_TreeWid::Cab_TreeWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cab_TreeWid)
{
    ui->setupUi(this);

    ui->treeWidget->setSortingEnabled(true);
    QTimer::singleShot(2500,this,SLOT(on_btn_clicked()));
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(selectedItemSlot(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this,SLOT(clickedSlot(QModelIndex)));
    setHeader();

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Cab_TreeWid::~Cab_TreeWid()
{
    delete ui;
}



/**
 * @brief 设置头的标题
 * @param headers
 */
void Cab_TreeWid::setHeader()
{
    QStringList head;
    head << tr("机柜列表") << "";
    ui->treeWidget->setHeaderLabels(head);
    ui->treeWidget->headerItem()->setTextAlignment(0,Qt::AlignHCenter|Qt::AlignVCenter);
    ui->treeWidget->setStyleSheet(tr("font:12px \"微软雅黑\""));
}

void Cab_TreeWid::initWidget()
{
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1); //设置列数
}


int Cab_TreeWid::getCabList(QTreeWidgetItem *item, const QString &room)
{
    QStringList cabs = DbCabinetList::get()->list(room);
    for(int i=0; i<cabs.size(); ++i)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, tr("机柜: ")+cabs.at(i));
        item->addChild(subItem);
    }

    return cabs.size();
}


/**
 * @brief 显示设备种类
 */
void Cab_TreeWid::getRoomList()
{
    QList<QTreeWidgetItem *> rootList;
    QStringList list = DbRoomList::get()->list();
    for(int i=0; i<list.size(); ++i)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, tr("机房: ")+list.at(i));
        rootList.append(item);
        int rtn = getCabList(item, list.at(i));
        if(rtn == 0) { // 没有分组时，不显示设备种类
            rootList.removeLast();
            delete item;
        }
    }

    ui->treeWidget->insertTopLevelItems(0,rootList);  //将结点插入部件中
}

void Cab_TreeWid::updateWidget()
{
    initWidget();
    getRoomList();
    ui->treeWidget->expandAll(); //全部展开
}


/**
 * @brief 获取副机编号
 * @param str
 * @return
 */
int Cab_TreeWid::getId(const QString &room, const QString &cab)
{
    int ret = -1;
    QStringList rooms = room.split(": ",QString::SkipEmptyParts);
    QStringList cabs = cab.split(": ",QString::SkipEmptyParts);

    if((cabs.size() == 2) && (rooms.size() == 2)) {
        QString r = rooms.at(1);
        QString c = cabs.at(1);
        ret = DbCabinetList::get()->findId(r, c);
    }

    return ret;
}


bool Cab_TreeWid::selectedItemSlot(QTreeWidgetItem *item,int column)
{
    if(item->parent())
    {
        QString room = item->parent()->text(0);
        QString cab = item->text(column);
        int  id = getId(room, cab);
        if(id > 0 ) emit selectedSig(id);
    }

    return true;
}


/**
 * @brief 实现单击点开操作
 * @param index
 */
void Cab_TreeWid::clickedSlot(QModelIndex index)
{
    bool ret = ui->treeWidget->isExpanded(index);
    if(ret)
        ui->treeWidget->collapse(index);
    else
        ui->treeWidget->expand(index);
}

void Cab_TreeWid::on_btn_clicked()
{
    updateWidget();
}
