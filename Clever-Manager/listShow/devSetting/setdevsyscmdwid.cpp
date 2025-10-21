/*
 * setdevsyscmdwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setdevsyscmdwid.h"
#include "ui_setdevsyscmdwid.h"
#include "updatedevdlg.h"
#include "dev/devType/pdudtname.h"

#if defined(ZEBRA_MODULE)
#include "dbpducommandchannel.h"
#endif

SetDevSysCmdWid::SetDevSysCmdWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDevSysCmdWid)
{
    ui->setupUi(this);
    mDataPacket = NULL;
}

SetDevSysCmdWid::~SetDevSysCmdWid()
{
    delete ui;
}


/**
 * @brief 初始化窗口
 */
void SetDevSysCmdWid::initWidget(void)
{
    ui->defaultCheckBox->setChecked(false);
    ui->resCheckBox->setChecked(false);
    ui->wholeCheckBox->setChecked(false);

#if defined(ZEBRA_MODULE)
    int dev = mDataPacket->devType;
    if((dev != PDU_TYPE_ZPDU) || mDataPacket->devNum)
        ui->channelCheckBox->hide();
    else
        ui->channelCheckBox->show();
#else
    ui->channelCheckBox->hide();
#endif
}

/**
 * @brief 更新设备
 * @param ip 设备地址
 * @param num 设备号
 */
void SetDevSysCmdWid::updateSlot(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    if(mDataPacket)
        initWidget();
}

/**
 * @brief 发送恢复出厂设置命令
 * @return
 */
void SetDevSysCmdWid::sentCmdData(int fn)
{
    net_dev_data pkt;

    pkt.addr = (uchar)(mDataPacket->devNum);
    pkt.fn[0] = 20; // 系统维护命令
    pkt.fn[1] = fn;

    uchar data[4] = {1};
    pkt.data = data;
    pkt.len = 1;

    uchar buf[32] = {0};
    bool on = ui->wholeCheckBox->isChecked();
    if(on) { // 广播数据包
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
bool SetDevSysCmdWid::saveNetData(void)
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

#if defined(ZEBRA_MODULE)
    ret = ui->channelCheckBox->isChecked();
    if(ret){
        QuMsgBox box(this, tr("恢复TCP通道时，ZPDU将自动重启，确认是否恢复TCP通道？"));
        if(box.Exec()) {
            sentCmdData(fn);

            bool on = ui->wholeCheckBox->isChecked();
            if(!on)
            {
                QString ip = mDataPacket->ip->get();
                DbPduCommandChannelItem qvec = DbPduCommandChannel::get()->selectItemByIp(ip);
                qvec.set_channel = 4;
                if(qvec.id < 0)  {
                    DbPduCommandChannel::get()->insertItem(qvec);
                } else{
                    DbPduCommandChannel::get()->updateItem(qvec);
                }
            }
            else
            {
                QVector<DbPduCommandChannelItem> qvecs = DbPduCommandChannel::get()->allItems();
                for(int i = 0 ; i < qvecs.size(); i++)
                {
                    DbPduCommandChannelItem qvec = qvecs[i];
                    qvec.set_channel = 4;
                    DbPduCommandChannel::get()->updateItem(qvec);
                }
            }
            flag++;
        }
    }
    fn += 1;
#endif

    ret = get_tcp_connect(mDataPacket->ip->get());
    if(flag) {
        QString str;
        if(ret)
            str = tr("设备命令执行成功!");
        else
            str = tr("设备命令执行失败!");
        InfoMsgBox box(this,str + "\n");
    }
    else
        InfoMsgBox box(this,tr("连接失败") + "\n");

    return ret;
}

/**
 * @brief 增加日志信息
 */
void SetDevSysCmdWid::saveLog(void)
{
    /*增加日志信息*/
    s_RecordLog log;
    log.title = tr("设备配置");
    log.operation = tr("设备维护");

    QString str;
    bool ret = ui->defaultCheckBox->isChecked();
    if(ret)
        str = tr("设备恢复出厂设置 ");

    ret = ui->resCheckBox->isChecked();
    if(ret)
        str += tr(" 设备重启");

    log.msg = tr("设备IP：%1 %2").arg(mDataPacket->ip->get()).arg(str);
    sql_insert_record(log);
}

/**
 * @brief 修改设备模式
 */
void SetDevSysCmdWid::setDevCmd(void)
{
    bool ret = ui->defaultCheckBox->isChecked();
    bool res = ui->resCheckBox->isChecked();
    bool rsetch = ui->channelCheckBox->isChecked();
    if(ret || res || rsetch) {
        ret = saveNetData();
        if(ret) {
            saveLog();
        }
    }
}

bool SetDevSysCmdWid::isChecked()
{
    bool ret = ui->defaultCheckBox->isChecked();
    if(ui->channelCheckBox->isChecked())
        ret = true;
    if(ui->resCheckBox->isChecked())
        ret = true;
    if(ui->wholeCheckBox->isChecked())
        ret = true;

    return ret;
}

/**
 * @brief 执行命令
 */
void SetDevSysCmdWid::on_saveBtn_clicked()
{
    if(mDataPacket) {
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

/**
 * @brief 设备升级
 */
void SetDevSysCmdWid::on_updateBtn_clicked()
{
    if(mDataPacket) {
        bool ret = usr_land_jur();  //管理权限
        if(ret) {
            UpdateDevDlg dlg(this);
            QString ip = mDataPacket->ip->get();
            dlg.setDevIp(ip, mDataPacket->devType);
            dlg.exec();
        }  else {
            QString str = tr("你无权进行此操作!!!");
            WaringMsgBox box(this,"\n" + str + "\n");
        }
    }
}
