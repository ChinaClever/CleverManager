/*
 * log_calendardlg.cpp
 * 日期选择窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_calendardlg.h"
#include "ui_log_calendardlg.h"
#include "common.h"

LOG_CalendarDlg::LOG_CalendarDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LOG_CalendarDlg)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/logo.jpg"));
    groupBox_background_icon(this);
    this->setWindowTitle(tr("日期选择"));

    connect(ui->calendarWidget,SIGNAL(selectionChanged()),this, SLOT(selectionChangedSLot()));
    selectionChangedSLot();
}

LOG_CalendarDlg::~LOG_CalendarDlg()
{
    delete ui;
}

void LOG_CalendarDlg::selectionChangedSLot()
{
    mDate= ui->calendarWidget->selectedDate();

    QString str = mDate.toString("yyyy-MM-dd");
    ui->timeLab->setText(str);
}

void LOG_CalendarDlg::on_pushButton_clicked()
{
    this->accept();
}
