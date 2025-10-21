/*
 * up_tcpwidget.cpp
 * TCP升级窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "up_tcpdialog.h"
#include "ui_up_tcpdialog.h"
#include "common.h"

UP_TcpDialog::UP_TcpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UP_TcpDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("设备升级"));
    QColor maincolor(232,240,250);
    set_background_color(this,maincolor);

    mTcpWidget = new UP_TcpWidget(ui->widget);
    connect(mTcpWidget, SIGNAL(quitSig()),this, SLOT(close()));
}

UP_TcpDialog::~UP_TcpDialog()
{
    delete ui;
}
