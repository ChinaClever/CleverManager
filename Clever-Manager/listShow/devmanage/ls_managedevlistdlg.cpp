/*
 * ls_managedevlistdlg.cpp
 * 设备管理对话框
 *    设备管理界面入口类
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_managedevlistdlg.h"
#include "ui_ls_managedevlistdlg.h"

LS_ManageDevListDlg::LS_ManageDevListDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LS_ManageDevListDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("设备管理"));
    initWidget();

    QColor color(219,241,252);
    set_background_color(this,color);
//    groupBox_background_icon(this);
    splitter_style_color(this);
}

LS_ManageDevListDlg::~LS_ManageDevListDlg()
{
    delete ui;
}

void LS_ManageDevListDlg::initWidget(void)
{
    mSpl = new LS_DevMainSpl(this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(mSpl);
}
