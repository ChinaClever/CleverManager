/*
 * setdevsyscmdwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setsyscmdwid.h"
#include "ui_pdu_setsyscmdwid.h"


Pdu_SetSysCmdWid::Pdu_SetSysCmdWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_SetSysCmdWid)
{
    ui->setupUi(this);
    mPacket = NULL;
}

Pdu_SetSysCmdWid::~Pdu_SetSysCmdWid()
{
    delete ui;
}


/**
 * @brief 初始化窗口
 */
void Pdu_SetSysCmdWid::initWidget()
{
    ui->defaultCheckBox->setChecked(false);
    ui->resCheckBox->setChecked(false);
    ui->wholeCheckBox->setChecked(false);
}

/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void Pdu_SetSysCmdWid::updateSlot(sDataPacket *packet)
{
    mPacket = packet;
    if(mPacket) initWidget();
}


bool Pdu_SetSysCmdWid::sentData(Net_sDevData &pkt)
{
    bool on = ui->wholeCheckBox->isChecked();
    if(on) pkt.addr = 0xff;

    uchar buf[168] = {0};
    NetPackData *pack = NetPackData::bulid();
    int len = pack->net_data_packets(mPacket->devType, &pkt, buf);
    if(!on) {
        QString ip = mPacket->net.ip;
        UdpSentSocket::bulid(this)->sentData(ip, buf, len);
    } else {
        UdpBDSent::bulid(this)->sent(buf, len);
    }

    return true;
}

/**
 * @brief 发送恢复出厂设置命令
 * @return
 */
void Pdu_SetSysCmdWid::sentCmdData(int fn)
{
    Net_sDevData pkt;

    pkt.addr = (uchar)(mPacket->id);
    pkt.fn[0] = 20; // 系统维护命令
    pkt.fn[1] = fn;

    uchar data[4] = {1};
    pkt.data = data;
    pkt.len = 1;

    sentData(pkt);
}


/**
 * @brief
 * @return
 */
bool Pdu_SetSysCmdWid::saveNetData()
{
    int fn = 2, flag=0;
    bool ret = ui->defaultCheckBox->isChecked();
    if(ret){
        sentCmdData(fn);
        flag++;
    }
    fn += 1;

    ret = ui->resCheckBox->isChecked();
    if(ret){
        sentCmdData(fn);
        flag++;
    }
    fn += 1;

    if(flag) InfoMsgBox box(this,tr("设备命令执行成功!\n"));

    return ret;
}

/**
 * @brief 增加日志信息
 */
void Pdu_SetSysCmdWid::saveLog()
{
    sUserLogItem log;
    log.remarks = tr("设备维护");

    QString str = tr("设备IP：%1 ").arg(mPacket->net.ip);
    bool ret = ui->defaultCheckBox->isChecked();
    if(ret) str += tr("设备恢复出厂设置 ");

    ret = ui->resCheckBox->isChecked();
    if(ret) str += tr(" 设备重启");

    log.remarks +=  str;
    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 修改设备模式
 */
void Pdu_SetSysCmdWid::setDevCmd()
{
    bool ret = ui->defaultCheckBox->isChecked();
    bool res = ui->resCheckBox->isChecked();
    if(ret || res) {
        ret = saveNetData();
        if(ret) {
            saveLog();
        }
    }
}

bool Pdu_SetSysCmdWid::isChecked()
{
    bool ret = ui->defaultCheckBox->isChecked();
    if(ui->resCheckBox->isChecked())
        ret = true;
    if(ui->wholeCheckBox->isChecked())
        ret = true;

    return ret;
}

/**
 * @brief 执行命令
 */
void Pdu_SetSysCmdWid::on_saveBtn_clicked()
{
    if(mPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            ret = isChecked();
            if(ret) {
                QString str = tr("你确定要执行此操作？");
                QuMsgBox box(this,"\n" + str + "\n");
                ret = box.Exec();
                if(ret)
                    setDevCmd();
            }
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}

