/*
 * setdevusrwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setuserwid.h"
#include "ui_pdu_setuserwid.h"

Pdu_SetUserWid::Pdu_SetUserWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_SetUserWid)
{
    ui->setupUi(this);
    mPacket = NULL;
}

Pdu_SetUserWid::~Pdu_SetUserWid()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void Pdu_SetUserWid::initWidget()
{
    mUsrName = mPacket->user;
    if(mUsrName.size()) {
        ui->usrEdit->setText(mUsrName);        
        mPwd = mPacket->pwd;
        ui->pwdEdit->setText(mPwd);
    }
    ui->wholeCheckBox->setChecked(false);
}

/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void Pdu_SetUserWid::updateSlot(sDataPacket *packet)
{
    mPacket = packet;
    if(mPacket) initWidget();
}

/**
 * @brief 检查输入是否合法
 * @return true
 */
bool Pdu_SetUserWid::checkLand()
{
     QString name = ui->usrEdit->text();
     if(name.isEmpty()) {
         QString str = tr("设备账号不能为空, 请重新输入!");
         CriticalMsgBox(this,"\n" + str + "\n");
         return false;
     }

     if(name.length() > 9) {
         QString str = tr("设备账号太长, 请重新输入!");
         CriticalMsgBox(this,"\n" + str + "\n");
         return false;
     }

     name = ui->pwdEdit->text();
     if(name.isEmpty()) {
         QString str = tr("设备密码不能为空, 请重新输入!");
         CriticalMsgBox(this,"\n" + str + "\n");
         return false;
     }

     if(name.length() > 9) {
         QString str = tr("设备密码太长, 请重新输入!");
         CriticalMsgBox(this,"\n" + str + "\n");
         return false;
     }

    return true;
}


bool Pdu_SetUserWid::sentData(Net_sDevData &pkt)
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
bool Pdu_SetUserWid::sentLand()
{
    Net_sDevData pkt;

    pkt.addr = (uchar)(mPacket->id);
    pkt.fn[0] = 6; // 设备用户信息
    pkt.fn[1] = 0x10; // 设备用户
    pkt.fn[1] += 1; // 设备账号和密码

    QString name = ui->usrEdit->text() +"; " + ui->pwdEdit->text();
    uchar data[128] = {0};
    for(int i=0;i<name.length(); ++i){
        data[i] = (uchar)name.at(i).toLatin1();
    }
    pkt.data = data;
    pkt.len = name.length();

    bool ret = sentData(pkt);
    InfoMsgBox box(this,tr("设备登录信息修改成功!\n"));

    return ret;
}


/**
 * @brief 增加日志信息
 */
void Pdu_SetUserWid::saveLandLog()
{
    sUserLogItem log;
    log.remarks = tr("设备配置");
    log.remarks +=  tr("设备IP：%1 登录账号%2 修改为：%3").arg(mPacket->net.ip).arg(mUsrName).arg(mPacket->user);
    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 修改设备名
 */
void Pdu_SetUserWid::setLand()
{
    QString name = ui->usrEdit->text();
    QString pwd = ui->pwdEdit->text();
    if((mUsrName != name) || (mPwd != pwd)) {
        bool ret = checkLand();
        if(ret) {
            ret = sentLand();
            if(ret) {
                mPacket->user = name;
                mPacket->pwd = pwd;
                saveLandLog();
            }
        }
    } else {
        InfoMsgBox box(this,tr("未修改信息!\n"));
    }
}



void Pdu_SetUserWid::on_saveBtn_clicked()
{
    if(mPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            setLand();
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
