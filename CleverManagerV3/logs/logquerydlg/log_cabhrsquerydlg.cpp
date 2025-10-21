/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_cabhrsquerydlg.h"
#include "ui_log_cabhrsquerydlg.h"

Log_CabHrsQueryDlg::Log_CabHrsQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log_CabHrsQueryDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("查询对话框"));
    //groupBox_background_icon(this);
    mDateBar = new SqlDateBar(ui->dateWid);
}

Log_CabHrsQueryDlg::~Log_CabHrsQueryDlg()
{
    delete ui;
}

void Log_CabHrsQueryDlg::setRoadHidden()
{
    ui->roadCmb->setHidden(true);
    ui->roadLab->setHidden(true);
}

QString Log_CabHrsQueryDlg::getCmd()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->roomEdit->text();
    if(!str.isEmpty()) cmd += QString(" and room like '%%1%'").arg(str);

    str = ui->cabEdit->text();
    if(!str.isEmpty()) cmd += QString(" and cab like '%%1%'").arg(str);

    int ret = ui->roadCmb->currentIndex();
    if(ret) {
        str = ui->roadCmb->currentText();
        cmd += QString(" and road like '%%1%'").arg(str);
    }

    return cmd;
}


void Log_CabHrsQueryDlg::on_okBtn_clicked()
{
    if(inputCheck()) {
        this->accept();
    }
}

void Log_CabHrsQueryDlg::on_quitBtn_clicked()
{
    this->close();
}
