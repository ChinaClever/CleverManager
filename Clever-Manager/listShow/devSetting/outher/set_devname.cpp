<<<<<<< HEAD:Clever-Manager/listShow/devSetting/outher/set_devname.cpp
﻿/*
 * set_devname.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "set_devname.h"

SET_DevName::SET_DevName(QWidget *parent) : QWidget(parent)
{
    m_tableWidget = new QTableWidget(this);
    initLayout();

    m_tableWidget->setSortingEnabled(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(getItemSlot(QTableWidgetItem *)));
}

SET_DevName::~SET_DevName()
{

}

void SET_DevName::initLayout(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备名称设置"),this);
    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(m_tableWidget);

    QGridLayout *pLayout  = new QGridLayout(this);
    pLayout->addWidget(groupBox);
}


/**
 * @brief 初始化窗口
 */
void SET_DevName::initWidget(void)
{
    m_tableWidget->clear();
    m_tableWidget->setColumnCount(4);    //设置列数
    m_tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("组名")<<tr("设备IP")<<tr("副机")<<tr("设备名");
    m_tableWidget->setHorizontalHeaderLabels(header);
}


/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void SET_DevName::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignHCenter);
    m_tableWidget->setItem(row, column, item);
}

void SET_DevName::addRowContent(QStringList &list)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
}

void SET_DevName::clearWidget(void)
{
    initWidget();
    m_devList.updateLine(); // 更新在线列表
}

/**
 * @brief 更新副机
 */
void SET_DevName::updateSlave(QString &ip,short num)
{
    DL_ListMap listMap;
    QString group;
    listMap.findGroup(ip,group);

    QStringList list;
    list << group << ip;

    if(num==0)
        list<< tr("主机");
    else if(num>0)
        list<< tr("副机：%1").arg(num);
    else
        list<< tr("本机");

    DataRead dataRead;
    QString devName;
    dataRead.readSlaveName(ip,num,devName);
    list<< devName;

    addRowContent(list);
}

/**
 * @brief 按IP更新
 */
void SET_DevName::updateIp(QString &ip)
{
    DataRead dataRead;

    bool mode = dataRead.readDevMode(ip);
    if(mode)
    {
        int num = dataRead.readSlaveNum(ip);
        for(int k=0; k<num; ++k)
            updateSlave(ip,k);
    }
    else
        updateSlave(ip,-1);

}

/**
 * @brief 按组更新
 * @param group
 */
void SET_DevName::updateGroup(QString &group)
{
    QList<dl_devStruct> devList;
    m_devList.listDev(group,devList);

    for(int j=0; j<devList.size(); ++j)
        updateIp(devList[j].IP);
}

/**
 * @brief 显示所有
 */
void SET_DevName::updateWidget(void)
{
    initWidget();
    m_devList.updateLine(); // 更新在线列表

    QStringList groupList;
    m_devList.listGroup(groupList);

    for(int i=0; i<groupList.size(); ++i)
        updateGroup(groupList[i]);

    m_devList.clearList(); //释放空间
}

short SET_DevName::getDevNum(QString &str)
{
    short num = 0;

    if(str.length() > 3)
        num = str.mid(3).toInt();

    return num;
}

/**
  * @brief 修改设备名
  * @param ip
  * @param name
  */
QString SET_DevName::setDevName(QString &ip, QString &str, QString &name)
{
    bool ok;
    QString newName = QInputDialog::getText(this,tr("设备名修改"),
                                            tr("请输入设备名："),QLineEdit::Normal,name,&ok);
    if(ok && !newName.isEmpty())
    {
        short num = getDevNum(str);
        if(newName.size() < 24)
        {
            SET_CmdClass cmd;
            cmd.setDevName(ip,num,newName);

            /*增加日志信息*/
            s_RecordLog log;
            log.title = tr("设备配置");
            log.operation = tr("修改设备名");
            log.msg = tr("设备IP：%1 设备名%2 改为：%3").arg(ip).arg(name).arg(newName);
            sql_insert_record(log);
        }
        else
            CriticalMsgBox box(this,tr("输入设备名过长!"));
    }

    return newName;
}

void SET_DevName::getItemSlot(QTableWidgetItem *)
{
    QList<QTableWidgetItem*>items=m_tableWidget->selectedItems();

    QString ip = items.at(1)->text();
    QString str = items.at(2)->text();
    QString name = items.at(3)->text();

    QString newName = setDevName(ip,str,name);
    if(!newName.isEmpty())
        items.at(3)->setText(newName);
}
=======
﻿/*
 * set_devname.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "set_devname.h"

SET_DevName::SET_DevName(QWidget *parent) : QWidget(parent)
{
    m_tableWidget = new QTableWidget(this);
    initLayout();

    m_tableWidget->setSortingEnabled(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(getItemSlot(QTableWidgetItem *)));
}

SET_DevName::~SET_DevName()
{

}

void SET_DevName::initLayout(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备名称设置"),this);
    QGridLayout *layout = new QGridLayout(groupBox);
    layout->addWidget(m_tableWidget);

    QGridLayout *pLayout  = new QGridLayout(this);
    pLayout->addWidget(groupBox);
}


/**
 * @brief 初始化窗口
 */
void SET_DevName::initWidget(void)
{
    m_tableWidget->clear();
    m_tableWidget->setColumnCount(4);    //设置列数
    m_tableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("组名")<<tr("设备IP")<<tr("副机")<<tr("设备名");
    m_tableWidget->setHorizontalHeaderLabels(header);
}


/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void SET_DevName::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    item->setTextAlignment(Qt::AlignHCenter);
    m_tableWidget->setItem(row, column, item);
}

void SET_DevName::addRowContent(QStringList &list)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
}

void SET_DevName::clearWidget(void)
{
    initWidget();
    m_devList.updateLine(); // 更新在线列表
}

/**
 * @brief 更新副机
 */
void SET_DevName::updateSlave(QString &ip,short num)
{
    DL_ListMap listMap;
    QString group;
    listMap.findGroup(ip,group);

    QStringList list;
    list << group << ip;

    if(num==0)
        list<< tr("主机");
    else if(num>0)
        list<< tr("副机：%1").arg(num);
    else
        list<< tr("本机");

    DataRead dataRead;
    QString devName;
    dataRead.readSlaveName(ip,num,devName);
    list<< devName;

    addRowContent(list);
}

/**
 * @brief 按IP更新
 */
void SET_DevName::updateIp(QString &ip)
{
    DataRead dataRead;

    bool mode = dataRead.readDevMode(ip);
    if(mode)
    {
        int num = dataRead.readSlaveNum(ip);
        for(int k=0; k<num; ++k)
            updateSlave(ip,k);
    }
    else
        updateSlave(ip,-1);

}

/**
 * @brief 按组更新
 * @param group
 */
void SET_DevName::updateGroup(QString &group)
{
    QList<dl_devStruct> devList;
    m_devList.listDev(group,devList);

    for(int j=0; j<devList.size(); ++j)
        updateIp(devList[j].IP);
}

/**
 * @brief 显示所有
 */
void SET_DevName::updateWidget(void)
{
    initWidget();
    m_devList.updateLine(); // 更新在线列表

    QStringList groupList;
    m_devList.listGroup(groupList);

    for(int i=0; i<groupList.size(); ++i)
        updateGroup(groupList[i]);

    m_devList.clearList(); //释放空间
}

short SET_DevName::getDevNum(QString &str)
{
    short num = 0;

    if(str.length() > 3)
        num = str.mid(3).toInt();

    return num;
}

/**
  * @brief 修改设备名
  * @param ip
  * @param name
  */
QString SET_DevName::setDevName(QString &ip, QString &str, QString &name)
{
    bool ok;
    QString newName = QInputDialog::getText(this,tr("设备名修改"),
                                            tr("请输入设备名："),QLineEdit::Normal,name,&ok);
    if(ok && !newName.isEmpty())
    {
        short num = getDevNum(str);
        if(newName.size() < 24)
        {
            SET_CmdClass cmd;
            cmd.setDevName(ip,num,newName);

            /*增加日志信息*/
            s_RecordLog log;
            log.title = tr("设备配置");
            log.operation = tr("修改设备名");
            log.msg = tr("设备IP：%1 设备名%2 改为：%3").arg(ip).arg(name).arg(newName);
            sql_insert_record(log);
        }
        else
            CriticalMsgBox box(this,tr("输入设备名过长!"));
    }

    return newName;
}

void SET_DevName::getItemSlot(QTableWidgetItem *)
{
    QList<QTableWidgetItem*>items=m_tableWidget->selectedItems();

    QString ip = items.at(1)->text();
    QString str = items.at(2)->text();
    QString name = items.at(3)->text();

    QString newName = setDevName(ip,str,name);
    if(!newName.isEmpty())
        items.at(3)->setText(newName);
}
>>>>>>> develop_beijing:Clever-Manager/listShow/devSetting/set_devname.cpp
