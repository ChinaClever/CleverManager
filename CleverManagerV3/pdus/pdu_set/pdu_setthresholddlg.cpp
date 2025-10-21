/*
 * setthresholddlg.cpp
 * 设备阈值设置窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setthresholddlg.h"
#include "ui_pdu_setthresholddlg.h"

Pdu_SetThresholdDlg::Pdu_SetThresholdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pdu_SetThresholdDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("阈值设置"));
    groupBox_background_icon(this);
    button_style_sheet(ui->saveBtn);
    button_style_sheet(ui->cancelBtn);
    mPacket = nullptr;
    mUnit = nullptr;
    mRate = 1;
}

Pdu_SetThresholdDlg::~Pdu_SetThresholdDlg()
{
    delete ui;
}

/**
 * @brief 初始化数据
 * @param id
 */
void Pdu_SetThresholdDlg::initData(int id)
{
    int data = mUnit->min[id];
    if(data >= 0) ui->minSpin->setValue(data/mRate);

    data = mUnit->max[id];
    if(data >= 0) ui->maxSpin->setValue(data/mRate);

    mLine = id;
}

/**
 * @brief 设置工作模式
 * @param mode
 */
void Pdu_SetThresholdDlg::setMode(int mode, int id, sDataPacket *packet)
{
    QString title, str;
    int bit = 0, max=99;

    mMode = mode;
    switch (mode)
    {
    case SET_CMD_LINE_VOL: // 相电压
        mUnit = &(packet->data.line.vol);
        title = tr("电压(L%1)阈值设置").arg(id+1);
        str = "V"; // 单位
        max = 400; // 最大值
        mRate = COM_RATE_VOL;
        break;

    case SET_CMD_LINE_CUR:  // 相电流
        title = tr("电流(L%1)阈值设置").arg(id+1);
        mUnit = &(packet->data.line.cur);
        str = "A";
        bit = 1;
        max = 64;
        mRate = COM_RATE_CUR;
        break;

    case SET_CMD_ENV_TEM:  // 温度
        title = tr("温度(%1)阈值设置").arg(id+1);
        mUnit = &(packet->data.env.tem);
        str = "℃";
        max = 99;
        mRate = COM_RATE_TEM;
        break;

    case SET_CMD_ENV_HUM:  // 湿度
        title = tr("湿度(%1)阈值设置").arg(id+1);
        mUnit = &(packet->data.env.hum);
        str = "%";
        max = 99;
        mRate = COM_RATE_HUM;
        break;

    default:
        qDebug()<<"Pdu_SetThresholdDlg::setMode err" << mode;
        return;
    }

    initData(id);
    ui->titleLabel->setText(title);
    setSpin(str, bit, max);

    mPacket = packet;
}


void Pdu_SetThresholdDlg::setSpin(const QString &str, int bit, int max)
{
    ui->minSpin->setSuffix(str);
    ui->maxSpin->setSuffix(str);

    ui->minSpin->setDecimals(bit);
    ui->maxSpin->setDecimals(bit);

    ui->minSpin->setMaximum(max);
    ui->maxSpin->setMaximum(max);
}



/**
 * @brief 检查输入是否合法
 * @return true
 */
bool Pdu_SetThresholdDlg::checkData()
{
    int min = ui->minSpin->value() * mRate;
    int max = ui->maxSpin->value() * mRate;

    QString str = "\n" + ui->titleLabel->text() + tr("错误!") + "\n";
    if(min > max)
    {
        str += tr("最小值大于最大值, 请重新输入!");
        CriticalMsgBox(this,str + "\n");
        return false;
    }

    return true;
}

/**
 * @brief 获取统一设置时名称
 * @param str
 */
QString Pdu_SetThresholdDlg::getModeStr()
{
    QString str;
    switch (mMode)
    {
    case SET_CMD_LINE_VOL: // 相电压
        str += tr(": 电压阈值设置");
        break;
    case SET_CMD_LINE_CUR:
        str += tr(": 电流阈值设置");
        break;

    case SET_CMD_ENV_TEM: // 温度
        str += tr(": 温度阈值设置");
        break;

    case SET_CMD_ENV_HUM: // 湿度
        str += tr(": 湿度阈值设置");
        break;
    }

    return str;
}

/**
 * @brief 增加日志信息
 */
void Pdu_SetThresholdDlg::saveLog()
{
    sUserLogItem log;
    log.remarks = tr("阈值修改：");
    QString str = tr("设备IP：") + mPacket->net.ip;

    int num = mPacket->id;
    if(num) str += tr(" 副机:%1").arg(num);

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一修改
       str += getModeStr();
    else
        str += ": " + ui->titleLabel->text();

    double min = ui->minSpin->value();
    double max = ui->maxSpin->value();    
    QString sym = ui->minSpin->suffix();
    str += tr(": 阈值 %1%2~%3%4").arg(min).arg(sym).arg(max).arg(sym);

    log.remarks += str;
    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 获取数据
 * @param buf
 * @return
 */
uchar *Pdu_SetThresholdDlg::getData(uchar *buf)
{
    int offset=0, rate = mRate;

    int min = ui->minSpin->value() * rate;
    buf[offset++] = (min >> 8)&0xFF;
    buf[offset++] = min & 0xFF;

    int max = ui->maxSpin->value() * rate;
    buf[offset++] = (max >> 8)&0xFF;
    buf[offset++] = max & 0xFF;

    return buf;
}

/**
 * @brief 发送设备数据
 * @return
 */
bool Pdu_SetThresholdDlg::sentData()
{
    Net_sDevData pkt;

    pkt.addr = mPacket->id;
    pkt.fn[0] = mMode;

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一设置
        pkt.fn[1] = 0;
    else
        pkt.fn[1] = mLine+1;
    pkt.len = 4;

    uchar data[32] = {0};
    pkt.data = getData(data);

    uchar buf[64] = {0};
    on = ui->wholeCheckBox->isChecked();
    if(on) { // 广播数据包
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
    InfoMsgBox box(this, tr("阈值修改成功!"));

    return true;
}

/**
 * @brief 保存数据
 */
void Pdu_SetThresholdDlg::saveData()
{
    int min = ui->minSpin->value() * mRate;
    int max = ui->maxSpin->value() * mRate;

    bool ret = ui->uniteCheckBox->isChecked();
    if(ret) // 统一设置
    {
        for(int i=0; i<mUnit->size; ++i) {
            mUnit->min[i] = min;
            mUnit->max[i] = max;
        }
    }  else {
        mUnit->min[mLine] = min;
        mUnit->max[mLine] = max;
    }

    ret = sentData();
    if(ret) saveLog();
}

void Pdu_SetThresholdDlg::on_saveBtn_clicked()
{
    bool ret = checkData();
    if(ret) {
        saveData();
        this->accept();
    }
}
