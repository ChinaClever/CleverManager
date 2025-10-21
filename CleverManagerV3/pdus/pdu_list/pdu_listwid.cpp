/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "pdu_listwid.h"
#include "ui_pdu_listwid.h"

Pdu_ListWid::Pdu_ListWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_ListWid)
{
    ui->setupUi(this);

    //ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setSortingEnabled(true);
    QTimer::singleShot(2500,this,SLOT(on_btn_clicked()));
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(selectedItemSlot(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this,SLOT(clickedSlot(QModelIndex)));
    setHeader();

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Pdu_ListWid::~Pdu_ListWid()
{
    delete ui;
}


/**
 * @brief 设置头的标题
 * @param headers
 */
void Pdu_ListWid::setHeader()
{
    QStringList head;
    head << tr("PDU设备列表") << "";
    ui->treeWidget->setHeaderLabels(head);
    ui->treeWidget->headerItem()->setTextAlignment(0,Qt::AlignHCenter|Qt::AlignVCenter);
    ui->treeWidget->setStyleSheet(tr("font:12px \"微软雅黑\" "));
    ui->treeWidget->setHeaderLabels(head);
}

void Pdu_ListWid::initWidget()
{
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1); //设置列数
}


int Pdu_ListWid::getDevList(QTreeWidgetItem *item, sCabPacket *cap)
{
    QStringList list;
    if(cap->en) {
        sDataPacket *p = cap->m;
        if(p) {
            if(p->en) {
                list << tr("主路") + QString(": %1: %2").arg(p->net.ip).arg(p->dev_num) ;
            }
        }

        p = cap->s;
        if(p) {
            if(p->en) {
                list << tr("备路") + QString(": %1: %2").arg(p->net.ip).arg(p->dev_num) ;
            }
        }

        if(list.size()) {
            QTreeWidgetItem *subItem = new QTreeWidgetItem(item, list);
            item->addChild(subItem);
        }
    }

    return list.size();
}


int Pdu_ListWid::getCabList(QTreeWidgetItem *item, const QString &room)
{
    QStringList cabs = DbCabinetList::get()->list(room);
    for(int i=0; i<cabs.size(); ++i)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, tr("机柜: ")+cabs.at(i));
        sCabPacket *cap = Dp_CabPackets::bulid()->get(room, cabs.at(i));
        if(cap) {
            int ret = getDevList(subItem, cap);
            if(ret > 0) item->addChild(subItem);
        }
    }
    return cabs.size();
}


/**
 * @brief 显示设备种类
 */
void Pdu_ListWid::getRoomList()
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

void Pdu_ListWid::updateWidget()
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
sDataPacket *Pdu_ListWid::getPacket(const QString &str)
{
    sDataPacket *p = nullptr;
    QStringList strlist = str.split(": ",QString::SkipEmptyParts);
    if(strlist.size() == 3) {
        QString ip = strlist.at(1);
        QString dev_num = strlist.at(2);
        p = Dp_PduPackets::bulid()->get(ip, dev_num);
    }

    return p;
}


bool Pdu_ListWid::selectedItemSlot(QTreeWidgetItem *item,int column)
{
    if(item->parent())
    {
        QString str = item->text(column);
        sDataPacket *p = getPacket(str);
        if(p) emit selectedSig(p);
    }

    return true;
}


/**
 * @brief 实现单击点开操作
 * @param index
 */
void Pdu_ListWid::clickedSlot(QModelIndex index)
{
    bool ret = ui->treeWidget->isExpanded(index);
    if(ret)
        ui->treeWidget->collapse(index);
    else
        ui->treeWidget->expand(index);
}

void Pdu_ListWid::on_btn_clicked()
{
    updateWidget();
}
