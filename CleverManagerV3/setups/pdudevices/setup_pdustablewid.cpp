/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "setup_pdustablewid.h"
#include "ui_setup_pdustablewid.h"
#include "setup_pdusbtnbar.h"

Setup_PdusTableWid::Setup_PdusTableWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_PdusTableWid)
{
    ui->setupUi(this);
    mSqlTableWid = new SqlTableWid(ui->widget);
}

Setup_PdusTableWid::~Setup_PdusTableWid()
{
    delete ui;
}


void Setup_PdusTableWid::initWid(BasicSql *db,  SqlQueryBase *btn)
{
    Setup_PdusBtnBar *bar = new Setup_PdusBtnBar;
    bar->setDlg(btn);
    mSqlTableWid->initWid(db, bar);
}
