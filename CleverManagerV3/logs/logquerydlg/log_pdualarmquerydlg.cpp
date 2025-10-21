/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_pdualarmquerydlg.h"
#include "ui_log_pdualarmquerydlg.h"

Log_PduAlarmQueryDlg::Log_PduAlarmQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log_PduAlarmQueryDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("查询对话框"));
    //groupBox_background_icon(this);
    mDateBar = new SqlDateBar(ui->dateWid);

    on_devnumCmb_currentIndexChanged(0);
    connect(ui->devnumCmb, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_devnumCmb_currentIndexChanged(int)));
}

Log_PduAlarmQueryDlg::~Log_PduAlarmQueryDlg()
{
    delete ui;
}

QString Log_PduAlarmQueryDlg::getCmd()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->roomEdit->text();
    if(!str.isEmpty()) cmd += QString(" and room like '%%1%'").arg(str);

    str = ui->cabEdit->text();
    if(!str.isEmpty()) cmd += QString(" and cab like '%%1%'").arg(str);

    int ret = ui->devtypeCmb->currentIndex();
    if(ret) {
        str = ui->devtypeCmb->currentText();
        if(ret > 1) {
            str += QString::number(ui->spinBox->value());
        }
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


void Log_PduAlarmQueryDlg::on_quitBtn_clicked()
{
    close();
}

bool Log_PduAlarmQueryDlg::inputCheck()
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

void Log_PduAlarmQueryDlg::on_okBtn_clicked()
{
    if(inputCheck()) {
        this->accept();
    }
}


void Log_PduAlarmQueryDlg::on_devnumCmb_currentIndexChanged(int index)
{
    bool en = true;
    if(index > 1) {
        en = false;
    }
    ui->spinBox->setHidden(en);
}
