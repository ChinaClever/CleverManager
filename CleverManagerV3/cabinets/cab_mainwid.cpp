/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_mainwid.h"
#include "ui_cab_mainwid.h"

Cab_MainWid::Cab_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cab_MainWid)
{
    ui->setupUi(this);

    mTree = new Cab_TreeWid(ui->treeWid);
    mCab = new Cab_CabinetWid(ui->widget);
    connect(mTree, SIGNAL(selectedSig(int)), mCab, SLOT(selectSlot(int)));
    connect(this, SIGNAL(selectedSig(int)), mCab, SLOT(selectSlot(int)));
}

Cab_MainWid::~Cab_MainWid()
{
    delete ui;
}
