/*
 * setdevsmtpwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setdevsmtpwid.h"
#include "ui_setdevsmtpwid.h"

SetDevSmtpWid::SetDevSmtpWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDevSmtpWid)
{
    ui->setupUi(this);
    mDataPacket = NULL;
}

SetDevSmtpWid::~SetDevSmtpWid()
{
    delete ui;
}


/**
 * @brief 初始化窗口
 */
void SetDevSmtpWid::initWidget(void)
{
    mSMTP = mDataPacket->net->smtp;
    QString str = mSMTP->usr->get();
    ui->usrEdit->setText(str);

    str = mSMTP->pwd->get();
    ui->pwdEdit->setText(str);

    str = mSMTP->server->get();
    ui->serEdit->setText(str);

    str = mSMTP->test->get();
    ui->recvEdit->setText(str);

    int port = mSMTP->port;
    ui->portEdit->setText(QString::number(port));
    ui->wholeCheckBox->setChecked(false);
}



/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void SetDevSmtpWid::updateSlot(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    if(mDataPacket)
        initWidget();
}


/**
 * @brief 检查网络数据是否合法
 * @return
 */
bool SetDevSmtpWid::checkData(void)
{
    QString str = ui->usrEdit->text();
    //    if(str.isEmpty()) {
    //        QString str = tr("设备SMTP账号错误, 请重新输入!");
    //        CriticalMsgBox(this,"\n" + str + "\n");
    //        return false;
    //    }

    //    str = ui->pwdEdit->text();
    //    if(str.isEmpty()) {
    //        QString str = tr("设备SMTP密码错误, 请重新输入!");
    //        CriticalMsgBox(this,"\n" + str + "\n");
    //        return false;
    //    }

    str = ui->portEdit->text();
    bool ret = isDigitStr(str);
    if(!ret) {
        QString str = tr("设备SMTP端口设置错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    //    str = ui->serEdit->text();
    //    if(str.isEmpty()) {
    //        QString str = tr("设备SMTP服务器错误, 请重新输入!");
    //        CriticalMsgBox(this,"\n" + str + "\n");
    //        return false;
    //    }

    return true;
}



/**
 * @brief 发送网络模式数据
 * @return
 */
void SetDevSmtpWid::sentNetData(int fn,QString &str)
{

    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
    pkt.fn[0] = 7; // 设备用户信息
    pkt.fn[1] = 0xA0; // SNMP
    pkt.fn[1] += fn;

    uchar data[128] = {0};
    for(int i=0;i<str.length(); ++i){
        data[i] = (uchar)str.at(i).toLatin1();
    }
    pkt.data = data;
    pkt.len = str.length();

    uchar buf[168] = {0};
    bool on = ui->wholeCheckBox->isChecked();
    if(on) // 广播数据包
    {
        pkt.addr = 0xff;
        int len = net_data_packets(mDataPacket->devType, TRA_TYPR_UDP, &pkt, buf);
        udp_queue_append(buf, len);
    } else {
         int len = net_data_packets(mDataPacket->devType, TRA_TYPR_TCP, &pkt, buf);
        tcp_queue_append(buf, len);
    }
}


/**
 * @brief 发送网络模式数据
 * @return
 */
void SetDevSmtpWid::sentNetData(int fn,short port)
{
    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
    pkt.fn[0] = 7; // 设备用户信息
    pkt.fn[1] = 0xA0;
    pkt.fn[1] += fn;

    uchar data[4] = {0};
    data[0] = port >> 8;
    data[1] = port & 0xff;
    pkt.data = data;
    pkt.len = 2;

    uchar buf[32] = {0};
    bool on = ui->wholeCheckBox->isChecked();
    if(on) // 广播数据包
    {
        int len = net_data_packets(mDataPacket->devType, TRA_TYPR_UDP, &pkt, buf);
        udp_queue_append(buf, len);
    } else {
        int len = net_data_packets(mDataPacket->devType, TRA_TYPR_TCP, &pkt, buf);
        tcp_queue_append(buf, len);
    }
}


/**
 * @brief
 * @return
 */
bool SetDevSmtpWid::saveNetData(void)
{
    int fn = 1, flag=0;
    QString str = ui->usrEdit->text();
    //        if(str != mSMTP->usr->get())
    {
        mSMTP->usr->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->pwdEdit->text();
    //        if(str != mSMTP->pwd->get())
    {
        mSMTP->pwd->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->serEdit->text();
    //        if(str != mSMTP->server->get())
    {
        mSMTP->server->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->portEdit->text();
    short port = str.toInt();
    //        if(port != mSMTP->port)
    {
        mSMTP->port = port;
        sentNetData(fn, port);
        flag++;
    }
    fn += 1;

    fn += 1; // 认证方式

    str = ui->recvEdit->text();
    //        if(str != mSMTP->test->get())
    {
        mSMTP->test->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    bool ret = get_tcp_connect(mDataPacket->ip->get());
    if(ret)
    {
        //if(flag) {
            QString str;
            if(ret)
                str = tr("设备SMTP修改成功!");
            else
                str = tr("设备SMTP修改失败!");
            InfoMsgBox box(this,str + "\n");
        //}
    }
    else
        InfoMsgBox box(this,tr("连接失败") + "\n");

    return ret;
}


/**
 * @brief 增加日志信息
 */
void SetDevSmtpWid::saveLog(void)
{
    /*增加日志信息*/
    s_RecordLog log;
    log.title = tr("设备配置");
    log.operation = tr("设备SMTP修改");

    log.msg = tr("设备IP：%1 设备SMTP信息已修改").arg(mDataPacket->ip->get());
    sql_insert_record(log);
}

/**
 * @brief 保存SMTP信息
 * @return
 */
bool SetDevSmtpWid::saveSMTP(void)
{
    bool ret = checkData();
    if(ret) {
        ret = saveNetData();
        if(ret)
            saveLog();
    }
    return ret;
}


void SetDevSmtpWid::on_saveBtn_clicked()
{
    if(mDataPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            saveSMTP();
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
