/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_adddevicedlg.h"
#include "ui_cab_adddevicedlg.h"
#include <QMessageBox>
#include "dp_basicthread.h"

Cab_AddDeviceDlg::Cab_AddDeviceDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cab_AddDeviceDlg)
{
    ui->setupUi(this);
}

Cab_AddDeviceDlg::~Cab_AddDeviceDlg()
{
    delete ui;
}

void Cab_AddDeviceDlg::init(ConsumerDeviceItem& item,int left,int right,int height)
{
    m_item = item;
    if(left<=0)ui->lineEdit_port->setEnabled(false);
    if(right<=0)ui->lineEdit_port2->setEnabled(false);

    QIntValidator* validator = new QIntValidator(1,left,this);
    ui->lineEdit_port->setValidator(validator);
    validator = new QIntValidator(1,right,this);
    ui->lineEdit_port2->setValidator(validator);

    ui->spinBox_pos->setRange(1,height);
    ui->spinBox_pos->setValue(m_item.address);
    ui->spinBox_height->setRange(1,ui->spinBox_pos->maximum()-ui->spinBox_pos->value()+1);
    ui->spinBox_height->setValue(m_item.occupy_height);
    if(m_item.main_pdu_port > 0)
        ui->lineEdit_port->setText(QString::number(m_item.main_pdu_port));//主pdu端口
    if(m_item.spare_pdu_port > 0)
        ui->lineEdit_port2->setText(QString::number(m_item.spare_pdu_port));//备pdu端口
    ui->lineEdit_name->setText(m_item.name);
}

bool Cab_AddDeviceDlg::inputCheck()
{
    bool ret = false;

    if(ui->lineEdit_name->text().isEmpty()){
        QMessageBox::warning(this,tr("警告"),tr("机架设备名称为空！"));
    } else {
        ret = DbConsumerDevices::get()->containName(m_item.cabinet_id, m_item.name);
        if(ret) {
            ret = false;
            QMessageBox::warning(this,tr("警告"),tr("机架设备名称已存在！"));
        } else {
            m_item.name = ui->lineEdit_name->text();
            m_item.address = ui->spinBox_pos->value();
            m_item.occupy_height = ui->spinBox_height->value();
            m_item.main_pdu_port = ui->lineEdit_port->text().toInt();
            m_item.spare_pdu_port = ui->lineEdit_port2->text().toInt();
            ret = true;
        }
    }

    return ret;
}

void Cab_AddDeviceDlg::save()
{
    DbConsumerDevices::get()->insertItem(m_item);
}


void Cab_AddDeviceDlg::on_okBtn_clicked()
{
    bool ret = inputCheck();
    if(ret) {
        save();
        accept();
    }
}


void Cab_AddDeviceDlg::on_spinBox_pos_valueChanged(int arg1)
{
    ui->spinBox_height->setMaximum(ui->spinBox_pos->maximum()-arg1+1);
}

Cab_ModifyDeviceDlg::Cab_ModifyDeviceDlg(QWidget *parent)
    :Cab_AddDeviceDlg(parent)
{
    ui->label_name->setText(tr("修改机架设备名称"));
}


void Cab_ModifyDeviceDlg::save()
{
    DbConsumerDevices::get()->updateItem(m_item);
}
