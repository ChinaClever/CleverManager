/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_userquerydlg.h"
#include "ui_log_userquerydlg.h"

Log_UserQueryDlg::Log_UserQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log_UserQueryDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("查询对话框"));
    //groupBox_background_icon(this);
    mDateBar = new SqlDateBar(ui->dateWid);
}

Log_UserQueryDlg::~Log_UserQueryDlg()
{
    delete ui;
}

QString Log_UserQueryDlg::getCmd()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->userEdit->text();
    if(!str.isEmpty()) {
         cmd += QString(" and name like '%%1%'").arg(str);
    }

    return cmd;
}

void Log_UserQueryDlg::on_quitBtn_clicked()
{
    this->close();
}

void Log_UserQueryDlg::on_okBtn_clicked()
{
    this->accept();
}
