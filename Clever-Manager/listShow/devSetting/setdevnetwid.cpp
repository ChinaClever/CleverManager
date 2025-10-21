/*
 * setdevnetwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setdevnetwid.h"
#include "ui_setdevnetwid.h"

SetDevNetWid::SetDevNetWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDevNetWid)
{
    ui->setupUi(this);
    mDataPacket = NULL;
}

SetDevNetWid::~SetDevNetWid()
{
    delete ui;
}

/**
 * @brief 根据网络模式，使能相关控件
 */
void SetDevNetWid::setMode(char mode)
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
void SetDevNetWid::initWidget(void)
{
    mNetIPAddr = mDataPacket->net->ip->ip;

    char mode = mNetIPAddr->mode;
    setMode(mode);

    QString str = mNetIPAddr->ip->get();
    ui->ipEdit->setText(str);

    str = mNetIPAddr->mask->get();
    ui->maskEdit->setText(str);

    str = mNetIPAddr->gw->get();
    ui->gwEdit->setText(str);

    str = mNetIPAddr->dns->get();
    ui->dnsEdit->setText(str);
    ui->wholeCheckBox->setChecked(false);
}

/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void SetDevNetWid::updateSlot(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    if(mDataPacket)
        initWidget();
}

void SetDevNetWid::on_comboBox_currentIndexChanged(int index)
{
    setMode(index);
}



/**
 * @brief 发送网络模式数据
 * @return
 */
bool SetDevNetWid::sentNetMode(void)
{
    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
    pkt.fn[0] = 7; // 设备用户信息
    pkt.fn[1] = 0x10; // 设备用户
    pkt.fn[1] += 6; // 设备账号和密码

    uchar mode = ui->comboBox->currentIndex();
    uchar data[4] = {mode};
    pkt.data = data;
    pkt.len = 1;

    uchar buf[32] = {0};
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

    QString str;
    bool ret = get_tcp_connect(mDataPacket->ip->get());
    if(ret)
        str = tr("设备网络模式修改成功!");
    else
        str = tr("设备网络模式修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}

/**
 * @brief 网络模式是否修改
 * @return
 */
bool SetDevNetWid::checkMode(void)
{
    bool ret = false;
    uchar mode = ui->comboBox->currentIndex();
    if(mode != mNetIPAddr->mode)
        ret = true;
    return ret;
}

/**
 * @brief 保存设备网络模式
 */
bool SetDevNetWid::saveNetMode(void)
{
    bool ret = checkMode();
    if(ret) {
        ret = sentNetMode();
        mNetIPAddr->mode = ui->comboBox->currentIndex();
    }

    return ret;
}

/**
 * @brief 发送网络模式数据
 * @return
 */
void SetDevNetWid::sentNetData(int fn,QString &str)
{
    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
    pkt.fn[0] = 7; // 设备用户信息
    pkt.fn[1] = 0x10;
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
        int len = net_data_packets(mDataPacket->devType, TRA_TYPR_UDP, &pkt, buf);
        if(fn != 1)
        udp_queue_append(buf, len);
    }
    else
    {
        int len = net_data_packets(mDataPacket->devType, TRA_TYPR_TCP, &pkt, buf);
        tcp_queue_append(buf, len);
    }
}

/**
 * @brief
 * @return
 */
bool SetDevNetWid::saveNetData(void)
{
    int fn = 1, flag=0;

    bool ret = get_tcp_connect(mDataPacket->ip->get());
    if(ret)
    {
        QString str = ui->ipEdit->text();
//        if(str != mNetIPAddr->ip->get())
        {
            mNetIPAddr->ip->set(str);
            sentNetData(fn, str);
            flag++;
        }
        fn += 1;

        str = ui->gwEdit->text();
//        if(str != mNetIPAddr->gw->get())
        {
            mNetIPAddr->gw->set(str);
            sentNetData(fn, str);
            flag++;
        }
        fn += 1;

        str = ui->maskEdit->text();
//        if(str != mNetIPAddr->mask->get())
        {
            mNetIPAddr->mask->set(str);
            sentNetData(fn, str);
            flag++;
        }
        fn += 1;

        str = ui->dnsEdit->text();
//        if(str != mNetIPAddr->dns->get())
        {
            mNetIPAddr->dns->set(str);
            sentNetData(fn, str);
            flag++;
        }
        fn += 1;

        if(flag) {
            QString str;
            if(ret)
                str = tr("设备网络修改成功!");
            else
                str = tr("设备网络修改失败!");
            InfoMsgBox box(this,str + "\n");
        }
    }
    else
        InfoMsgBox box(this,tr("连接失败") + "\n");

    return ret;
}

/**
 * @brief 检查网络数据是否合法
 * @return
 */
bool SetDevNetWid::checkNetData(void)
{
    if(mNetIPAddr->mode == 1) // 自动获取
        return false;

    QString str = ui->ipEdit->text();
    bool ret = isIPaddress(str);
    if(!ret) {
        QString str = tr("设备IP地址错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->maskEdit->text();
    ret = isIPaddress(str);
    if(!ret) {
        QString str = tr("设备子网掩码错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->maskEdit->text();
    ret = isIPaddress(str);
    if(!ret) {
        QString str = tr("设备默认网关错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->dnsEdit->text();
    ret = isIPaddress(str);
    if(!ret) {
        QString str = tr("设备DNS设置错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    return true;
}


bool SetDevNetWid::saveNetAddr(void)
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
void SetDevNetWid::saveLog(void)
{
   /*增加日志信息*/
   s_RecordLog log;
   log.title = tr("设备配置");
   log.operation = tr("设备网络信息修改");

   log.msg = tr("设备IP：%1 设备网络信息已修改").arg(mDataPacket->ip->get());
   sql_insert_record(log);
}

/**
 * @brief 保存网络配置数据
 */
void SetDevNetWid::on_saveBtn_clicked()
{
    if(mDataPacket) {
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
