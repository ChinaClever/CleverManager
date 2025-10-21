/*
 * dt_detaildlg.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_detaildlg.h"
#include "ui_dt_detaildlg.h"


DT_DetailDlg::DT_DetailDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DT_DetailDlg)
{
    ui->setupUi(this);

    m_pDevInfo = new DS_DevInfoTable(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_pDevInfo);
}

DT_DetailDlg::~DT_DetailDlg()
{
    delete ui;
}

void DT_DetailDlg::updateWidget(QString &ip,short num)
{
    m_pDevInfo->updateWidget(ip,num);
}
