/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_roomhrsquerydlg.h"
#include "ui_log_roomhrsquerydlg.h"

Log_RoomHrsQueryDlg::Log_RoomHrsQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log_RoomHrsQueryDlg)
{
    ui->setupUi(this); this->setWindowTitle(tr("查询对话框"));
    //groupBox_background_icon(this);
    mDateBar = new SqlDateBar(ui->dateWid);
}

Log_RoomHrsQueryDlg::~Log_RoomHrsQueryDlg()
{
    delete ui;
}


QString Log_RoomHrsQueryDlg::getCmd()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->roomEdit->text();
    if(!str.isEmpty()) cmd += QString(" and room like '%%1%'").arg(str);

    return cmd;
}


void Log_RoomHrsQueryDlg::on_okBtn_clicked()
{
    if(inputCheck()) {
        this->accept();
    }
}

void Log_RoomHrsQueryDlg::on_quitBtn_clicked()
{
    this->close();
}
