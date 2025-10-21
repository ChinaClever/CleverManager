/*
 * ls_treewidget.cpp
 *
 * 实现tree型结构显示
 *  1、设备种类显示
 *  2、分组显示
 *  3、设备IP显示
 *  4、级联设备显示
 *
 *  信号：双击时，会发出选择信号
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_treewidget.h"
#include "pdu/dev/devType/pdudtname.h"
#include "icon/iconpath.h"
#include "common.h"
#include "ipsort.h"
LS_treeWidget::LS_treeWidget(QWidget *parent) : QWidget(parent)
{
    //    lab = new QLabel(tr("设备："),this);
    //    lab->setFixedHeight(30);
    //    lab->setFont(QFont("微软雅黑",12));
    //    lab->setMaximumSize(50,30);

    edit = new QLineEdit(this);
    edit->setFixedHeight(30);
    edit->setFont(QFont("微软雅黑",12));
    edit->setMaximumHeight(30);
    edit->setStyleSheet("border:1px solid #CCE8CF;" "background-color:white"); //
    edit->setPlaceholderText(tr("请输入查找内容"));

    //    btn = new QPushButton(tr("查找"),this);
    btn =new QPushButton(QIcon(":/images/other/serach.png"),tr(""),this);
    btn->setFixedSize(30,30);
    btn->setStyleSheet("QPushButton{background-color:rgb(255,255,255); color:rgb(255, 255, 255);}"
                       "QPushButton:hover{background-color:rgb(91, 237, 238);color:rgb(0, 0, 0);}"
                       "QPushButton:pressed{background-color:rgb(143, 185, 181);color:rgb(0, 0, 0);}");
    //    btnBlue_style_sheet(btn);
    //    btn->setStyleSheet("background-color:transparent");

    connect(btn,SIGNAL(clicked()),this,SLOT(seekDev()));

    mTreeWidget = new QTreeWidget(this);
    mTreeWidget->setHeaderHidden(true);
    //mTreeWidget->setSortingEnabled(true);
    connect(mTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(selectedItemSlot(QTreeWidgetItem*,int)));
    connect(mTreeWidget, SIGNAL(clicked(QModelIndex)), this,SLOT(clickedSLot(QModelIndex)));

    mMode = -1;
    mDevList = new PduDGList();
    initLayout();
}

LS_treeWidget::~LS_treeWidget()
{
    delete mDevList;
}


void LS_treeWidget::initLayout(void)
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    //    hLayout->addWidget(lab);
    hLayout->addWidget(edit);
    hLayout->addWidget(btn);
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(mTreeWidget,0,0);
    layout->addLayout(hLayout,1,0);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 3, 0, 0);
}


/**
 * @brief 设置头的标题
 * @param headers
 */
void LS_treeWidget::setHeaderLabels(QStringList &headers)
{
    mTreeWidget->setHeaderLabels(headers);
}

void LS_treeWidget::initWidget(void)
{
    mTreeWidget->clear();
    mTreeWidget->setColumnCount(1); //设置列数
}

/**
  * @brief 更新设备列表
  * @param expand
  * @return
  */
bool LS_treeWidget::updateListDev(bool expand)
{
    mMode = PDU_DL_LIST;
    return updateWidget(expand);
}

/**
  * @brief 更新在线设备列表
  * @param expand
  * @return
  */
bool LS_treeWidget::updateline(bool expand)
{
    mMode = PDU_DL_LINE;
    return updateWidget(expand);
}

/**
  * @brief 更新离线设备列表
  * @param expand
  * @return
  */
bool LS_treeWidget::updateOffLine(bool expand)
{
    mMode = PDU_DL_OFF;
    return updateWidget(expand);
}

/**
  * @brief 报警设备列表
  * @param expand
  * @return
  */
bool LS_treeWidget::updateAlarm(bool expand)
{
    mMode = PDU_DL_ALARM;
    return updateWidget(expand);
}

/**
 * @brief 检查设备状态，获取图标
 */
QString LS_treeWidget::getDevStateIcon(PduDataPacket *data)
{
    QString path;
    int id = 0;

    if(data->offLine > 0)
    {
        if(data->state)
            id = 1; // 报警
    }
    else
        id = 2; // 离线
    icon_get_devStatus(id,path);

    return path;
}



/**
 * @brief 显示级联设备
 * @param item
 * @param ip
 */
int LS_treeWidget::createDevTable(QTreeWidgetItem *item, QString &ip)
{
    PduDLObject dlObj;
    QList<int> numList;

    switch (mMode)
    {
    case PDU_DL_LIST:
        dlObj.list(ip, numList);
        break;

    case PDU_DL_LINE:
        dlObj.line(ip, numList);
        break;

    case PDU_DL_OFF:
        dlObj.offLine(ip, numList);
        break;

    case PDU_DL_ALARM:
        dlObj.alarm(ip, numList);
        break;
    default:
        break;
    }

    for(int i=0; i<numList.size(); ++i)
    {
        int num = numList.at(i);
        PduDataPacket *data = pdu_hd_getPacket(ip, num);

        QStringList strList;
        //  strList << QString::number(num);
        QString name = data->info->type->name->get();

        QString str;
        if(num==0){
            str = tr(" 主机：");str += name;
        }
        else if(num > 0)
            str = tr("副机 %1：").arg(num);

        strList << str;

        QTreeWidgetItem *subItem = new QTreeWidgetItem(item, strList);
        QString path = getDevStateIcon(data);
        subItem->setIcon(0,QIcon(path));

        item->addChild(subItem);
    }

    return numList.size();
}


/**
 * @brief 根据设备种类，进行分组
 * @param type
 */
void LS_treeWidget::getDevListMode(int type, PduDevList &pduDevList, QStringList &devList)
{
    switch (mMode)
    {
    case PDU_DL_LIST:
        pduDevList.getList(type, devList);
        break;

    case PDU_DL_LINE:
        pduDevList.getLine(type, devList);
        break;

    case PDU_DL_OFF:
        pduDevList.getOff(type, devList);
        break;

    case PDU_DL_ALARM:
        pduDevList.getAlarm(type, devList);
        break;
    default:
        break;
    }
}


/**
 * @brief 根据设备各类，创建子项目
 * @param type
 * @param item
 * @return
 */
int LS_treeWidget::getDevList(int type, QTreeWidgetItem *item)
{
    PduDevList pduDevList;
    QStringList devList;
    getDevListMode(type,pduDevList,devList);
    ip_sort_method(devList); // 设备信息ip列表排序 2018-08-27
    for(int j = devList.size() - 1; j >= 0; --j)//为了和数据列表模块一样顺序，更改顺序输出2018-08-27
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, devList[j]);

        QString path = icon_get_devIcon();
        subItem->setIcon(0,QIcon(path));

        createDevTable(subItem,devList[j]);
        item->addChild(subItem);
    }

    return devList.size();
}

/**
 * @brief 显示设备种类
 */
void LS_treeWidget::devTypeWidget(void)
{
    PduDTName pduDt; // 设备类型名称类

    QList<int> pduDtList;
    pduDt.list(pduDtList);

    initWidget();
    QList<QTreeWidgetItem *> rootList;
    for(int d = pduDtList.size()-1; d>=0; --d) // 设备种类,为了和数据列表模块一样顺序，更改顺序输出2018-08-27
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
            //            int rtn = devGroupWidget(type, devTypeItem); // 分组管理 暂时还未启用这功能
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
 * @brief 根据设备种类，进行分组
 * @param type
 */
void LS_treeWidget::getGroup(int type)
{
    switch (mMode)
    {
    case PDU_DL_LIST:
        mDevList->startListGroup(type);
        break;

    case PDU_DL_LINE:
        mDevList->startLineGroup(type);
        break;

    case PDU_DL_OFF:
        mDevList->startOffLineGroup(type);
        break;

    case PDU_DL_ALARM:
        mDevList->startAlarmGroup(type);
        break;
    default:
        break;
    }
}

/**
 * @brief 显示设备分组
 * @param type
 * @param item
 */
int LS_treeWidget::devGroupWidget(int type, QTreeWidgetItem *item)
{
    int ret=0;
    getGroup(type);

    QStringList groupList;
    mDevList->listGroup(groupList);
    for(int i=0; i<groupList.size(); ++i)
    {
        QTreeWidgetItem *groupItem = new QTreeWidgetItem(item);
        groupItem->setText(0, groupList[i]);

        QString path = icon_get_group();
        groupItem->setIcon(0,QIcon(path));

        item->addChild(groupItem);
        ret = devListWidget(groupList[i], groupItem);
        if(ret == 0) // 没有子设备时，不显示组名
        {
            item->removeChild(item);
            groupList.removeAt(i);
        }
    }

    return groupList.size();
}

/**
 * @brief 显示同一组中的设备IP
 * @param group
 * @param item
 * @return
 */
int LS_treeWidget::devListWidget(QString &group, QTreeWidgetItem *item)
{
    QStringList devList;

    mDevList->listDev(group,devList);
    for(int j=0; j<devList.size(); ++j)
    {
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
        subItem->setText(0, devList[j]);

        QString path = icon_get_devIcon();
        subItem->setIcon(0,QIcon(path));
        item->addChild(subItem);

        createDevTable(subItem,devList[j]);
    }

    return devList.size();
}

/**
 * @brief 更新列表
 * @param expand
 * @return
 */
bool LS_treeWidget::updateWidget(bool expand)
{
    devTypeWidget();

    if(expand)
        mTreeWidget->expandAll(); //全部展开


    return expand;
}

/**
 * @brief 获取副机编号
 * @param str
 * @return
 */
short LS_treeWidget::getDevNum(QString &str)
{
    short num = -1;

    bool ret = str.contains(tr("主机")) || str.contains("Master Unit");
    if(ret)
        num = 0;
    else
    {
        ret = str.contains(tr("副机")) ;
        if(ret)
        {
            QStringList strlist = str.split("：",QString::SkipEmptyParts);
            num = strlist[0].mid(3).toInt();
        }

        ret = str.contains("Slave Unit");
        if(ret)
        {
            QStringList strlist = str.split(":",QString::SkipEmptyParts);
            num = strlist[0].mid(11,1).toInt();
        }
    }

    return num;
}


bool LS_treeWidget::selectedItemSlot(QTreeWidgetItem *item,int column)
{
    if(item->parent())
    {
        QString str = item->text(column);
        bool ret = isIPaddress(str);
        if(ret)  {
            mSelStr = str; // 获取设备IP
            emit selectedSig(mSelStr, 0);
        } else {
            short num = getDevNum(str);
            if(num >= 0)
            {
                str = item->parent()->text(0);
                ret = isIPaddress(str);
                if(ret) {
                    mSelStr = str;
                    emit selectedSig(mSelStr, num); // 发出信号
                }
            }
        }
    }

    return true;
}

/**
 * @brief 查找定位
 */
void LS_treeWidget::seekDev(void)
{
    QString dev = edit->text();
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

            bool ret = isIPaddress(dev);
            if(ret)
            {
                mSelStr = dev;
                emit selectedSig(mSelStr,0);
            }
            break;
        }
        ++it;
    }
}


/**
 * @brief 实现单击点开操作
 * @param index
 */
void LS_treeWidget::clickedSLot(QModelIndex index)
{
    bool ret = mTreeWidget->isExpanded(index);
    if(ret)
        mTreeWidget->collapse(index);
    else
        mTreeWidget->expand(index);
}



/**
  * @brief 是否离线
  * @param ip
  * @return
  */
bool LS_treeWidget::isContain(QString &ip)
{
    bool ret = false;

    QTreeWidgetItemIterator it(mTreeWidget);
    while (*it)
    {
        if((*it)->text(0) == ip)
        {
            ret = true;
            break;
        }
        ++it;
    }

    return ret;
}
