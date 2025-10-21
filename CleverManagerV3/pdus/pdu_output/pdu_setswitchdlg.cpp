/*
 * setswitchdlg.cpp
 * 输出位开关控制窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setswitchdlg.h"
#include "ui_pdu_setswitchdlg.h"


Pdu_SetSwitchDlg::Pdu_SetSwitchDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pdu_SetSwitchDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("开关设置"));

    groupBox_background_icon(this);
    oldSw = true;
}

Pdu_SetSwitchDlg::~Pdu_SetSwitchDlg()
{
    delete ui;
}

/**
 * @brief 设置工作模式
 * @param mode
 */
void Pdu_SetSwitchDlg::setMode(int id, sDataPacket *packet)
{
    QString str = tr("输出位(%1)开关控制").arg(id+1);
    ui->titleLabel->setText(str);

    mPacket = packet;
    int ret = mPacket->data.output.sw[id];
    if(ret == 1) // 关
        oldSw = false;

    if(oldSw)
        ui->openRadio->setChecked(true);
    else
        ui->closeRadio->setChecked(true);

    mId = id;
}


/**
 * @brief 增加日志信息
 */
void Pdu_SetSwitchDlg::saveLog()
{
    sUserLogItem log;
    log.remarks = tr("开关控制：");

    QString str = tr("设备IP：") + mPacket->net.ip;
    int num = mPacket->id;
    if(num) str += tr(" 副机:%1").arg(QString::number(num));

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一修改
    {
        str += tr(": 开关状态统一修改");
        if(ui->wholeCheckBox->isChecked())
            str += tr(" 开关状态全局设置");
    } else {
        if(ui->wholeCheckBox->isChecked())
            str += tr(" 开关状态全局设置");
        str += ": " + ui->titleLabel->text();
    }

    bool sw =  ui->openRadio->isChecked();
    if(sw)
        str += tr(" 打开");
    else
        str += tr(" 关闭");
    log.remarks += str;
    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 获取数据
 * @param buf
 * @return
 */
uchar *Pdu_SetSwitchDlg::getData(uchar *buf)
{
    bool sw =  ui->openRadio->isChecked();
    if(sw)
        buf[0] = 1;
    else
        buf[0] = 0;

    return buf;
}


/**
 * @brief 发送设备数据
 * @return true
 */
bool Pdu_SetSwitchDlg::sentData()
{    
    Net_sDevData pkt;
    pkt.addr = mPacket->id;
    pkt.fn[0] = 13; // 输出位功能码

    bool on = ui->uniteCheckBox->isChecked();
    if(on) { // 统一设置
        pkt.fn[0] = 18;
        pkt.fn[1] = 0;
    } else
        pkt.fn[1] = mId+1;
    pkt.len = 1;

    uchar data[12] = {0};
    pkt.data = getData(data);

    uchar buf[64] = {0};
    on = ui->wholeCheckBox->isChecked();
    if(on) {// 广播数据包
        pkt.addr = 0xff;
    }

    NetPackData *pack = NetPackData::bulid();
    int len = pack->net_data_packets(mPacket->devType, &pkt, buf);
    if(!on) {
        QString ip = mPacket->net.ip;
        UdpSentSocket::bulid(this)->sentData(ip, buf, len);
    } else {
        UdpBDSent::bulid(this)->sent(buf, len);
    }
    InfoMsgBox box(this, tr("修改成功!"));

    return true;
}

/**
 * @brief 保存数据
 */
void Pdu_SetSwitchDlg::saveData()
{    
    int value = 2;
    bool sw =  ui->openRadio->isChecked();
    if(!sw)
        value = 1;

    bool ret = ui->uniteCheckBox->isChecked();
    if(ret) { // 统一设置
        for(int i=0; i<mPacket->data.output.size; ++i)
            mPacket->data.output.sw[i] = value;
    } else {
        mPacket->data.output.sw[mId] = value;
    }

    ret = sentData();
    if(ret) saveLog();
}

/**
 * @brief 保存按钮响应函数
 */
void Pdu_SetSwitchDlg::on_saveBtn_clicked()
{
    saveData();
    this->accept();
}



/********************机房拓扑图模块，机柜中的PDU输出位开关控制接口********************/
extern int pdu_hd_getDevType(QString &ip);
/**
 * @brief 保存日志信息
 */
static void sql_save_swLog(const QString &ip,int devNum, int bit, int sw)
{
    sUserLogItem log;
    log.remarks = QObject::tr("开关控制：");

    QString str = QObject::tr("设备IP：") + ip;
    int num = devNum;
    if(num)
        str += QObject::tr(" 副机:%1").arg(QString::number(num));

    if(bit==0) // 统一修改
        str += QObject::tr(": 开关状态修改");
    else
        str += ": " +QObject::tr("输出位(%1)开关控制").arg(bit);

    if(sw==1)
        str += QObject::tr(" 打开");
    else
        str += QObject::tr(" 关闭");
    log.remarks += str;
    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 控制PDU输出位的开关
 * @param ip IP地址
 * @param devNum 设备级联号
 * @param bit 输出位  n表示第n-1位(0表示所有输出位)
 * @param sw 1表示开，0表示关
 */
bool set_pdu_outputSw_two(const QString &ip,int devNum, int bit, uchar sw)
{
    Net_sDevData pkt;
    pkt.addr = devNum;
    pkt.fn[0] = 13; // 输出位功能码
    pkt.fn[1] = bit;
    pkt.len = 1;

    uchar data[12] = {sw};
    pkt.data = data;

    uchar buf[64] = {0};
    int devType = 0;

    NetPackData *pack = NetPackData::bulid();
    int len = pack->net_data_packets(devType, &pkt, buf);
    UdpSentSocket::bulid()->sentData(ip, buf, len);
    sql_save_swLog(ip,devNum,bit, sw);

    return true;
}

bool set_pdu_outputSw(const QString &ip,int devNum, int bit, bool sw)
{
    uchar cmd = 0;
    if(sw) cmd = 1;

    return set_pdu_outputSw_two(ip, devNum, bit, cmd);
}
