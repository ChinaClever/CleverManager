/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_cabelecquerydlg.h"
#include "ui_log_cabhrsquerydlg.h"

Log_CabElecQueryDlg::Log_CabElecQueryDlg(QWidget *parent) :
    Log_CabHrsQueryDlg(parent)
{
    ui->roadCmb->removeItem(0);
}

Log_CabElecQueryDlg::~Log_CabElecQueryDlg()
{

}

bool Log_CabElecQueryDlg::inputCheck()
{
    bool ret = false;
    QString str = ui->roomEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("机房名不能为空，请重新输入!!!"));
    } else {
        ret = true;
    }

    return ret;
}

QStringList Log_CabElecQueryDlg::getCabs()
{
    QStringList list;

    QString cab = ui->cabEdit->text();
    if(!cab.isEmpty()) {
        list << QString(" and cab like '%%1%'").arg(cab);
    } else {
        QString room = ui->roomEdit->text();
        QString cmd = QString(" where room=\'%1\'").arg(room);
        QStringList rets = DbCabHrs::bulid()->listColumn("cab", cmd);
        for(int i=0; i<rets.size(); i++) {
            list << QString(" and cab like '%%1%'").arg(rets.at(i));
        }
    }

    return list;
}

QStringList Log_CabElecQueryDlg::getCmds()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->roomEdit->text();
    cmd += QString(" and room like '%%1%'").arg(str);

    str = ui->roadCmb->currentText();
    cmd += QString(" and road like '%%1%'").arg(str);

    QStringList list, cabs = getCabs();
    for(int i=0; i<cabs.size(); ++i) {
        list << cmd + cabs.at(i);
    }

    return list;
}


QString Log_CabElecQueryDlg::getCmd()
{
    QStringList list = getCmds();
    DbCabElec::bulid()->elec(list);

    return "";
}
