/*
 * setdevnamewid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setnamewid.h"
#include "ui_pdu_setnamewid.h"

Pdu_SetNameWid::Pdu_SetNameWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_SetNameWid)
{
    ui->setupUi(this);
    mPacket = NULL;
}

Pdu_SetNameWid::~Pdu_SetNameWid()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void Pdu_SetNameWid::initWidget()
{
    QString name = mPacket->name;
    ui->lineEdit->setText(name);

    QStringList list;
    list << tr("主机");
    for(int i=1; i<10; ++i)
        list << tr("副机 ") + QString::number(i);
    ui->comboBox->clear();
    ui->comboBox->addItems(list);
    ui->comboBox->setEditable(false);

    int num = mPacket->id;
    ui->comboBox->setCurrentIndex(num);
    ui->wholeCheckBox->setChecked(false);
    if(num){
        ui->saveBtn->setEnabled(false);
    }
}

/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void Pdu_SetNameWid::updateSlot(sDataPacket *packet)
{
    mPacket = packet;
    if(mPacket) initWidget();
}


/**
 * @brief 检查输入是否合法
 * @return true
 */
bool Pdu_SetNameWid::checkDevName()
{
     QString name = ui->lineEdit->text();
     if(name.isEmpty()) {
         QString str = tr("设备名称不能为空, 请重新输入!");
         CriticalMsgBox(this,"\n" + str + "\n");
         return false;
     }

     if(name.length() > 9) {
         QString str = tr("设备名称太长, 请重新输入!");
         CriticalMsgBox(this,"\n" + str + "\n");
         return false;
     }

    return true;
}


bool Pdu_SetNameWid::sentData(Net_sDevData &pkt)
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
 * @brief 发送设备数据
 * @return
 */
bool Pdu_SetNameWid::sentDevName()
{
    Net_sDevData pkt;

    pkt.addr = (uchar)(mPacket->id);
    pkt.fn[0] = 5; // 设备信息
    pkt.fn[1] = 0x10; // 设备类型
    pkt.fn[1] += 1; // 设备名称

    QString name = ui->lineEdit->text();
    uchar data[64] = {0};
    for(int i=0;i<name.length(); ++i){
        data[i] = (uchar)name.at(i).toLatin1();
    }
    pkt.data = data;
    pkt.len = name.length();

    bool ret = sentData(pkt);;
    InfoMsgBox box(this,tr("设备名称修改成功!\n"));

    return ret;
}


/**
 * @brief 增加日志信息
 */
void Pdu_SetNameWid::saveDevNameLog()
{
    sUserLogItem log;
    log.remarks = tr("修改设备名：");

    QString name = mPacket->name;
    QString newName = ui->lineEdit->text();
    log.remarks += tr("设备IP：%1 设备名%2 改为：%3").arg(mPacket->net.ip).arg(name).arg(newName);

    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 修改设备名
 */
void Pdu_SetNameWid::renameDev()
{
    QString name = mPacket->name;
    QString newName = ui->lineEdit->text();
    if(name != newName)
    {
        bool ret = checkDevName();
        if(ret) {
            ret = sentDevName();
            if(ret) {
                saveDevNameLog();                
                mPacket->name = newName;
            }
        }
    }
}


/**
 * @brief 数据保存
 */
void Pdu_SetNameWid::on_saveBtn_clicked()
{
    if(mPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            renameDev();
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
