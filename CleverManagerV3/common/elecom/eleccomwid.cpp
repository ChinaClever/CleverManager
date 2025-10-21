/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "eleccomwid.h"
#include "ui_eleccomwid.h"

ElecComWid::ElecComWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ElecComWid)
{
    ui->setupUi(this);
    mSqlTableWid = new SqlTableWid(ui->widget);
}

ElecComWid::~ElecComWid()
{
    delete ui;
}

void ElecComWid::initWid(BasicSql *db,  SqlQueryBase *btn)
{
    ElecBtnBar *bar = new ElecBtnBar;
    bar->setDlg(btn);
    mSqlTableWid->initWid(db, bar);
}
