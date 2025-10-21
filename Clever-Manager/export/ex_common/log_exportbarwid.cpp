/*
 * log_exportbarwid.cpp
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_exportbarwid.h"
#include "ui_log_exportbarwid.h"
#include "logs/log_calendardlg.h"
#include <QGridLayout>
extern bool usr_land_jur(void);

Log_ExportBarWid::Log_ExportBarWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log_ExportBarWid)
{
    ui->setupUi(this);
    initWidget();

    set_background_color(this,QColor(0,17,55));
    btnyellow_style_sheet(ui->startDateBtn);
    btnyellow_style_sheet(ui->endDateBtn);
    btnyellow_style_sheet(ui->findBtn);
    btnyellow_style_sheet(ui->exportBtn);
    btnyellow_style_sheet(ui->cleverBtn);
    ui->cleverBtn->setHidden(true);


    QGridLayout *layout = new QGridLayout(parent);
    layout->addWidget(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    //    QTimer::singleShot(15*1000,this,SLOT(on_findBtn_clicked())); //延时初始化
}

Log_ExportBarWid::~Log_ExportBarWid()
{
    delete ui;
}


void Log_ExportBarWid::initWidget()
{
    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate());
    on_roomRadio_clicked();
}


void Log_ExportBarWid::on_modeBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0: on_devRadio_clicked(); break;
    case 1: on_roomRadio_clicked(); break;
    case 2: on_cabRadio_clicked(); break;
    case 3:{
        ui->label->setText(tr("机房名"));///////////////////////////////////////2024-07-25
        ui->edit->setText(cab);
        ui->spinBox->setHidden(true);
        ui->saveLabel->setHidden(true);
        break;
    }
    }
}

void Log_ExportBarWid::on_cabRadio_clicked()
{
    ui->label->setText(tr("机柜名"));
    ui->edit->setText(cab);
    ui->spinBox->setHidden(true);
    ui->saveLabel->setHidden(true);
}

void Log_ExportBarWid::on_roomRadio_clicked()
{
    ui->label->setText(tr("机房名"));
    ui->edit->setText(room);
    ui->spinBox->setHidden(true);
    ui->saveLabel->setHidden(true);
}

void Log_ExportBarWid::on_devRadio_clicked()
{
    ui->label->setText(tr("设备IP"));
    ui->edit->setText(ip);
    ui->spinBox->setHidden(false);
    ui->saveLabel->setHidden(false);
}

bool Log_ExportBarWid::inputCheck()
{
    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();

    if(startDate > endDate) {
        CriticalMsgBox box(0, tr("开始日期应早于结束时间，请重新输入!"));
        return false;
    }

    if(0 == ui->modeBox->currentIndex()) {
        QString str = ui->edit->text();
        if(!isIPaddress(str)) {
            CriticalMsgBox box(0, tr("设备IP地址格式错误，请重新输入!"));
            return false;
        }
    }
    getValue();

    return true;
}


void Log_ExportBarWid::getValue()
{
    startDate = ui->startDateEdit->date();
    startTime.setHMS(0,0,0);

    endDate = ui->endDateEdit->date();
    endTime.setHMS(23,59,59);

    slaveNum = ui->spinBox->value();;
}

void Log_ExportBarWid::on_findBtn_clicked()
{
    if(inputCheck()) {
        int sig = ui->modeBox->currentIndex();
        switch(sig) {
        case LOG_EXPORT_DEV: ip = ui->edit->text(); break;
        case LOG_EXPORT_ROOM: room = ui->edit->text(); break;
        case LOG_EXPORT_CAB: cab = ui->edit->text(); break;
        case LOG_EXPORT_ALLBYROOM: cab = ui->edit->text(); break;
        }

        emit findSig(sig);
    }

    /*
 * 显示pdu信息
 */
    //    QDate date=ui->endDateEdit->date();

    //      box=new infobox(this,ui->edit->text(),date);
    //      box->exec();
}

void Log_ExportBarWid::on_exportBtn_clicked()
{
    if(inputCheck()) {
        int sig = LOG_EXPORT_DEV;
        if(1 == ui->modeBox->currentIndex())
            sig = LOG_EXPORT_ROOM;
        else if(2 == ui->modeBox->currentIndex())
            sig = LOG_EXPORT_CAB;
        else if(3 == ui->modeBox->currentIndex())
            sig = LOG_EXPORT_ALLBYROOM;
        emit exportSig(sig);
    }
}

void Log_ExportBarWid::setDevMode(int index)
{
    on_devRadio_clicked();
    ui->modeBox->addItem(tr("机柜查询"));
    if(index == 1)
        ui->modeBox->addItem(tr("批量查询"));///////////////////////////////////////2024-07-25
    else if(index == 0 && ui->modeBox->count() == 4){
        ui->modeBox->removeItem(ui->modeBox->count() - 1);
    }
    ui->cleverBtn->setHidden(false);
}

QDate Log_ExportBarWid::getCalDate()
{
    LOG_CalendarDlg dlg;
    dlg.exec();

    return dlg.getDate();
}

void Log_ExportBarWid::on_startDateBtn_clicked()
{
    ui->startDateEdit->setDate(getCalDate());
}

void Log_ExportBarWid::on_endDateBtn_clicked()
{
    ui->endDateEdit->setDate(getCalDate());
}


/*
 * pdu历史数据
 */
void Log_ExportBarWid::on_pushButton_clicked()
{
    //    QDate date=ui->endDateEdit->date();
    //          box=new diagram(this,ui->edit->text(),ui->startDateEdit->date(),ui->endDateEdit->date());
    //          box->exec();
    //    scatter=new DiaScatter(this,ui->edit->text(),ui->startDateEdit->date(),ui->endDateEdit->date());
    //    scatter->exec();
}


// 清空
void Log_ExportBarWid::on_cleverBtn_clicked()
{
    bool ret = usr_land_jur();
    if(ret == false)
    {
        InfoMsgBox(0, tr("您无法进行此操作"));
        return ;
    }

    QuMsgBox box(0,tr("确定清空日志信息"));
    ret = box.Exec();
    if(ret)
        emit clearSig();
}



