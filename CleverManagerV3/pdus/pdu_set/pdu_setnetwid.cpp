/*
 * setdevnetwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setnetwid.h"
#include "ui_pdu_setnetwid.h"

Pdu_SetNetWid::Pdu_SetNetWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_SetNetWid)
{
    ui->setupUi(this);
    mPacket = NULL;
}

Pdu_SetNetWid::~Pdu_SetNetWid()
{
    delete ui;
}

/**
 * @brief 根据网络模式，使能相关控件
 */
void Pdu_SetNetWid::setMode(char mode)
{
    bool en = true;
    if(mode) { // 自动获取
        en = false;
        ui->comboBox->setCurrentIndex(1);
    } else
        ui->comboBox->setCurrentIndex(0);

    ui->ipEdit->setEnabled(en);
    ui->maskEdit->setEnabled(en);
    ui->gwEdit->setEnabled(en);
    ui->dnsEdit->setEnabled(en);
}

/**
 * @brief 初始化窗口
 */
void Pdu_SetNetWid::initWidget()
{
    sIpAddr *net = &(mPacket->net);
    char mode = net->mode;
    setMode(mode);

    QString str = net->ip;
    ui->ipEdit->setText(str);

    str = net->mask;
    ui->maskEdit->setText(str);

    str = net->gw;
    ui->gwEdit->setText(str);

    str = net->dns;
    ui->dnsEdit->setText(str);
    ui->wholeCheckBox->setChecked(false);
}

void Pdu_SetNetWid::clearWidget()
{
    setMode(1);
    ui->ipEdit->setText("");
    ui->maskEdit->setText("");
    ui->gwEdit->setText("");
    ui->dnsEdit->setText("");
    ui->wholeCheckBox->setChecked(false);
}


/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void Pdu_SetNetWid::updateSlot(sDataPacket *packet)
{
    clearWidget();
    mPacket = packet;
    if(packet) {
        if(packet->id) {
            mPacket = Dp_PduPackets::bulid(this)->get(packet->net.ip, 0);
            if(mPacket) initWidget();
        }
    }
}

void Pdu_SetNetWid::on_comboBox_currentIndexChanged(int index)
{
    setMode(index);
}


bool Pdu_SetNetWid::sentData(Net_sDevData &pkt)
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
 * @brief 发送网络模式数据
 * @return
 */
bool Pdu_SetNetWid::sentNetMode()
{
    Net_sDevData pkt;

    pkt.addr = (uchar)(mPacket->id);
    pkt.fn[0] = 7; // 设备用户信息
    pkt.fn[1] = 0x10; // 设备用户
    pkt.fn[1] += 6; // 设备账号和密码

    uchar mode = ui->comboBox->currentIndex();
    uchar data[4] = {mode};
    pkt.data = data;
    pkt.len = 1;

    sentData(pkt);
    QString str = tr("设备网络模式修改成功!");
    InfoMsgBox box(this,str + "\n");

    return true;
}

/**
 * @brief 网络模式是否修改
 * @return
 */
bool Pdu_SetNetWid::checkMode()
{
    bool ret = false;
    uchar mode = ui->comboBox->currentIndex();
    if(mode != mPacket->net.mode)
        ret = true;

    return ret;
}

/**
 * @brief 保存设备网络模式
 */
bool Pdu_SetNetWid::saveNetMode()
{
    bool ret = checkMode();
    if(ret) {
        ret = sentNetMode();
        mPacket->net.mode = ui->comboBox->currentIndex();
    }

    return ret;
}


/**
 * @brief 发送网络模式数据
 * @return
 */
void Pdu_SetNetWid::sentNetData(int fn,QString &str)
{
    Net_sDevData pkt;

    pkt.addr = (uchar)(mPacket->id);
    pkt.fn[0] = 7; // 设备用户信息
    pkt.fn[1] = 0x10;
    pkt.fn[1] += fn;

    uchar data[128] = {0};
    for(int i=0;i<str.length(); ++i){
        data[i] = (uchar)str.at(i).toLatin1();
    }
    pkt.data = data;
    pkt.len = str.length();

    sentData(pkt);
}

/**
 * @brief
 * @return
 */
bool Pdu_SetNetWid::saveNetData()
{
    int fn = 1, flag=0;

    sIpAddr *net = &(mPacket->net);
    QString str = ui->ipEdit->text();
    if(str != net->ip)
    {
        net->ip = str;
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->gwEdit->text();
    if(str != net->gw)
    {
        net->gw = str;
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->maskEdit->text();
    if(str != net->mask)
    {
        net->mask = str;
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->dnsEdit->text();
    if(str != net->dns)
    {
        net->dns = str;
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    if(flag) InfoMsgBox box(this,tr("设备网络修改成功!\n"));

    return true;
}

/**
 * @brief 检查网络数据是否合法
 * @return
 */
bool Pdu_SetNetWid::checkNetData()
{
    if(mPacket->net.mode == 1) // 自动获取
        return false;

    QString str = ui->ipEdit->text();
    bool ret = cm_isIPaddress(str);
    if(!ret) {
        QString str = tr("设备IP地址错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->maskEdit->text();
    ret = cm_isIPaddress(str);
    if(!ret) {
        QString str = tr("设备子网掩码错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->maskEdit->text();
    ret = cm_isIPaddress(str);
    if(!ret) {
        QString str = tr("设备默认网关错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->dnsEdit->text();
    ret = cm_isIPaddress(str);
    if(!ret) {
        QString str = tr("设备DNS设置错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    return true;
}


bool Pdu_SetNetWid::saveNetAddr()
{
    bool ret = checkNetData();
    if(ret) {
        ret = saveNetData();
    }
    return ret;
}


/**
 * @brief 增加日志信息
 */
void Pdu_SetNetWid::saveLog()
{
    sUserLogItem log;
    log.remarks = tr("网络配置");
    log.remarks +=  tr("设备IP：%1 设备网络信息已修改").arg(mPacket->net.ip);
    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 保存网络配置数据
 */
void Pdu_SetNetWid::on_saveBtn_clicked()
{
    if(mPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            bool ret1 = saveNetMode();
            bool ret2 = saveNetAddr();
            if(ret1 || ret2)
                saveLog();
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
