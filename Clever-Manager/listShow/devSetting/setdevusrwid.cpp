/*
 * setdevusrwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setdevusrwid.h"
#include "ui_setdevusrwid.h"

SetDevUsrWid::SetDevUsrWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDevUsrWid)
{
    ui->setupUi(this);
    mDataPacket = NULL;
}

SetDevUsrWid::~SetDevUsrWid()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void SetDevUsrWid::initWidget(void)
{
    QStringList usrList;
    int rtn = mDataPacket->usr->usrHash->getUsr(usrList);
    if(rtn>0) {
        mUsrName = usrList.last();
        ui->usrEdit->setText(mUsrName);

        mPwd = mDataPacket->usr->usrHash->get(mUsrName)->pwd->get();
        ui->pwdEdit->setText(mPwd);
    }
    ui->wholeCheckBox->setChecked(false);
}

/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void SetDevUsrWid::updateSlot(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    if(mDataPacket)
        initWidget();
}

/**
 * @brief 检查输入是否合法
 * @return true
 */
bool SetDevUsrWid::checkLand(void)
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

/**
 * @brief 发送设备数据
 * @return
 */
bool SetDevUsrWid::sentLand(void)
{
    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
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

    QString str;
    bool ret = get_tcp_connect(mDataPacket->ip->get());
    if(ret)
        str = tr("设备登录信息修改成功!");
    else
        str = tr("设备登录信息修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}


/**
 * @brief 增加日志信息
 */
void SetDevUsrWid::saveLandLog(void)
{
   /*增加日志信息*/
   s_RecordLog log;
   log.title = tr("设备配置");
   log.operation = tr("设备登录信息修改");

   QString name = ui->usrEdit->text();
   log.msg = tr("设备IP：%1 登录账号%2 修改为：%3").arg(mDataPacket->ip->get()).arg(mUsrName).arg(name);
   sql_insert_record(log);
}

/**
 * @brief 修改设备名
 */
void SetDevUsrWid::setLand(void)
{
    QString name = ui->usrEdit->text();
    QString pwd = ui->pwdEdit->text();
//    if((mUsrName != name) || (mPwd != pwd)) {
        bool ret = checkLand();
        if(ret) {
            ret = sentLand();
            if(ret) {
                saveLandLog();                
                mDataPacket->usr->usrHash->setPwd(name, pwd);
            }
        }
//    }
}



void SetDevUsrWid::on_saveBtn_clicked()
{
    if(mDataPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            setLand();
        } else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
