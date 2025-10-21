/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "setup_pdusquerydlg.h"
#include "ui_setup_pdusquerydlg.h"

Setup_PdusQueryDlg::Setup_PdusQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup_PdusQueryDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("查询对话框"));
    //groupBox_background_icon(this);
}

Setup_PdusQueryDlg::~Setup_PdusQueryDlg()
{
    delete ui;
}



QString Setup_PdusQueryDlg::getCmd()
{
    QString cmd;
    QString str = ui->roomEdit->text();
    if(!str.isEmpty()) cmd += QString(" and room like '%%1%'").arg(str);

    str = ui->cabEdit->text();
    if(!str.isEmpty()) cmd += QString(" and cab like '%%1%'").arg(str);

    int ret = ui->devtypeCmb->currentIndex();
    if(ret) {
        str = ui->devtypeCmb->currentText();
        cmd += QString(" and dev_type like '%%1%'").arg(str);
    }

    str = ui->ipEdit->text();
    if(!str.isEmpty()) cmd += QString(" and ip like '%%1%'").arg(str);

    ret = ui->devnumCmb->currentIndex();
    if(ret) {
        str = ui->devnumCmb->currentText();
        cmd += QString(" and dev_num like '%%1%'").arg(str);
    }

    return cmd;
}


void Setup_PdusQueryDlg::on_quitBtn_clicked()
{
    close();
}

bool Setup_PdusQueryDlg::inputCheck()
{
    bool ret = true;

    QString ip = ui->ipEdit->text();
    if(!ip.isEmpty()) {
        ret = cm_isIPaddress(ip);
        if(!ret) {
            CriticalMsgBox box(this, tr("设备IP地址输入有误，请重新输入!!!"));
        }
    }

    return ret;
}

void Setup_PdusQueryDlg::on_okBtn_clicked()
{
    if(inputCheck()) {
        this->accept();
    }
}

