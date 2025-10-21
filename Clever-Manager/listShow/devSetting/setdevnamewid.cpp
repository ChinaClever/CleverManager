/*
 * setdevnamewid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setdevnamewid.h"
#include "ui_setdevnamewid.h"

SetDevNameWid::SetDevNameWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDevNameWid)
{
    ui->setupUi(this);
    mDataPacket = NULL;
}

SetDevNameWid::~SetDevNameWid()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void SetDevNameWid::initWidget(void)
{
    QString name = mDataPacket->info->type->name->get();
    ui->lineEdit->setText(name);

    QStringList list;
    list << tr("主机");
    for(int i=1; i<10; ++i)
        list << tr("副机 ") + QString::number(i);
    ui->comboBox->clear();
    ui->comboBox->addItems(list);

    int num = mDataPacket->devNum;
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
void SetDevNameWid::updateSlot(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    if(mDataPacket)
        initWidget();
}


/**
 * @brief 检查输入是否合法
 * @return true
 */
bool SetDevNameWid::checkDevName(void)
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

/**
 * @brief 发送设备数据
 * @return
 */
bool SetDevNameWid::sentDevName(void)
{
    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
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

    uchar buf[128] = {0};
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
        str = tr("设备名称修改成功!");
    else
        str = tr("设备名称修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}


/**
 * @brief 增加日志信息
 */
void SetDevNameWid::saveDevNameLog(void)
{
   /*增加日志信息*/
   s_RecordLog log;
   log.title = tr("设备配置");
   log.operation = tr("修改设备名");

   QString name = mDataPacket->info->type->name->get();
   QString newName = ui->lineEdit->text();
   log.msg = tr("设备IP：%1 设备名%2 改为：%3").arg(mDataPacket->ip->get()).arg(name).arg(newName);
   sql_insert_record(log);
}

/**
 * @brief 修改设备名
 */
void SetDevNameWid::renameDev(void)
{
    QString name = mDataPacket->info->type->name->get();
    QString newName = ui->lineEdit->text();
//    if(name != newName)    
    {
        bool ret = checkDevName();
        if(ret) {
            ret = sentDevName();
            if(ret) {
                saveDevNameLog();                
                mDataPacket->info->type->name->set(newName);
            }
        }
    }
}



/**
 * @brief 检查设备工作模式
 * @return true
 */
bool SetDevNameWid::checkDevMode(void)
{
    bool ret = false;

    int mode = ui->comboBox->currentIndex();
    int num = mDataPacket->devNum;
    if(mode != num)
        ret = true;

    return ret;
}



/**
 * @brief 发送设备工作模式
 * @return
 */
bool SetDevNameWid::sentDevMode(void)
{
    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
    pkt.fn[0] = 5; // 设备信息
    pkt.fn[1] = 0x10; // 设备类型
    pkt.fn[1] += 2; // 设备工作模式

    int mode = ui->comboBox->currentIndex();
    uchar data[4] = {mode};
    pkt.data = data;
    pkt.len = 1;

    uchar buf[32] = {0};

    if(mode==0) { // 主机模式时，才能统一设置
        bool on = ui->wholeCheckBox->isChecked();
        if(on) { // 广播数据包
            int len = net_data_packets(mDataPacket->devType, TRA_TYPR_UDP, &pkt, buf);
            udp_queue_append(buf, len);
        }
        else
        {
            int len = net_data_packets(mDataPacket->devType, TRA_TYPR_TCP, &pkt, buf);
            tcp_queue_append(buf, len);
        }
    }

    QString str;
    bool ret = get_tcp_connect(mDataPacket->ip->get());
    if(ret)
        str = tr("设备工作模式修改成功!");
    else
        str = tr("设备工作模式修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}



/**
 * @brief 增加日志信息
 */
void SetDevNameWid::saveDevModeLog(void)
{
   /*增加日志信息*/
   s_RecordLog log;
   log.title = tr("设备配置");
   log.operation = tr("修改工作模式");

   QString str;
   QString name = mDataPacket->info->type->name->get();
   int mode = ui->comboBox->currentIndex();
   if(mode)
       str = tr("改为副机 ") + QString::number(mode);
   else
       str = tr("主机模式");
   log.msg = tr("设备IP：%1 设备名%2 ：%3").arg(mDataPacket->ip->get()).arg(name).arg(str);
   sql_insert_record(log);
}

/**
 * @brief 修改设备模式
 */
void SetDevNameWid::setDevMode(void)
{
    bool ret = checkDevMode();
    if(ret) {
        ret = sentDevMode();
        if(ret) {
            saveDevModeLog();

            int mode = ui->comboBox->currentIndex();
            mDataPacket->devNum = mode;
        }
    }
}

/**
 * @brief 数据保存
 */
void SetDevNameWid::on_saveBtn_clicked()
{
    if(mDataPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            renameDev();
            setDevMode();
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
