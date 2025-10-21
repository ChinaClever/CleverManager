/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "tp_questdlg.h"
#include "ui_tp_questdlg.h"
#include <QMessageBox>


Tp_RequestDlg::Tp_RequestDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tp_RequestDlg)
{
    ui->setupUi(this);
}

Tp_RequestDlg::Tp_RequestDlg(QWidget *parent, const QString &title, const QString &text) :
    QDialog(parent),ui(new Ui::Tp_RequestDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->label_text->setText(text);
}

Tp_RequestDlg::~Tp_RequestDlg()
{
    delete ui;
}


void Tp_RequestDlg::delRoom(int id, const QString &name)
{
    DbRoomList::get()->remove(id);

    sUserLogItem log;
    log.remarks = tr("机房管理：");
    log.remarks += tr("删除机房:%1").arg(name);
    DbUserLog::bulid()->insertItem(log);
}

void Tp_RequestDlg::on_okBtn_clicked()
{
    accept();
}
