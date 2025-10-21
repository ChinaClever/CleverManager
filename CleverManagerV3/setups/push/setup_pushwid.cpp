/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "setup_pushwid.h"
#include "ui_setup_pushwid.h"
#include "configbase.h"
#include "msgbox.h"
#include <QUrl>

Setup_PushWid::Setup_PushWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_PushWid)
{
    ui->setupUi(this);
    initWid();
}

Setup_PushWid::~Setup_PushWid()
{
    delete ui;
}

void Setup_PushWid::initWid()
{
    sConfigItem *it = ConfigBase::bulid()->item;
    ui->envCb->setChecked(it->envEn);
    ui->tCb->setChecked(it->thresholdEn);
    ui->netCb->setChecked(it->netAddrEn);
    ui->timeSpin->setValue(it->pushTime);

    ui->udpBtn_1->setChecked(it->udpEn[0]);
    ui->udpUrl_1->setText(it->udpUrl[0]);
    ui->udpPort_1->setValue(it->udpPort[0]);

    ui->udpBtn_2->setChecked(it->udpEn[1]);
    ui->udpUrl_2->setText(it->udpUrl[1]);
    ui->udpPort_2->setValue(it->udpPort[1]);

    ui->wsEN->setChecked(it->wsEn);
    ui->wsUrl->setText(it->wsUrl);
    ui->wsPort->setValue(it->wsPort);

    button_style_sheet(ui->btn);
    button_style_sheet(ui->udpBtn_1);
    button_style_sheet(ui->udpBtn_2);
    button_style_sheet(ui->wsBtn);
}

bool Setup_PushWid::checkUdpUrl(int id)
{
    bool en = ui->udpEn_1->isChecked();
    QString url = ui->udpUrl_1->text();
    if(id) {
        en = ui->udpEn_2->isChecked();
        url = ui->udpUrl_2->text();
    }

    bool ret = true;
    if(en) {
        if(url.size()) {
            QUrl u(url);
            if(!u.isValid()) {
                ret = false;
                WaringMsgBox box(this, tr("地址输入格式有误，请重新输入!"));
            }

        } else {
            ret = false;
            WaringMsgBox box(this, tr("地址输入不能为空，请重新输入!"));
        }
    }

    return ret;
}

void Setup_PushWid::on_udpBtn_2_clicked()
{    
    int id = 1;
    bool ret = checkUdpUrl(id);
    if(ret) {
        ConfigBase *con = ConfigBase::bulid();
        sConfigItem *it = con->item;

        it->udpEn[id] = ui->udpEn_2->isChecked() ? 1:0;
        it->udpUrl[id] = ui->udpUrl_2->text();
        it->udpPort[id] = ui->udpPort_2->value();

        con->setUdpEn(id, it->udpEn[id]);
        con->setUdpUrl(id, it->udpUrl[id]);
        con->setUdpPort(id, it->udpPort[id]);

        InfoMsgBox bos(this, tr("修改成功!"));
    }
}


void Setup_PushWid::on_udpBtn_1_clicked()
{
    int id = 0;
    bool ret = checkUdpUrl(id);
    if(ret) {
        ConfigBase *con = ConfigBase::bulid();
        sConfigItem *it = con->item;

        it->udpEn[id] = ui->udpEn_1->isChecked() ? 1:0;
        it->udpUrl[id] = ui->udpUrl_1->text();
        it->udpPort[id] = ui->udpPort_1->value();

        con->setUdpEn(id, it->udpEn[id]);
        con->setUdpUrl(id, it->udpUrl[id]);
        con->setUdpPort(id, it->udpPort[id]);

        InfoMsgBox bos(this, tr("修改成功!"));
    }
}


void Setup_PushWid::on_btn_clicked()
{
    ConfigBase *con = ConfigBase::bulid();
    sConfigItem *it = con->item;

    it->envEn = ui->envCb->isChecked() ? 1:0;
    it->thresholdEn = ui->tCb->isChecked() ? 1:0;
    it->netAddrEn = ui->netCb->isChecked() ? 1:0;
    it->pushTime = ui->timeSpin->value();

    con->setEnvEn(it->envEn);
    con->setNetEn(it->netAddrEn);
    con->setPushTime(it->pushTime);
    con->setThresholdEn(it->thresholdEn);

    InfoMsgBox bos(this, tr("修改成功!"));
}

bool Setup_PushWid::checkWsUrl()
{
    bool ret = ui->wsEN->isChecked();
    if(ret) {
        QString url = ui->wsUrl->text();
        if(url.contains("ws://") || url.contains("wss://")) {
            ret = true;
        } else {
            ret = false;
            WaringMsgBox box(this, tr("地址输入有误，请重新输入!"));
        }
    } else {
        ret = true;
    }

    return ret;
}

void Setup_PushWid::on_wsBtn_clicked()
{
    bool ret = checkWsUrl();
    if(ret) {
        ConfigBase *con = ConfigBase::bulid();
        sConfigItem *it = con->item;

        it->wsEn = ui->wsEN->isChecked() ? 1:0;
        it->wsUrl = ui->wsUrl->text();
        it->wsPort = ui->wsPort->value();

        con->setWsEn(it->wsEn);
        con->setWsUrl(it->wsUrl);
        con->setWsPort(it->wsPort);

        InfoMsgBox bos(this, tr("修改成功!"));
    }
}
