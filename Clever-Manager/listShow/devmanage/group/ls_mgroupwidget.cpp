/*
 * ls_mgroupwidget.cpp
 * 设备组管理界面
 *  1、提供组更新响应函数
 *  2、实现增加、删除、修改、移动分组功能
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_mgroupwidget.h"
#include <QInputDialog>
#include "common/msgBox/msgbox.h"
#include "pdu/dev/devmanage/pdudmmanage.h"
#include "common.h"

ls_MGroupWidget::ls_MGroupWidget(QWidget *parent) : QWidget(parent)
{
    splitter=new QSplitter(Qt::Vertical,this);
    initWidget();

    layout = new QGridLayout(this);
    layout->addWidget(splitter);
    groupBox_background_icon(this);
}

/**
 * @brief 初始化表格
 */
void ls_MGroupWidget::initTable(void)
{
    mTable = new LS_MgroupList(splitter);
    connect(mTable,SIGNAL(selectSig(QString &)),this,SIGNAL(selectSig(QString &)));
}

/**
 * @brief 初始化Btn按键
 */
void ls_MGroupWidget::initBtn(void)
{
    mBtn = new LS_MGroupBtn(splitter);

    connect(mBtn,SIGNAL(addSig()),this,SLOT(addSlot()));
    connect(mBtn,SIGNAL(delSig()),this,SLOT(delSlot()));
    connect(mBtn,SIGNAL(editSig()),this,SLOT(editSlot()));
    connect(mBtn,SIGNAL(moveSig()),this,SLOT(moveSlot()));
}

/**
 * @brief 初始化窗口
 */
void ls_MGroupWidget::initWidget(void)
{
    initTable();
    initBtn();

    splitter->setStretchFactor(0,7);
    splitter->setStretchFactor(1,3);
}

/**
 * @brief 组更新，此响应函数，是由设备种类 变化时发出的
 * @param type
 */
void ls_MGroupWidget::updateSlot(int type)
{
    mTable->updateWidget(type);
}

/**
 * @brief 组名是否存在
 * @param group
 * @return
 */
bool ls_MGroupWidget::isExist(QString &group)
{
    PduDGList *groupList = mTable->mGroupList;
    bool ret = groupList->containsGroup(group);
    if(ret)
        InfoMsgBox box(this,group+"组已存在!!! ");

    return ret;
}

/**
 * @brief 增加一个分组
 */
void ls_MGroupWidget::addSlot()
{
    bool isOK;
    QString group = QInputDialog::getText(this,tr("新建分组"),tr("请输入组名："),
                                          QLineEdit::Normal,"PDU",&isOK);
    if(isOK && !group.isEmpty())
    {
        isOK = isExist(group);
        if(!isOK)
        {
            PduDGList *groupList = mTable->mGroupList;
            groupList->insertGroup(group);
            mTable->updateWidget();

            ///==========
            ///
            ///

            //                qDebug() << group;
            //                /*增加日志信息*/
            //                 s_RecordLog log;
            //                 log.title = tr("设备管理");
            //                 log.operation = tr("新建组");
            //                 log.msg = tr("增加%1组").arg(group);
            //                 sql_insert_record(log);
        }
    }
}

/**
 * @brief 删除一个分组
 */
void ls_MGroupWidget::delSlot()
{
    QString group;
    int type = mTable->selectGroup(group);;
    if(type > 0)
    {
        QString str = "确定删除"+group +"组?";
        QuMsgBox box(this,str);
        bool ret = box.Exec();
        if(ret)
        {
            PduDGList *groupList = mTable->mGroupList;
            QStringList devList;
            groupList->listDev(group, devList);

            for(int i=0; i<devList.size(); ++i)
                pdu_dm_del(type, devList.at(i)); //  删除设备
            groupList->delGroup(group); // 删除
            mTable->updateWidget();

            //           /*增加日志信息*/
            //            s_RecordLog log;
            //            log.title = tr("设备管理");
            //            log.operation = tr("删除组");
            //            log.msg = tr("%1已删除").arg(group);
            //            sql_insert_record(log);
        }
    }
}


/**
 * @brief 组重命名
 * @param oldGroup
 * @param group
 */
void ls_MGroupWidget::renameGroup(QString &oldGroup,QString &group)
{
    PduDGList *groupList = mTable->mGroupList;

    QStringList devList;
    groupList->listDev(oldGroup, devList);
    for(int i=0; i<devList.size(); ++i)
        pdu_dm_renameGroup(devList[i], group); //  设备修改组名

    groupList->renameGroup(oldGroup, group);
}

void ls_MGroupWidget::moveGroup(QString &oldGroup, QString &group)
{
    renameGroup(oldGroup, group);

    PduDGList *groupList = mTable->mGroupList;
    groupList->insertGroup(oldGroup); // 保留旧组
}

/**
 * @brief 编辑分组
 */
void ls_MGroupWidget::editSlot()
{
    QString oldGroup, group;
    int type = mTable->selectGroup(oldGroup);
    if(type > 0)
    {
        bool isOK;
        QString str = tr("组%1改为：").arg(oldGroup);
        group = QInputDialog::getText(this,tr("编辑分组"),str,
                                      QLineEdit::Normal,group,&isOK);
        if(isOK && !group.isEmpty())
        {
            isOK = isExist(group);
            if(!isOK)
            {
                renameGroup(oldGroup, group);
                mTable->updateWidget(); // 更新表

                //                /*增加日志信息*/
                //                s_RecordLog log;
                //                log.title = tr("设备管理");
                //                log.operation = tr("组重命名");
                //                log.msg = tr("%1 %2").arg(str).arg(group);
                //               sql_insert_record(log);
            }
        }
    }
}

/**
 * @brief 移动组
 */
void ls_MGroupWidget::moveSlot()
{
    QString group;
    bool isOK = mTable->selectGroup(group);
    if(isOK)
    {
        PduDGList *groupDG = mTable->mGroupList;
        QStringList groupList;
        groupDG->listGroup(groupList); //获得所有组
        groupList.removeOne(group); //过滤自己

        if(groupList.size() > 0)
        {
            QString str = tr("组%1移动到：").arg(group);
            QString newGroup = QInputDialog::getItem(this,tr("移动分组"),str,groupList,0,false,&isOK);
            if(isOK && !newGroup.isEmpty())
            {
                moveGroup(group, newGroup);
                mTable->updateWidget(); // 更新表


                //                /*增加日志信息*/
                //                s_RecordLog log;
                //                log.title = tr("设备管理");
                //                log.operation = tr("组移动");
                //                log.msg = tr("%1 %2").arg(str).arg(group);
                //                sql_insert_record(log);
            }
        }
        else
            InfoMsgBox box(this,tr("仅有一个组不能移动！！"));
    }
}
