/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_pduelecquerydlg.h"
#include "ui_log_pdualarmquerydlg.h"

Log_PduElecQueryDlg::Log_PduElecQueryDlg(QWidget *parent) : Log_PduAlarmQueryDlg(parent)
{
    ui->devtypeCmb->removeItem(0);
    ui->devnumCmb->removeItem(0);
}


bool Log_PduElecQueryDlg::inputCheck()
{
    bool ret = false;

    QString msg, ip = ui->ipEdit->text();
    if(!ip.isEmpty()) {
        ret = cm_isIPaddress(ip);
        if(!ret) {
            msg = tr("设备IP地址输入有误，请重新输入!!!");
        }
    } else {
        msg = tr("设备IP地址不能为空，请重新输入!!!");
    }

    if(!msg.isEmpty()) {
        CriticalMsgBox box(this, msg);
    }

    return ret;
}

QStringList Log_PduElecQueryDlg::getItems()
{
    QStringList list;

    QString item = mDateBar->getDate();
    QString cmd = QString(" where %1").arg(item);
    QStringList rets = DbPduHrs::bulid()->listColumn("item", cmd);
    for(int i=0; i<rets.size(); i++) {
        list << QString(" and item like '%%1%'").arg(rets.at(i));
    }

    return list;
}


QStringList Log_PduElecQueryDlg::getCmds()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->roomEdit->text();
    if(!str.isEmpty()) cmd += QString(" and room like '%%1%'").arg(str);

    str = ui->cabEdit->text();
    if(!str.isEmpty()) cmd += QString(" and cab like '%%1%'").arg(str);

    str = ui->devtypeCmb->currentText();
    cmd += QString(" and dev_type like '%%1%'").arg(str);

    str = ui->ipEdit->text();
    cmd += QString(" and ip like '%%1%'").arg(str);

    str = ui->devnumCmb->currentText();
    if(ui->devnumCmb->currentIndex()) {
         str += QString::number(ui->spinBox->value());
    }
    cmd += QString(" and dev_num like '%%1%'").arg(str);

    QStringList list, items = getItems();
    for(int i=0; i<items.size(); ++i) {
        list << cmd + items.at(i);
    }

    return list;
}


QString Log_PduElecQueryDlg::getCmd()
{
    QStringList list = getCmds();
    DbPduElec::bulid()->elec(list);

    return "";
}


void Log_PduElecQueryDlg::on_devnumCmb_currentIndexChanged(int index)
{
    bool en = true;
    if(index) {
        en = false;
    }
    ui->spinBox->setHidden(en);
}
