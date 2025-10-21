/*
 * ls_mdevtable.cpp
 * 管理界面，设备列表显示界面
 *  根据设备组，来显示相应的设备列表
 *  显示设备IP、主机名、级联数量
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mdevtable.h"
#include "pdu/data/hash/read/pduhashread.h"

LS_MdevTable::LS_MdevTable(QWidget *parent) : QWidget(parent)
{
    initTableWidget();
}

LS_MdevTable::~LS_MdevTable()
{

}


/**
 * @brief 初始化表格
 */
void LS_MdevTable::initTableWidget(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备列表"),this);
    mTableWidget = new QTableWidget(groupBox);

    QGridLayout *groupLayout = new QGridLayout(groupBox);
    groupLayout->addWidget(mTableWidget);

    layout = new QGridLayout(this);
    layout->addWidget(groupBox);

    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //    mTableWidget->setSortingEnabled(true);

    connect(mTableWidget,  SIGNAL(itemClicked(QTableWidgetItem*)),
            this, SLOT(selectSlot(QTableWidgetItem *)));
}

/**
 * @brief 初始化窗口
 */
void LS_MdevTable::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("设备IP")<<tr("主机名") << tr("级联数量");
    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->setHorizontalHeaderLabels(header);
    mTableWidget->horizontalHeader()->setStretchLastSection(true);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void LS_MdevTable::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignCenter);
    mTableWidget->setItem(row, column, item);
}


void LS_MdevTable::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
}

/**
 * @brief 显示设备信息
 * @param ip
 */
void LS_MdevTable::showDev(QString &ip)
{
    PduDevHash *devHash = pdu_hd_get(ip);
    int num = devHash->size(); //级联数量

    PduDataPacket *pduData = devHash->getMaster(); //  获取主机的设备信息
    QString name = pduData->info->type->name->get();

    QStringList listStr;
    listStr << ip;
    listStr << name;
    listStr << QString::number(num);

    addRowContent(listStr);
}

/**
 * @brief 更新窗口
 */
void LS_MdevTable::updateWidget(void)
{
    initWidget();

    QStringList devList;
    mGroupList->listDev(mGroup, devList);
    for(int i=0; i<devList.size(); ++i)
        showDev(devList[i]);

    if(devList.size() > 0)
    {
        mTableWidget->setCurrentCell(0,0);
        selectSlot(mTableWidget->currentItem());
    }
}


/**
 * @brief 更新窗口
 * @param group
 * @return
 */
void LS_MdevTable::updateWidget(QString &group)
{
    mIP.clear();
    mGroup = group;
    updateWidget();
}

/**
 * @brief 获取当前选正的IP
 * @param IP
 * @return
 */
bool LS_MdevTable::selectIP(QString &IP, QString &name)
{
    bool ret = true;

    int row = mTableWidget->currentItem()->row();//当前选中行
    if(row>=0)
    {
        IP = mTableWidget->item(row, 0)->text();//获取某一格内容
        name = mTableWidget->item(row, 1)->text();
    }
    else
        ret = false;

    return ret;
}

/**
 * @brief 选择设备
 * @param dev
 * @return
 */
bool LS_MdevTable::getSelectDev(QString &ip,QString &name)
{
    bool ret = true;

    if(!mIP.isEmpty())
    {
        ip = mIP;
        name = mName;
    }
    else
        ret = false;

    return ret;
}

/**
 * @brief 获取设备IP
 * @param ip
 * @return
 */
bool LS_MdevTable::getIP(QString &ip)
{
    bool ret = true;

    if(!mIP.isEmpty())
        ip = mIP;
    else
        ret = false;

    return ret;
}

/**
 * @brief 获取组名
 * @param group
 */
bool LS_MdevTable::getGroup(QString &group)
{
    bool ret = true;

    if(mGroup.isEmpty())
        ret = false;
    else
        group = mGroup;

    return ret;
}

/**
 * @brief 选择设备
 * @param it
 */
void LS_MdevTable::selectSlot(QTableWidgetItem *)
{
    QString str, name;
    bool ret = selectIP(str, name);
    if(ret)
    {
        if(str != mIP)
        {
            mIP = str;
            mName = name;
            emit selectSig(mIP);
        }
    }
}
