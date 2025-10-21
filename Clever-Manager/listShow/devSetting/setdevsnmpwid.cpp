/*
 * setdevsnmpwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setdevsnmpwid.h"
#include "ui_setdevsnmpwid.h"

SetDevSnmpWid::SetDevSnmpWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDevSnmpWid)
{
    ui->setupUi(this);
    mDataPacket = NULL;
}

SetDevSnmpWid::~SetDevSnmpWid()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void SetDevSnmpWid::initWidget(void)
{
   mSnmp = mDataPacket->net->snmp;

   QString str = mSnmp->get->get();
   ui->getEdit->setText(str);

   str = mSnmp->set->get();
   ui->setEdit->setText(str);

   str = mSnmp->trap1->get();
   ui->trap1Edit->setText(str);

   str = mSnmp->trap2->get();
   ui->trap2Edit->setText(str);
    ui->wholeCheckBox->setChecked(false);
}


/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void SetDevSnmpWid::updateSlot(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    if(mDataPacket)
        initWidget();
}


/**
 * @brief 检查网络数据是否合法
 * @return
 */
bool SetDevSnmpWid::checkData(void)
{
    QString str = ui->getEdit->text();
    if(str.isEmpty()) {
        QString str = tr("设备GET共同体错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->setEdit->text();
    if(str.isEmpty()) {
        QString str = tr("设备SET共同体错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->trap1Edit->text();
    bool ret = isIPaddress(str);
    if(!ret) {
        QString str = tr("Trap1地址错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    str = ui->trap2Edit->text();
    ret = isIPaddress(str);
    if(!ret) {
        QString str = tr("Trap2地址设置错误, 请重新输入!");
        CriticalMsgBox(this,"\n" + str + "\n");
        return false;
    }

    return true;
}


/**
 * @brief 发送网络模式数据
 * @return
 */
void SetDevSnmpWid::sentNetData(int fn,QString &str)
{

    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
    pkt.fn[0] = 7; // 设备用户信息
    pkt.fn[1] = 0x80; // SNMP
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
 * @brief
 * @return
 */
bool SetDevSnmpWid::saveNetData(void)
{
    int fn = 2, flag=0;
    QString str = ui->getEdit->text();
    //        if(str != mSnmp->get->get())
    {
        mSnmp->get->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->setEdit->text();
    //        if(str != mSnmp->set->get())
    {
        mSnmp->set->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->trap1Edit->text();
    //        if(str != mSnmp->trap1->get())
    {
        mSnmp->trap1->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    str = ui->trap2Edit->text();
    //        if(str != mSnmp->trap2->get())
    {
        mSnmp->trap2->set(str);
        sentNetData(fn, str);
        flag++;
    }
    fn += 1;

    bool ret = get_tcp_connect(mDataPacket->ip->get());
    if(ret)
    {
        //    if(flag) {
        QString str;
        if(ret)
            str = tr("设备SNMP修改成功!");
        else
            str = tr("设备SNMP修改失败!");
        InfoMsgBox box(this,str + "\n");
        //    }
    }
    return ret;
}



/**
 * @brief 增加日志信息
 */
void SetDevSnmpWid::saveLog(void)
{
   /*增加日志信息*/
   s_RecordLog log;
   log.title = tr("设备配置");
   log.operation = tr("设备SNMP修改");

   log.msg = tr("设备IP：%1 设备SNMP信息已修改").arg(mDataPacket->ip->get());
   sql_insert_record(log);
}


bool SetDevSnmpWid::saveSnmp(void)
{
    bool ret = checkData();
    if(ret) {
        ret = saveNetData();
        if(ret)
            saveLog();
    }
    return ret;
}


void SetDevSnmpWid::on_saveBtn_clicked()
{
    if(mDataPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            saveSnmp();
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
