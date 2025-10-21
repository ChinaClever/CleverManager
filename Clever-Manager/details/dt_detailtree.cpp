/*
 * dt_detailtree.cpp
 * 设备明细列表树
 *  1、设备各类、分类显示
 *  2、设备分组显示
 *  3、设备列表显示
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_detailtree.h"
#include "pdu/dev/devType/pdudtname.h"


DT_DetailTree::DT_DetailTree(QWidget *parent) : QWidget(parent)
{
    mTreeWidget = new QTreeWidget(this); //树界面
//    mTreeWidget->setStyleSheet("background-color:yellow");
    mTreeWidget->setFrameStyle(QFrame::NoFrame);

  mTreeWidget->setStyleSheet("border:none");
    mTreeWidget->setSortingEnabled(true); // 排序
    connect(mTreeWidget, SIGNAL(clicked(QModelIndex)), this,SLOT(clickedSLot(QModelIndex)));

    mRoomID = -1;
    initWidget();
    initLayout();

    QStringList list;
    list << tr("设备信息列表") <<"";
    setHeaderLabels(list);
    connect(DbCabinetList::get(),SIGNAL(itemChanged(int,int)),this,SLOT(itemChangedSlot(int,int)));
//    mPduDg = new PduDGList();; // 设备分组链表类
}

DT_DetailTree::~DT_DetailTree()
{
//    delete mPduDg;
}

/**
 * @brief 初始化布局
 */
void DT_DetailTree::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(mTreeWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(10, 8, 0, 0);
}

/**
 * @brief 设置头的标题
 * @param headers
 */
void DT_DetailTree::setHeaderLabels(QStringList &headers)
{
    mTreeWidget->setHeaderLabels(headers);

}


/**
 * @brief 初始化窗口
 */
void DT_DetailTree::initWidget(void)
{
    emit clearSig(); //清除信号
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(1); //设置列数
}


/**
 * @brief 创建表格
 * @param item
 * @param ip
 */
void DT_DetailTree::createTable(QTreeWidgetItem *item, QString &ip)
{
    DT_DetailTable *table = new DT_DetailTable(this);
    table->updateWidget(ip);
    connect(table,SIGNAL(selectSig(QString,short)),this,SIGNAL(selectSig(QString,short)));
    connect(this,SIGNAL(clearSig()), table,SLOT(closeSlot())); //清除表格，释放内存

    QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
    mTreeWidget->setItemWidget(subItem,0,table);
    item->addChild(subItem);
}

/**
 * @brief 根据设备各类，创建子项目
 * @param type
 * @param item
 * @return
 */
int DT_DetailTree::getDevList(int type, QTreeWidgetItem *item)
{
    PduDevList pduDevList;

    QStringList devList;
    pduDevList.getLine(type,devList);

    for(int j=0; j<devList.size(); ++j)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, devList[j]);

        QString path = icon_get_devIcon();
        subItem->setIcon(0,QIcon(path));

        createTable(subItem,devList[j]);
        item->addChild(subItem);
    }

    return devList.size();
}

/**
 * @brief 显示设备种类
 */
void DT_DetailTree::devTypeWidget(void)
{    
    PduDTName pduDt; // 设备类型名称类

    QList<int> pduDtList;
    pduDt.list(pduDtList);

    QList<QTreeWidgetItem *> rootList;
    for(int d=0; d<pduDtList.size(); ++d) // 设备种类
    {
        int type = pduDtList.at(d);

        QString typeName;
        bool ret = pduDt.getName(type,typeName); // 获取设备名
        if(ret)
        {
            QTreeWidgetItem *devTypeItem = new QTreeWidgetItem(mTreeWidget);
            devTypeItem->setText(0, typeName);

            QString path = icon_get_devType();
            devTypeItem->setIcon(0,QIcon(path));

            rootList.append(devTypeItem);
//            int rtn = devGroupWidget(type, devTypeItem); // 不使用分组功能
            int rtn = getDevList(type, devTypeItem);
            if(rtn == 0) // 没有分组时，不显示设备种类
            {
                rootList.removeLast();
                delete devTypeItem;
            }
        }
    }

    mTreeWidget->insertTopLevelItems(0,rootList);  //将结点插入部件中
}

/**
 * @brief 显示设备分组 分组功能未使用
 * @param type
 * @param item
 */
int DT_DetailTree::devGroupWidget(int type, QTreeWidgetItem *item)
{
    mPduDg->startLineGroup(type); // 在线链表

    QStringList groupList;
    mPduDg->listGroup(groupList);
    for(int i=0; i<groupList.size(); ++i)
    {
        QTreeWidgetItem *groupItem = new QTreeWidgetItem(item);
        groupItem->setText(0, groupList[i]);

        QString path = icon_get_group();
        groupItem->setIcon(0,QIcon(path));

        item->addChild(groupItem);
        int ret = devListWidget(groupList[i], groupItem);
        if(ret == 0) // 没有子设备时，不显示组名
        {
            item->removeChild(item);
            groupList.removeAt(i);
        }
    }

    return groupList.size();
}

/**
 * @brief 根据分组来获取设备信息   分组功能未使用
 * @param group
 * @param item
 * @return
 */
int DT_DetailTree::devListWidget(QString &group, QTreeWidgetItem *item)
{
    QStringList devList;
    mPduDg->listDev(group,devList);

    for(int j=0; j<devList.size(); ++j)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, devList[j]);

        QString path = icon_get_devIcon();
        subItem->setIcon(0,QIcon(path));

        createTable(subItem,devList[j]);
        item->addChild(subItem);
    }

    return devList.size();
}


/**
 * @brief 创建机柜中的设备列表
 * @param item
 * @param ipList ip列表
 * @param numLIs 设备号列表
 */
void DT_DetailTree::createCabTable(QTreeWidgetItem *item, QStringList &ipList, QList<int> &numList)
{
    DT_DetailTable *table = new DT_DetailTable(this);
    table->updateWidget(mRoomID, ipList, numList);

    connect(table,SIGNAL(selectSig(QString,short)),this,SIGNAL(selectSig(QString,short)));
    connect(this,SIGNAL(clearSig()), table,SLOT(closeSlot())); //清除表格，释放内存

    QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
    mTreeWidget->setItemWidget(subItem,0,table);
    item->addChild(subItem);
}

/**
 * @brief 根据机柜，列出设备列表
 */
int DT_DetailTree::getCabDevList(const CabinetItem& cItem, QTreeWidgetItem *item)
{
    QList<int> numLIst;
    QStringList devList;

    QString ip =  cItem.main_ip;
    if(!ip.isEmpty()) {
        devList << ip;
        numLIst << cItem.main_num;
    }

    ip =  cItem.spare_ip;
    if(!ip.isEmpty()) {
        devList << ip;
        numLIst << cItem.spare_num;
    }
    createCabTable(item,devList, numLIst);

    return devList.size();
}

/**
 * @brief 根据机房键值，获取机柜列表
 * @param id
 */
void DT_DetailTree::getCabList(int id)
{
    try{
        QList<QTreeWidgetItem *> rootList;
        QVector<CabinetItem> cItems = DbCabinetList::get()->selItemsByRoomId(id); //pdu列表
        foreach(const CabinetItem& cItem,cItems)
        {
            QTreeWidgetItem *devTypeItem = new QTreeWidgetItem(mTreeWidget);
            devTypeItem->setText(0, cItem.name);
            QString path = ":/topology/topology/1.png"; //icon_get_devType();
            devTypeItem->setIcon(0,QIcon(path));

            rootList.append(devTypeItem);
            int rtn = getCabDevList(cItem, devTypeItem);  //列出当前机柜列表
            if(rtn == 0) //空
            {
                rootList.removeLast();
                delete devTypeItem;
            }
        }
        mTreeWidget->insertTopLevelItems(0,rootList);  //将结点插入部件中
    }catch(const SqlExecption& exception){
        qDebug() << exception.msg() << exception.type();
    }
}


/**
 * @brief 更新列表
 * @param expand
 * @return
 */
bool DT_DetailTree::updateWidget(bool expand)
{
    initWidget();
    if(mRoomID > 0) // 更新机房中的机柜
        getCabList(mRoomID);
    else    // 更新所有设备
        devTypeWidget();

    if(expand)
        mTreeWidget->expandAll(); //全部展开

    return expand;
}

/**
 * @brief 初始化机房信息
 * @param id 0表示所有，>1 机房键值
 * @param name
 */
void DT_DetailTree::initData(int id, QString &name)
{
    QStringList list;
    if(id > 0)
        list << tr("机房:") + name;
    else
        list <<  tr("所有在线设备");
     setHeaderLabels(list);

     mRoomID = id;
}

/**
 * @brief 查找设备
 * @param
 */
void DT_DetailTree::seekDev(QString &dev)
{
    QTreeWidgetItemIterator it(mTreeWidget);

    while (*it)
    {
        if((*it)->text(0) == dev)
        {
            mTreeWidget->collapseAll(); //全部关闭
            mTreeWidget->setCurrentIndex(mTreeWidget->rootIndex());
            mTreeWidget->setCurrentItem((*it));

            QModelIndex index = mTreeWidget->currentIndex();
            mTreeWidget->expand(index);
            break;
        }
        ++it;
    }
}

/**
 * @brief 实现单击点开操作
 * @param index
 */
void DT_DetailTree::clickedSLot(QModelIndex index)
{
    bool ret = mTreeWidget->isExpanded(index);
    if(ret)
        mTreeWidget->collapse(index);
    else
        mTreeWidget->expand(index);
}


/**
 * @brief 根据机柜ID来验证，是否是这一机房
 * @param id
 * @return true 表示这一机房有机柜修改信息
 */
bool DT_DetailTree::getCabinetById(int id)
{
    bool ret = false;
    QVector<CabinetItem> items =  DbCabinetList::get()->findItemById(id); // 获取所有机房列表
    if(items.size() == 1) {
        CabinetItem it = items.first();
        if(it.room_id == mRoomID)
            ret = true;
    }

    return ret;
}

/**
 * @brief 当机柜发生变化时，刷新列表
 * @param id 机柜键值
 */
void DT_DetailTree::itemChangedSlot(int id,int)
{
    if(mRoomID>0){
        bool ret = getCabinetById(id);
        if(ret) {
            updateWidget(false);
        }
    }
}
