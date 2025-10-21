/*
 * setswitchdlg.cpp
 * 输出位开关控制窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setswitchdlg.h"
#include "ui_setswitchdlg.h"

setSwitchDlg::setSwitchDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setSwitchDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("开关设置"));
    //    this->setStyleSheet("background-color:white");
    groupBox_background_icon(this);
    oldSw = true;
    btnBlue_style_sheet(this);
}

setSwitchDlg::~setSwitchDlg()
{
    delete ui;
}

/**
 * @brief 设置工作模式
 * @param mode
 */
void setSwitchDlg::setMode(int line,QString &name,PduDataPacket *packet)
{
    QString str = tr("输出位(%1)开关控制").arg(line+1);
    ui->titleLabel->setText(str);
    ui->nameLabel->setText(name);

    mPacket = packet;
    mSw = mPacket->manage->sw;
    int ret = mPacket->data->output->sw->get(line);
    if(ret == 1) // 关
        oldSw = false;

    if(oldSw)
        ui->openRadio->setChecked(true);
    else
        ui->closeRadio->setChecked(true);

    mLine = line;
}


/**
 * @brief 增加日志信息
 */
void setSwitchDlg::saveLog(void)
{
    s_RecordLog log;
    log.title = tr("设备配置");
    log.operation = tr("开关控制");

    QString str = tr("设备IP：") + mPacket->ip->get();
    int num = mPacket->devNum;
    if(num)
        str += tr(" 副机:%1").arg(QString::number(num));

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一修改
    {
        str += tr(": 开关状态统一修改");
        if(ui->wholeCheckBox->isChecked())
            str += tr(" 开关状态全局设置");
    }
    else
    {
        if(ui->wholeCheckBox->isChecked())
            str += tr(" 开关状态全局设置");
        str += ": " + ui->titleLabel->text();

    }

    bool sw =  ui->openRadio->isChecked();
    if(sw)
        str += tr(" 打开");
    else
        str += tr(" 关闭");
    log.msg = str;
    sql_insert_record(log);
}

/**
 * @brief 获取数据
 * @param buf
 * @return
 */
uchar *setSwitchDlg::getData(uchar *buf)
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
bool setSwitchDlg::sentData(void)
{    
    net_dev_data pkt;
    pkt.addr = mPacket->devNum;
    pkt.fn[0] = 13; // 输出位功能码

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一设置
    {
        pkt.fn[0] = 18;
        pkt.fn[1] = 0;
    }
    else
        pkt.fn[1] = mLine+1;
    pkt.len = 1;

    uchar data[12] = {0};
    pkt.data = getData(data);

    uchar buf[64] = {0};


    on = ui->wholeCheckBox->isChecked();
    if(on) // 广播数据包
    {
        pkt.addr = 0xff;
        int len = net_data_packets(mPacket->devType, TRA_TYPR_UDP, &pkt, buf);
        udp_queue_append(buf, len);
    } else {
        int len = net_data_packets(mPacket->devType, TRA_TYPR_TCP, &pkt, buf);
        tcp_queue_append(buf, len);
    }

    QString str;
    bool ret = get_tcp_connect(mPacket->ip->get());
    if(ret)
        str = tr("修改成功!");
    else
        str = tr("修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}

/**
 * @brief 保存数据
 */
void setSwitchDlg::saveData(void)
{    
    int value = 2;
    bool sw =  ui->openRadio->isChecked();
    if(!sw)
        value = 1;

    bool ret = ui->uniteCheckBox->isChecked();
    if(ret) { // 统一设置
        mSw->setSW(sw);
        mPacket->data->output->sw->setAll(value);
    } else {
        mSw->set(mLine, sw);
        mPacket->data->output->sw->set(mLine,value);
    }

    ret = sentData();
    if(ret)
        saveLog();
}

/**
 * @brief 保存按钮响应函数
 */
void setSwitchDlg::on_saveBtn_clicked()
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
    s_RecordLog log;
    log.title = QObject::tr("设备配置");
    log.operation = QObject::tr("开关控制");

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
    log.msg = str;
    sql_insert_record(log);
}

/**
 * @brief 控制PDU输出位的开关
 * @param ip IP地址
 * @param devNum 设备级联号
 * @param bit 输出位  n表示第n-1位(0表示所有输出位)
 * @param sw 1表示开，0表示关
 */
bool set_pdu_outputSw_two(QString &ip,int devNum, int bit, int sw)
{
    net_dev_data pkt;
    pkt.addr = devNum;
    pkt.fn[0] = 13; // 输出位功能码
    pkt.fn[1] = bit;
    pkt.len = 1;

    uchar data[12] = {sw};
    pkt.data = data;

    uchar buf[64] = {0};
    int devType = pdu_hd_getDevType(ip);
    int len = net_data_packets(devType, TRA_TYPR_TCP, &pkt, buf);
    bool ret = TcpSent::bulid()->onceSend(ip, buf,len);
    if(ret) {
        sql_save_swLog(ip,devNum,bit, sw);
    }

    return ret;
}

bool set_pdu_outputSw(const QString &ip,int devNum, int bit, bool sw)
{
    int cmd = 0;
    if(sw) cmd = 1;

    return set_pdu_outputSw_two(QString(ip), devNum, bit, cmd);
}
