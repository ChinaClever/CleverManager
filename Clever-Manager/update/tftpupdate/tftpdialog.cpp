/*
 * tftpdialog.cpp
 * tftp上传对话框
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tftpdialog.h"
#include "ui_tftpdialog.h"
#include "common.h"

TftpDialog::TftpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TftpDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("设备升级"));
    mTftpWidget = new TftpWidget(ui->widget);
    connect(mTftpWidget, SIGNAL(quitSig()),this, SLOT(close()));
    groupBox_background_icon(this);
}

TftpDialog::~TftpDialog()
{
    delete ui;
}

