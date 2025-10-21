#include "dialogaddcabinetdevice.h"
#include "ui_dialogaddcabinetdevice.h"
#include <QMessageBox>
#include "common.h"

DialogAddCabinetDevice::DialogAddCabinetDevice(ConsumerDeviceItem& item,int left,int right,int height,QWidget *parent) :
    QDialog(parent),m_item(item),
    ui(new Ui::DialogAddCabinetDevice)
{
    ui->setupUi(this);

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

DialogAddCabinetDevice::~DialogAddCabinetDevice()
{
    delete ui;
}

void DialogAddCabinetDevice::accept()
{
    if(ui->lineEdit_name->text().isEmpty()){
        QMessageBox::warning(NULL,tr("警告"),tr("机架设备名称为空！"));
    } else {
        m_item.name = ui->lineEdit_name->text();
        m_item.address = ui->spinBox_pos->value();
        m_item.occupy_height = ui->spinBox_height->value();
        m_item.main_pdu_port = ui->lineEdit_port->text().toInt();
        m_item.spare_pdu_port = ui->lineEdit_port2->text().toInt();

        int ret = DbConsumerDevices::get()->containName(m_item.cabinet_id, m_item.name);
        if(ret) {
            ////=========  有名称相同的机架设备  需要提醒用户
            QDialog::accept();
        } else {
            QDialog::accept();
        }
    }
}

void DialogAddCabinetDevice::on_spinBox_pos_valueChanged(int arg1)
{
    ui->spinBox_height->setMaximum(ui->spinBox_pos->maximum()-arg1+1);
}

DialogModifyCabinetDevice::DialogModifyCabinetDevice(ConsumerDeviceItem &item, int left, int right, int height, QWidget *parent)
    :DialogAddCabinetDevice(item,left,right,height,parent)
{
    ui->label_name->setText(tr("修改机架设备名称"));
}

