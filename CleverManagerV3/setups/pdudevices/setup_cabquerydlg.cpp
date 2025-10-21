/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "setup_cabquerydlg.h"
#include "ui_setup_cabquerydlg.h"

Setup_CabQueryDlg::Setup_CabQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup_CabQueryDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("查询对话框"));
    //groupBox_background_icon(this);
}

Setup_CabQueryDlg::~Setup_CabQueryDlg()
{
    delete ui;
}

void Setup_CabQueryDlg::setRoom()
{
    ui->cabLab->setHidden(true);
    ui->cabEdit->setHidden(true);
}


QString Setup_CabQueryDlg::getCmd()
{
    QString cmd;
    QString str = ui->roomEdit->text();
    if(!str.isEmpty()) cmd += QString(" and room like '%%1%'").arg(str);

    str = ui->cabEdit->text();
    if(!str.isEmpty()) cmd += QString(" and cab like '%%1%'").arg(str);

    return cmd;
}


void Setup_CabQueryDlg::on_quitBtn_clicked()
{
    close();
}


void Setup_CabQueryDlg::on_okBtn_clicked()
{
    this->accept();
}

