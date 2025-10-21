/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_removerackdlg.h"
#include "ui_cab_removerackdlg.h"
#include <QMessageBox>

Cab_RemoveRackDlg::Cab_RemoveRackDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cab_RemoveRackDlg)
{
    ui->setupUi(this);
}

Cab_RemoveRackDlg::~Cab_RemoveRackDlg()
{
    delete ui;
}

void Cab_RemoveRackDlg::setAddress(int i)
{
    ui->spinBox->setValue(i);
}

int Cab_RemoveRackDlg::address()
{
    return ui->spinBox->value();
}
