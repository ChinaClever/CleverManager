/*
 * setoutputdlg.cpp
 * 输出位阈值设置窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setoutputdlg.h"
#include "ui_pdu_setoutputdlg.h"

Pdu_SetOutputDlg::Pdu_SetOutputDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pdu_SetOutputDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("单元设置"));
    groupBox_background_icon(this);
}

Pdu_SetOutputDlg::~Pdu_SetOutputDlg()
{
    delete ui;
}


/**
 * @brief 初始化数据
 * @param id
 */
void Pdu_SetOutputDlg::initData(int id)
{
    sDataUnit *unit = &(mPacket->data.output.cur);

    int data = unit->min[id];
    if(data >= 0)
        ui->minSpin->setValue(data/COM_RATE_CUR);

    data = unit->max[id];
    if(data >= 0)
        ui->maxSpin->setValue(data/COM_RATE_CUR);

    data = unit->crMin[id];
    if(data >= 0)
        ui->crMinSpin->setValue(data/COM_RATE_CUR);
    else
        ui->crMinSpin->setDisabled(true);

    data = unit->crMax[id];
    if(data >= 0)
        ui->crMaxSpin->setValue(data/COM_RATE_CUR); //设置临界上值
    else
        ui->crMaxSpin->setDisabled(true);

    mId = id;
}

/**
 * @brief 设置工作模式
 * @param mode
 */
void Pdu_SetOutputDlg::setMode(int id, sDataPacket *packet)
{
    mPacket = packet;
    initData(id);
}



/**
 * @brief 检查输入是否合法
 * @return true
 */
bool Pdu_SetOutputDlg::checkData()
{
    int min = ui->minSpin->value() * COM_RATE_CUR;
    int max = ui->maxSpin->value() * COM_RATE_CUR;

    QString str = "\n" + ui->titleLabel->text() + tr("错误!") + "\n";
    if(min > max) {
        str += tr("最小值大于最大值, 请重新输入!");
        CriticalMsgBox(this,str + "\n");
        return false;
    }

    if(ui->crMinSpin->isEnabled())
    {
        int crMin = ui->crMinSpin->value() * 10;
        int crMax = ui->crMaxSpin->value() * 10;
        if(crMin > crMax)  {
            str += tr("临界最小值大于临界最大值, 请重新输入!");
            CriticalMsgBox(this,str + "\n");
            return false;
        }

        if(crMin < min) {
            str += tr("临界最小值小于最小值, 请重新输入!");
            CriticalMsgBox(this,str + "\n");
            return false;
        }

        if(crMax > max) {
            str += tr("临界最大值大于最大值, 请重新输入!");
            CriticalMsgBox(this,str + "\n");
            return false;
        }
    }

    return true;
}

/**
 * @brief 增加日志信息
 */
void Pdu_SetOutputDlg::saveLog()
{
    sUserLogItem log;
    log.remarks = tr("单元阈值修改：");

    QString str = tr("设备IP：") + mPacket->net.ip;
    int num = mPacket->id;
    if(num) str += tr(" 副机:%1").arg(QString::number(num));

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一修改
        str += tr(": 单元阈值修改");
    else
        str += ": " + ui->titleLabel->text();

    double min = ui->minSpin->value();
    double max = ui->maxSpin->value();
    str += tr(": 报警阈值:%1A~%2A ").arg(min).arg(max);

    if(ui->crMinSpin->isEnabled())
    {
        double crMin = ui->crMinSpin->value();
        double crMax = ui->crMaxSpin->value();
        str += tr(": 临界阈值:%1A~%2A ").arg(crMin).arg(crMax);
    }
    log.remarks += str;
    DbUserLog::bulid()->insertItem(log);
}

/**
 * @brief 获取数据
 * @param buf
 * @return
 */
uchar *Pdu_SetOutputDlg::getData(uchar *buf)
{
    int offset=0;

    int min = ui->minSpin->value() * COM_RATE_CUR;
    buf[offset++] = (min >> 8)&0xFF;  //高八位
    buf[offset++] = min & 0xFF;  //低八位

    int max = ui->maxSpin->value() * COM_RATE_CUR;
    buf[offset++] = (max >> 8)&0xFF;
    buf[offset++] = max & 0xFF;

    int crMin = ui->crMinSpin->value() * COM_RATE_CUR;
    buf[offset++] = (crMin >> 8)&0xFF;
    buf[offset++] = crMin & 0xFF;

    int crMax = ui->crMaxSpin->value() * COM_RATE_CUR;
    buf[offset++] = (crMax >> 8)&0xFF;
    buf[offset++] = crMax & 0xFF;

    return buf;
}

/**
 * @brief 发送设备数据
 * @return
 */
bool Pdu_SetOutputDlg::sentData()
{
    Net_sDevData pkt;
    pkt.addr = mPacket->id;
    pkt.fn[0] = 0; // 输出位功能码

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一设置
        pkt.fn[1] = 0;
    else
        pkt.fn[1] = mId+1;
    pkt.len = 8;

    uchar data[32] = {0};
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
    InfoMsgBox box(this, tr("阈值修改失败!"));

    return true;
}

/**
 * @brief 保存数据
 */
void Pdu_SetOutputDlg::saveData()
{
    int min = ui->minSpin->value() * COM_RATE_CUR;
    int max = ui->maxSpin->value() * COM_RATE_CUR;
    int crMin = ui->crMinSpin->value() * COM_RATE_CUR;
    int crMax = ui->crMaxSpin->value() * COM_RATE_CUR;

    sDataUnit *unit = &(mPacket->data.output.cur);
    bool ret = ui->uniteCheckBox->isChecked();
    if(ret) // 统一设置
    {
        for(int i=0; i<unit->size; ++i) {
            unit->min[i] = min;
            unit->max[i] = max;
            unit->crMin[i] = crMin;
            unit->crMax[i] = crMax;
        }
    } else {
        unit->min[mId] = min;
        unit->max[mId] = max;
        unit->crMin[mId] = crMin;
        unit->crMax[mId] = crMax;
    }

    ret = sentData();
    if(ret) saveLog();
}


void Pdu_SetOutputDlg::on_saveBtn_clicked()
{
    bool ret = checkData();
    if(ret) {
        saveData();
        this->accept();
    }
}


