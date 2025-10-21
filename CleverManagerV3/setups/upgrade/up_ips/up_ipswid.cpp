/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "up_ipswid.h"
#include "ui_up_ipswid.h"
#include <QFileDialog>
#include "msgbox.h"
#include <QHostAddress>
#include "common.h"
#include "up_datapacket.h"
#include "dbpdudevices.h"

Up_IpsWid::Up_IpsWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Up_IpsWid)
{
    ui->setupUi(this);
    on_getRadio_clicked(true);
}

Up_IpsWid::~Up_IpsWid()
{
    delete ui;
}

void Up_IpsWid::disEnable()
{
    ui->widget->setDisabled(true);
    ui->exportBtn->setDisabled(true);
    ui->ipEdit->setDisabled(true);
    ui->ipBtn->setDisabled(true);
    ui->getBtn->setDisabled(true);
}


void Up_IpsWid::on_exportRadio_clicked(bool checked)
{
    disEnable();
    ui->exportBtn->setEnabled(checked);
}

void Up_IpsWid::on_ipRadio_clicked(bool checked)
{
    disEnable();
    ui->ipEdit->setEnabled(checked);
    ui->ipBtn->setEnabled(checked);
}

void Up_IpsWid::on_ipsRadio_clicked(bool checked)
{
    disEnable();
    ui->widget->setEnabled(checked);
}


bool Up_IpsWid::readFile(QString &fn, QStringList &list)
{
    int i=0;
    QFile file(fn);
    if(file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd())
        {
            i++;
            QString str = QString(file.readLine()).trimmed().remove("\r\n");
            if(list.contains(str)) continue;

            bool ret = cm_isIPaddress(str);
            if(ret) {
                list  << str;
            } else {
                CriticalMsgBox bos(this, tr("第%1条，%2 不是IP地址!!").arg(i).arg(str));
                return false;
            }
        }
    }

    return true;
}

void Up_IpsWid::on_exportBtn_clicked()
{
    QString fn = QFileDialog::getOpenFileName(0,tr("文件选择"),"/","",0);
    if (!fn.isNull()) {
        QStringList list;
        bool ret = readFile(fn, list);
        if(ret) {
            Up_DataPacket::bulid()->data->ips = list;
        }
    }
}

void Up_IpsWid::on_ipBtn_clicked()
{
    QString ip = ui->ipEdit->text();
    bool ret = cm_isIPaddress(ip);
    if(ret) {
        QStringList list; list << ip;
        Up_DataPacket::bulid()->data->ips = list;
    } else {
        CriticalMsgBox bos(this, tr("%1 不是IP地址!!").arg(ip));
    }
}

int Up_IpsWid::getIpInt(QString &ip)
{
    QHostAddress sendip(ip);
    return sendip.toIPv4Address();
}

int Up_IpsWid::bulidIps( QString startIp,  QString &endIp)
{
    QStringList list;
    QHostAddress startAddr(startIp), endAddr(endIp);
    for(uint i=startAddr.toIPv4Address(); i<=endAddr.toIPv4Address(); ++i)
    {
        list << QHostAddress(i).toString().remove("::ffff:"); // IPV4删除这个字符串
    }
    Up_DataPacket::bulid()->data->ips = list;

    return list.size();
}

void Up_IpsWid::on_ipsBtn_clicked()
{
    QString start = ui->startEdit->text();
    QString end = ui->endEdit->text();

    bool ret = cm_isIPaddress(start);
    if(ret) {
        ret = cm_isIPaddress(end);
        if(ret) {
            int rtn = bulidIps(start, end);
            if(!rtn) {
                CriticalMsgBox bos(this, tr("IP地址生成错误!!"));
            }
        } else {
            CriticalMsgBox bos(this, tr("%1 不是IP地址!!").arg(end));
        }
    } else {
        CriticalMsgBox bos(this, tr("起始IP:%1 不是IP地址!!").arg(start));
    }
}

void Up_IpsWid::on_getRadio_clicked(bool checked)
{
    disEnable();
    ui->getBtn->setEnabled(checked);
}

void Up_IpsWid::on_getBtn_clicked()
{
    QString dev_type = Up_DataPacket::bulid()->data->dev_type;
    QStringList ips = DbPduDevices::get()->listIps(dev_type);
    Up_DataPacket::bulid()->data->ips = ips;
}
