/*
 * setoutputdlg.cpp
 * 输出位阈值设置窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setoutputdlg.h"
#include "ui_setoutputdlg.h"

SetOutputDlg::SetOutputDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetOutputDlg)
{
    is_loop=false;
    is_loopcur = false;
    ui->setupUi(this);
    this->setWindowTitle(tr("单元设置"));
    groupBox_background_icon(this);
    ui->uniteNameBox->setHidden(true);

    //    initWidget();
}

SetOutputDlg::~SetOutputDlg()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void SetOutputDlg::initWidget(void)
{
    QString title = tr("单元设置");
    ui->titleLabel->setText(title);


    if(is_loop && !is_loopcur)
    {
        QString str = "V";
        ui->minSpin->setSuffix(str);
        ui->maxSpin->setSuffix(str);
        ui->crMinSpin->setSuffix(str);
        ui->crMaxSpin->setSuffix(str);

        int bit = 1;
        ui->minSpin->setDecimals(bit);
        ui->maxSpin->setDecimals(bit);
        ui->crMinSpin->setDecimals(bit);
        ui->crMaxSpin->setDecimals(bit);
    }else
    {
        QString str = "A";
        ui->minSpin->setSuffix(str);
        ui->maxSpin->setSuffix(str);
        ui->crMinSpin->setSuffix(str);
        ui->crMaxSpin->setSuffix(str);

        int bit = 1;
        ui->minSpin->setDecimals(bit);
        ui->maxSpin->setDecimals(bit);
        ui->crMinSpin->setDecimals(bit);
        ui->crMaxSpin->setDecimals(bit);

        int max=16;
        ui->minSpin->setMaximum(max);
        ui->maxSpin->setMaximum(max);
        ui->crMinSpin->setMaximum(max);
        ui->crMaxSpin->setMaximum(max);
    }
}

/**
 * @brief 初始化数据
 * @param line
 */
void SetOutputDlg::initData(int line)
{
    if(!is_loop)
        mUnit = mPacket->data->output->cur;
    else
    {
        if(is_loopcur)
            mUnit=mPacket->data->loop->cur;
        else
            mUnit=mPacket->data->loop->vol;
    }

    int data = mUnit->min->get(line);
    if(data >= 0)
        ui->minSpin->setValue(data/COM_RATE_CUR);

    data = mUnit->max->get(line);
    if(data >= 0)
        ui->maxSpin->setValue(data/COM_RATE_CUR);

    data = mUnit->crMin->get(line);
    if(data >= 0)
        ui->crMinSpin->setValue(data/COM_RATE_CUR);
    else
        ui->crMinSpin->setDisabled(true);

    data = mUnit->crMax->get(line);
    if(data >= 0)
        ui->crMaxSpin->setValue(data/COM_RATE_CUR); //设置临界上值
    else
        ui->crMaxSpin->setDisabled(true);

    mLine = line;
}

/**
 * @brief 设置工作模式
 * @param mode
 */
void SetOutputDlg::setMode(int line,QString &name,PduDataPacket *packet)//line在输出位中代表输出位，在回路中就代表某一回路
{
    mName = name;
    ui->nameEdit->setText(name);

    mPacket = packet;
    //    qDebug()<<"mpacket:"<<mPacket->data->loop->cur->crMax->get(line);
    initWidget();//再初始化一次界mian
    initData(line);
}

/**
 * @brief 针对回路电流阈值设置，改变title，同时隐藏上下限界
 * @param name
 */
void SetOutputDlg::setLoopMode()
{

}

/**
 * @brief 输出位或者回路
 * @param name
 */
void SetOutputDlg::setName(QString &name,bool is_cur) //同时设置为回路
{
    ui->label_4->setText(name);
    ui->titleLabel->setText(tr("回路设置"));
    ui->nameEdit->setEnabled(false);
    is_loop = true;

    //清空临界值，保留最小最大
    ui->widget->setVisible(false);

    if(is_cur)
        is_loopcur = true;
    else
        is_loopcur = false;


}

/**
 * @brief 检查输入是否合法
 * @return true
 */
bool SetOutputDlg::checkData(void)
{
    int min = ui->minSpin->value() * COM_RATE_CUR;
    int max = ui->maxSpin->value() * COM_RATE_CUR;

    QString str = "\n" + ui->titleLabel->text() + tr("错误!") + "\n";
    if(min > max)
    {
        str += tr("最小值大于最大值, 请重新输入!");
        CriticalMsgBox(this,str + "\n");
        return false;
    }

    if(ui->crMinSpin->isEnabled())
    {
        int crMin = ui->crMinSpin->value() * 10;
        int crMax = ui->crMaxSpin->value() * 10;
        if(crMin > crMax)
        {
            str += tr("临界最小值大于临界最大值, 请重新输入!");
            CriticalMsgBox(this,str + "\n");
            return false;
        }

        if(crMin < min)
        {
            str += tr("临界最小值小于最小值, 请重新输入!");
            CriticalMsgBox(this,str + "\n");
            return false;
        }

        if(crMax > max)
        {
            str += tr("临界最大值大于最大值, 请重新输入!");
            CriticalMsgBox(this,str + "\n");
            return false;
        }
    }

    QString name = ui->nameEdit->text();
    if(name.isEmpty())
    {
        str += tr("输出位名称不能为空, 请重新输入!");
        CriticalMsgBox(this,str + "\n");
        return false;
    }

    return true;
}

/**
 * @brief 增加日志信息
 */
void SetOutputDlg::saveLog(void)
{
    s_RecordLog log;
    if(is_loop)
    {
        log.title = tr("回路设置");
        log.operation = tr("回路阈值修改");
    }
    else
    {
        log.title = tr("设备配置");
        log.operation = tr("单元阈值修改");
    }

    QString str = tr("设备IP：") + mPacket->ip->get();
    int num = mPacket->devNum;
    if(num)
        str += tr(" 副机:%1").arg(QString::number(num));

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
    log.msg = str;
    sql_insert_record(log);
}

/**
 * @brief 获取数据
 * @param buf
 * @return
 */
uchar *SetOutputDlg::getData(uchar *buf)
{
    int offset=0;

    int min = ui->minSpin->value() * COM_RATE_CUR;
    buf[offset++] = (min >> 8)&0xFF;  //高八位
    buf[offset++] = min & 0xFF;  //低八位

    int max = ui->maxSpin->value() * COM_RATE_CUR;
    buf[offset++] = (max >> 8)&0xFF;
    buf[offset++] = max & 0xFF;

    if(!is_loop) //如果不是回路，那么发送临界数据
    {
        int crMin = ui->crMinSpin->value() * COM_RATE_CUR;
        buf[offset++] = (crMin >> 8)&0xFF;
        buf[offset++] = crMin & 0xFF;

        int crMax = ui->crMaxSpin->value() * COM_RATE_CUR;
        buf[offset++] = (crMax >> 8)&0xFF;
        buf[offset++] = crMax & 0xFF;

    }

    return buf;
}

/**
 * @brief 发送设备数据
 * @return
 */
bool SetOutputDlg::sentData(void)
{
    net_dev_data pkt;

    pkt.addr = mPacket->devNum;
    if(is_loop)
    {
        if(is_loopcur)
            pkt.fn[0]=0x71;  //回路
        else
            pkt.fn[0]=0x73;
    }
    else
        pkt.fn[0] = 0; // 输出位功能码

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一设置
        pkt.fn[1] = 0;
    else
        pkt.fn[1] = mLine+1;
    pkt.len = 8;

    uchar data[32] = {0};
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
        str = tr("阈值修改成功!");
    else
        str = tr("阈值修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}

/**
 * @brief 保存数据
 */
void SetOutputDlg::saveData(void)
{
    int min = ui->minSpin->value() * COM_RATE_CUR;
    int max = ui->maxSpin->value() * COM_RATE_CUR;
    int crMin = ui->crMinSpin->value() * COM_RATE_CUR;
    int crMax = ui->crMaxSpin->value() * COM_RATE_CUR;

    bool ret = ui->uniteCheckBox->isChecked();
    if(ret) // 统一设置
    {
        mUnit->min->setAll(min);
        mUnit->max->setAll(max);
        if(ui->crMinSpin->isEnabled())
        {
            mUnit->crMin->setAll(crMin);
            mUnit->crMax->setAll(crMax);
        }
    }
    else
    {
        mUnit->min->set(mLine, min);
        mUnit->max->set(mLine, max);
        if(ui->crMinSpin->isEnabled())
        {
            mUnit->crMin->set(mLine, crMin);
            mUnit->crMax->set(mLine, crMax);
        }
    }

    ret = sentData();
    if(ret)
        saveLog();
}

/**
 * @brief 增加日志信息
 */
void SetOutputDlg::saveNameLog(void)
{
    s_RecordLog log;
    log.title = tr("设备配置");
    log.operation = tr("输出位名称修改");

    QString str = tr("设备IP：") + mPacket->ip->get();
    int num = mPacket->devNum;
    if(num)
        str += tr(" 副机:%1").arg(QString::number(num));

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一修改
        str += tr(": 输出位名称修改");
    else
        str += ": " + ui->titleLabel->text();

    str += tr(":  输出位名称%1").arg(mName);
    log.msg = str;
    sql_insert_record(log);
}

/**
 * @brief 发送设备数据
 * @return
 */
bool SetOutputDlg::sentNameData(void)
{
    net_dev_data pkt;

    pkt.addr = mPacket->devNum;
    pkt.fn[0] = 10; // 输出位功能码

    bool on = ui->uniteCheckBox->isChecked();
    if(on) // 统一设置
        pkt.fn[1] = 0;
    else
        pkt.fn[1] = mLine+1;
    pkt.len = mName.size();

    char data[32] = {0};
    for(int i=0; i<mName.size(); ++i)
        data[i] = mName.at(i).toLatin1();
    pkt.data = (uchar *)data;

    uchar buf[64] = {0};


    on = ui->wholeCheckBox->isChecked();
    if(on) // 广播数据包
    {
        int len = net_data_packets(mPacket->devType, TRA_TYPR_UDP, &pkt, buf);
        udp_queue_append(buf, len);
    }
    else
    {
        int len = net_data_packets(mPacket->devType, TRA_TYPR_TCP, &pkt, buf);
        tcp_queue_append(buf, len);
    }

    QString str;
    bool ret = get_tcp_connect(mPacket->ip->get());
    if(ret)
        str = tr("输出位名称修改成功!");
    else
        str = tr("输出位名称修改失败!");
    InfoMsgBox box(this,str + "\n");

    return ret;
}

/**
 * @brief 保存输出位名称
 */
void SetOutputDlg::saveName(void)
{
    QString name = ui->nameEdit->text();
    if(name != mName)
    {
        mName = name;
        bool ret = ui->uniteNameBox->isChecked();
        if(ret) // 统一设置
            mPacket->output->name->setAll(name);
        else
            mPacket->output->name->set(mLine, name);

        ret = sentNameData();
        if(ret)
            saveNameLog();
    }
}

void SetOutputDlg::on_saveBtn_clicked()
{
    bool ret = checkData();
    if(ret)
    {
        saveData();
        saveName();
        this->accept();
    }
}


