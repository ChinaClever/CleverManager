/*
 * setthresholddlg.cpp
 * 设备阈值设置窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setthresholddlg.h"
#include "ui_setthresholddlg.h"

SetThresholdDlg::SetThresholdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetThresholdDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("阈值设置"));
    mRate = 1;
    groupBox_background_icon(this);
}

SetThresholdDlg::~SetThresholdDlg()
{
    delete ui;
}

/**
 * @brief 初始化数据
 * @param line
 */
void SetThresholdDlg::initData(int line)
{
    int data = mUnit->min->get(line);
    if(data >= 0)
        ui->minSpin->setValue(data/mRate);

    data = mUnit->max->get(line);
    if(data >= 0)
        ui->maxSpin->setValue(data/mRate);

    mLine = line;
}

/**
 * @brief 设置工作模式
 * @param mode
 */
void SetThresholdDlg::setMode(int mode, int line,PduDataPacket *packet)
{
    QString title, str;
    int bit = 0, max=99;

    mMode = mode;
    switch (mode)
    {
    case SET_CMD_LINE_VOL: // 相电压
        mUnit = packet->data->line->vol;
        title = tr("电压(L%1)阈值设置").arg(line+1);
        str = "V"; // 单位
        max = 400; // 最大值
        mRate = COM_RATE_VOL;
        break;

    case SET_CMD_LINE_CUR:  // 相电流
        title = tr("电流(L%1)阈值设置").arg(line+1);
        mUnit = packet->data->line->cur;
        str = "A";
        bit = 1;
        max = 64;
        mRate = COM_RATE_CUR;
        break;

    case SET_CMD_ENV_TEM:  // 温度
        title = tr("温度(%1)阈值设置").arg(line+1);
        mUnit = packet->data->env->tem;
        str = "℃";
        max = 99;
        mRate = COM_RATE_TEM;
        break;

    case SET_CMD_ENV_HUM:  // 湿度
        title = tr("湿度(%1)阈值设置").arg(line+1);
        mUnit = packet->data->env->hum;
        str = "%";
        max = 99;
        mRate = COM_RATE_HUM;
        break;

    default:
        return;
    }

    initData(line);
    ui->titleLabel->setText(title);

    mSymbol = str;
    ui->minSpin->setSuffix(str);
    ui->maxSpin->setSuffix(str);

    ui->minSpin->setDecimals(bit);
    ui->maxSpin->setDecimals(bit);

    ui->minSpin->setMaximum(max);
    ui->maxSpin->setMaximum(max);
    mPacket = packet;
}

/**
 * @brief 检查输入是否合法
 * @return true
 */
bool SetThresholdDlg::checkData(void)
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
void SetThresholdDlg::getModeStr(QString &str)
{
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
}

/**
 * @brief 增加日志信息
 */
void SetThresholdDlg::saveLog(void)
{
    s_RecordLog log;
    log.title = tr("设备配置");
    log.operation = tr("阈值修改");
    QString str = tr("设备IP：") + mPacket->ip->get();

    int num = mPacket->devNum;
    if(num)
        str += tr(" 副机:%1").arg(num);

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一修改
       getModeStr(str);
    else
        str += ": " + ui->titleLabel->text();

    double min = ui->minSpin->value();
    double max = ui->maxSpin->value();
    str += tr(": 阈值 %1%2~%3%4").arg(min).arg(mSymbol).arg(max).arg(mSymbol);
    log.msg = str;
    sql_insert_record(log);
}

/**
 * @brief 获取数据
 * @param buf
 * @return
 */
uchar *SetThresholdDlg::getData(uchar *buf)
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
bool SetThresholdDlg::sentData(void)
{
    net_dev_data pkt;

    pkt.addr = mPacket->devNum;
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
    if(ui->wholeCheckBox->isChecked()) // 广播数据包
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
        str = tr("阈值修改成功!");
    else
        str = tr("阈值修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}

/**
 * @brief 保存数据
 */
void SetThresholdDlg::saveData(void)
{
    int min = ui->minSpin->value() * mRate;
    int max = ui->maxSpin->value() * mRate;

    bool ret = ui->uniteCheckBox->isChecked();
    if(ret) // 统一设置
    {
        mUnit->min->setAll(min);
        mUnit->max->setAll(max);
    }
    else
    {
        mUnit->min->set(mLine, min);
        mUnit->max->set(mLine, max);
    }

    ret = sentData();
    if(ret)
        saveLog();
}

void SetThresholdDlg::on_saveBtn_clicked()
{
    bool ret = checkData();
    if(ret)
    {
        saveData();
        this->accept();
    }
}
