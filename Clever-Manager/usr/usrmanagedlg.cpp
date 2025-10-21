/*
 * usrmanagedlg.cpp
 *  用户管理界面
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "usrmanagedlg.h"
#include "sql/records/db_sqlquery.h"
#include<qlabel.h>
extern QString usr_land_name(void);
extern bool usr_land_jur(void);

UsrManageDlg::UsrManageDlg(QWidget *parent) : QWidget(parent)
{
    initFun();
    btnBlue_style_sheet(this);
}

UsrManageDlg::~UsrManageDlg()
{

}


/**
 * @brief 窗口初始化
 */
void UsrManageDlg::initWidget(void)
{
    groupBox = new QWidget(this);//tr("用户设置"),
    //    groupBox->setStyleSheet("border:none");

    m_usrList = new UsrListTab(groupBox);
    set_background_color(m_usrList, QColor(255,255,255));

    m_newBtn = new QPushButton(tr("新增用户"),groupBox); //新增按键
    m_newBtn->setFont(QFont("微软雅黑",12));
    m_newBtn->setMaximumWidth(120);
    m_newBtn->setMinimumHeight(30);
    connect(m_newBtn,SIGNAL(clicked()),this,SLOT(addBtnSlot()));

    m_editBtn = new QPushButton(tr("编辑用户"),groupBox); //编辑用户
    m_editBtn->setFont(QFont("微软雅黑",12));
    m_editBtn->setMaximumWidth(120);
    m_editBtn->setMinimumHeight(30);
    connect(m_editBtn,SIGNAL(clicked()),this,SLOT(editBtnSlot()));

    m_delBtn = new QPushButton(tr("删除用户"),groupBox); //删除用户
    m_delBtn->setFont(QFont("微软雅黑",12));
    m_delBtn->setMaximumWidth(120);
    m_delBtn->setMinimumHeight(30);
    connect(m_delBtn,SIGNAL(clicked()),this,SLOT(delBtnSlot()));
}

/**
 * @brief 布局
 */
void UsrManageDlg::initLayout(void)
{
    hLayout = new QHBoxLayout;
    hLayout->addWidget(m_newBtn);
    hLayout->addWidget(m_editBtn);
    hLayout->addWidget(m_delBtn);
    hLayout->setContentsMargins(0,13,0,0);

    QLabel *title=new QLabel(tr("  用户设置"),this);
    title->setStyleSheet("background-color:rgb(12,62,180)");
    QPalette  pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    title->setPalette(pal);
    title->setFixedSize(100,30);
    title->setFont(QFont("微软雅黑",12));
    QVBoxLayout *label_out=new QVBoxLayout;
    label_out->addWidget(title);
    label_out->setContentsMargins(0,0,0,0);
    label_out->setSpacing(0);

    QGridLayout *layout = new QGridLayout(groupBox);
    layout->setSpacing(0);
    layout->setContentsMargins(9,0,0,0);
    layout->addLayout(label_out,0,0);
    layout->addWidget(m_usrList,1,0);
    layout->addLayout(hLayout,2,0);

    m_layout = new QGridLayout(this);
//    m_layout->setSpacing(0);
//    m_layout->setContentsMargins(10,10,0,10);

    m_layout->addWidget(groupBox);
}


void UsrManageDlg::initFun(void)
{
    initWidget();
    initLayout();
}

/**
 * @brief 新增加用户
 */
void UsrManageDlg::addBtnSlot(void)
{
    bool ret = usr_land_jur();
    if(ret)
    {
        NewUsrDlg dlg(this);

        dlg.exec();
        m_usrList->upListTab();
    }
    else
        InfoMsgBox box(this,tr("您权限不够，无法操作"));
}

/**
 * @brief 编辑操作
 */
void UsrManageDlg::editBtnSlot(void)
{
    bool ret = usr_land_jur();
    if(ret)
        m_usrList->editSelectUsr();
    else
        InfoMsgBox box(this,tr("您权限不够，无法操作"));
}


bool UsrManageDlg::delCheck(QString &name)
{
    bool ret = usr_land_jur();
    if(ret)
    {
        QString usrName = usr_land_name();
        if(name == usrName)
        {
            InfoMsgBox box(this,tr("您无法删除自己账户"));
            ret = false;
        }
    }
    else
        InfoMsgBox box(this,tr("您权限不够，无法操作"));

    return ret;
}

/**
 * @brief 删除用户
 */
void UsrManageDlg::delBtnSlot(void)
{
    QString name;
    bool ret = m_usrList->getSelectUsr(name);
    if(ret)
    {
        ret = delCheck(name);
        if(ret == false)
            return ;

        QString str = tr("确定删除用户：")+ name;
        QuMsgBox box(this,str);
        bool ret = box.Exec();
        if(ret)
        {
            UsrClass usr;
            usr.delUsr(name);
            m_usrList->upListTab();

            /*增加日志信息*/
            sLandLog log;
            log.title = tr("删除");
            log.operation = tr("用户删除");
            log.msg = tr("删除用户:%1").arg(name);
            sql_insert_land(log);
        }
    }
}
