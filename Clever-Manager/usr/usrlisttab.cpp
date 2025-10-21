/*
 * usrlisttab.cpp
 *  显示所有用户列表信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "usrlisttab.h"
#include <QHeaderView>
#include<qtablewidget.h>
extern bool usr_land_jur(void);

UsrListTab::UsrListTab(QWidget *parent) : QWidget(parent)
{
    m_tab = new QTableWidget(this);
    layout = new QGridLayout(this);
    layout->addWidget(m_tab);
    layout->setContentsMargins(0,0,0,0);

    m_tab->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑表格
    m_tab->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置表格为选择整行
    connect(m_tab,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(doubleClickedSlot(QTableWidgetItem *)));

    m_usrMap = &g_usrMap;
    upListTab();
}

UsrListTab::~UsrListTab()
{

}

/**
 * @brief 初始化窗口
 */
void UsrListTab::initWidget(void)
{
    m_tab->clear();
    m_tab->setRowCount(0);        //设置行数/
    QStringList header;
    header<<tr("用户")<<tr("权限")<<tr("电话")<<tr("邮箱")<<tr("备注");

     m_tab->verticalHeader()->setHighlightSections(false);

    m_tab->setColumnCount(header.size());
    m_tab->setHorizontalHeaderLabels(header);
    m_tab->setColumnWidth(0,200);
    m_tab->setColumnWidth(1,200);
    m_tab->setColumnWidth(2,200);
    m_tab->setColumnWidth(3,280);
    m_tab->horizontalHeader()->setStretchLastSection(true);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void UsrListTab::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    m_tab->setItem(row, column, item);
    if(column < m_tab->columnCount()-1)
        item->setTextAlignment(Qt::AlignCenter);
}

void UsrListTab::addRowContent(QStringList &list)
{
    int row = m_tab->rowCount();
    m_tab->insertRow(row);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
}

/**
 * 功能：显示用户列表
 */
void UsrListTab::upListTab(void)
{
    UsrClass *usr = new UsrClass;
    UsrMaptype *UsrMap = usr->getUsrMap();

    initWidget();
    UsrMaptype::iterator it; //遍历map
    for ( it = UsrMap->begin(); it != UsrMap->end(); ++it )
    {
        QStringList list;

        list << it.key();
        if(it.value()->jurisdiction)
            list << tr("管理员");
        else
            list << tr("访客");

        list << it.value()->telephone;
        list << it.value()->email;
        list << it.value()->remarks;

        addRowContent(list);
    }

    delete usr;
}

/**
 * 功能：获取选中用户名
 */
bool UsrListTab::getSelectUsr(QString &name)
{
    QList<QTableWidgetItem*>items = m_tab->selectedItems();
    int count=items.count();

    for(int i=0;i<count;i++)
    {
        //        int row = m_tab->row(items.at(i));//获取选中的行
        QTableWidgetItem*item = items.at(i);
        name = item->text();//获取内容
        if(!name.isEmpty())
            return true;
    }

    return false;
}

bool UsrListTab::editSelectUsr(void)
{
    QString name;
    bool ret = getSelectUsr(name);
    if(ret)
    {
        EditUsrDlg *dlg = new EditUsrDlg(this);
        dlg->setUsrName(name);
        dlg->exec();

        upListTab();
        delete dlg;
    }
    return ret;
}

void UsrListTab::doubleClickedSlot(QTableWidgetItem *)
{
    bool ret = usr_land_jur();
    if(ret)
        editSelectUsr();
}
