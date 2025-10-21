/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "up_maindlg.h"
#include "ui_up_maindlg.h"

Up_MainDlg::Up_MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Up_MainDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("设备升级"));
    QTimer::singleShot(rand()%50,this,SLOT(initFunSLot()));
}

Up_MainDlg::~Up_MainDlg()
{
    delete ui;
}

void Up_MainDlg::initFunSLot()
{
    Up_DataPacket::bulid()->data;
    mIpsWid = new Up_IpsWid(ui->ipWid);
    mUpgradeWid = new UpgradeWid(ui->upWid);

    mIpsTabWid = new Up_IpsTabWid(ui->ipsTabWid);
    mOksTabWid = new Up_OksTabWid(ui->oksTabWid);
    mErrsTabWid = new Up_ErrsTabWid(ui->errsTabWid);
    mDevSelectWid = new Up_DevSelectWid(ui->devWid);
}
