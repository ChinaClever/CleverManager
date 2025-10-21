/*
 * ds_datadsplay.cpp
 * 设备数据显示
 *  1、通过公共接口，刷新数据显示updateDsp();
 *  2、使用定时器实现每一秒会自动刷新设备数据
 *  3、显示任何一相时，自动检查是否有异常情况
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_datadsplay.h"
#include "dev/devType/pdudtname.h"

DS_DataDsplay::DS_DataDsplay(QWidget *parent) : QWidget(parent)
{
    dspVLayout = new QVBoxLayout(this);
    createControl();
    layoutControl();
    setDefaultColor();

    QTimer::singleShot(1*1000,this,SLOT(initFun())); // 延时
}

DS_DataDsplay::~DS_DataDsplay()
{
    delete mPduTG;
    delete mPduData;
}


void DS_DataDsplay::initFun(void)
{
    mAlarm = 0;
    mDataPacket = NULL;
    mPduTG = new PduPacketTG();
    mPduData = new sPduTGDataStr;

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(1000);
}

/**
 * @brief 创建控件
 */
void DS_DataDsplay::createControl(void)
{
    QFont ft;
    ft.setPointSize(11);
    ft.setFamily("微软雅黑");

    ipLab = new QLabel(tr("设备IP：---"),this);
    ipLab->setMaximumHeight(30);
    ipLab->setMinimumWidth(255);
    ipLab->setFont(ft);

    devLab = new QLabel(tr("设备名：---"),this); // 设备名
    devLab->setMaximumHeight(30);
    devLab->setMinimumWidth(120);
    devLab->setFont(ft);

    groupLab= new QLabel(tr("机房：---"),this); // 所在机房和机柜
    groupLab->setMaximumHeight(30);
    groupLab->setMinimumWidth(180);
    groupLab->setFont(ft);

    lineLab = new QLabel(tr("相数:L--"),this); //相数
    lineLab->setMaximumHeight(30);
    lineLab->setMinimumWidth(55);
    lineLab->setFont(ft);

    mVolLcd = new DS_LcdNumWid(this);
    mVolLcd->setUnit(tr("电压"),"V");

    mCurLcd = new DS_LcdNumWid(this);
    mCurLcd->setUnit(tr("电流"),"A");

    mPowLcd = new DS_LcdNumWid(this);
    mPowLcd->setUnit(tr("功率"),"kW");

    ft.setPointSize(12);
    eleLab = new QLabel(tr("电能：--- kWh"),this);
    eleLab->setAlignment(Qt::AlignCenter);
    eleLab->setMaximumHeight(60);
    eleLab->setFont(ft);

    //    ft.setPointSize(11);
    tempLab = new QLabel(tr("温度：---"),this);
    tempLab->setAlignment(Qt::AlignCenter);
    tempLab->setMaximumHeight(40);
    tempLab->setFont(ft);

    humLab  = new QLabel(tr("湿度：---"),this);
    humLab->setAlignment(Qt::AlignCenter);
    humLab->setMaximumHeight(40);
    humLab->setFont(ft);

    mDevStatus = new DS_DevStatus(this);
}

/**
 * @brief 布局
 */
void DS_DataDsplay::layoutControl(void)
{
    QHBoxLayout *devHlayout = new QHBoxLayout;
    devHlayout->addWidget(ipLab);
    devHlayout->addWidget(devLab);
    devHlayout->addWidget(groupLab);
    devHlayout->addWidget(lineLab);
    devHlayout->setSpacing(10);
    devHlayout->setContentsMargins(10, 0, 0, 0);

    QHBoxLayout *linelayout_2 = new QHBoxLayout;
    linelayout_2->addWidget(mVolLcd);
    linelayout_2->addWidget(mCurLcd);
    linelayout_2->addWidget(mPowLcd);
    linelayout_2->setSpacing(0);
    linelayout_2->setContentsMargins(0, 40, 0, 0);

    QHBoxLayout *linelayout_3 = new QHBoxLayout;
    linelayout_3->addWidget(eleLab);
    linelayout_3->setSpacing(0);
    linelayout_3->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *linelayout_4 = new QHBoxLayout;
    linelayout_4->addWidget(tempLab);
    linelayout_4->addWidget(humLab);
    linelayout_4->setSpacing(0);
    linelayout_4->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *linelayout_5 = new QHBoxLayout;
    linelayout_5->addWidget(mDevStatus);
    linelayout_5->setSpacing(0);
    linelayout_5->setContentsMargins(0, 0, 0, 0);

    //    dspVLayout->setMargin(15);
    //    dspVLayout->setSpacing(15);
    dspVLayout->addLayout(devHlayout);
    dspVLayout->addLayout(linelayout_2);
    dspVLayout->addLayout(linelayout_3);
    dspVLayout->addLayout(linelayout_4);
    dspVLayout->addLayout(linelayout_5);
}

/**
 * @brief 显示设备信息
 */
void DS_DataDsplay::dsyDevInfo(void)
{
    QString str;
    if(mDataPacket) {
        QString typeName;
        PduDTName pduTypeName;
        bool ret = pduTypeName.getName(mDataPacket->devType, typeName);
        if(ret)
            str = tr("类型:") + typeName +"  ";
    }

    str += tr(" 设备IP:") + mDev.ip +"  ";
    if(mDev.num)
        str += tr(" 副机%1:").arg(mDev.num);
    else
        str += tr(" 主机");
    ipLab->setText(str);

    QString name;
    mDataPacket->info->type->name->get(name);
    if(mDev.num) name = "";
    str = tr("  设备名：")+name;
    devLab->setText(str);

    str.clear();
    mDataPacket->info->addr->area->get(name);
    if(!name.isEmpty()) {
        str = tr("机房：")+name;
    }
    mDataPacket->info->addr->cab->get(name);
    if(!name.isEmpty()) {
        str += tr("  机柜：")+name;
    }

    groupLab->setText(str);
}

/**
 * @brief 设置默认颜色
 */
void DS_DataDsplay::setDefaultColor(void)
{
    // QPalette curlcdpat = curLcd->palette();
    //  curlcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::green);
    // curLcd->setPalette(curlcdpat);
    mCurLcd->setStyleSheet("background-color: rgb(219,241,252)");

    //  QPalette vollcdpat = volLcd->palette();
    //  vollcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::green);
    //  volLcd->setPalette(vollcdpat);
    mVolLcd->setStyleSheet("background-color: rgb(219,241,252)");
    mPowLcd->setStyleSheet("background-color: rgb(219,241,252)");

    tempLab->setStyleSheet("color: rgb(0, 0, 0)");
    humLab->setStyleSheet("color: rgb(0, 0, 0)");
}

/**
 * @brief 设置报警色
 * @param alarm
 */
void DS_DataDsplay::setAlarmColor(int alarm)
{
    DS_LcdNumWid *lcd=NULL;

    if(alarm == 1)
        lcd = mCurLcd;
    else if(alarm == 2)
        lcd = mVolLcd;
    else if(alarm == 3)
       tempLab->setStyleSheet("color:red;");
    else if(alarm == 4)
       humLab->setStyleSheet("color:red;");

    if(lcd)
    {
        // QPalette lcdpat = lcd->palette(); //调色板
        /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
        // lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
        //  lcd->setPalette(lcdpat);  //设置当前窗口的调色板
        lcd->setStyleSheet("background-color: yellow");
    }
}


/**
 * @brief 检查报警类型
 */
void DS_DataDsplay::checkAlarm(void)
{
    static short alarm=0;

    if(alarm != mAlarm)
    {
        alarm = mAlarm;

        setDefaultColor();
        if(alarm &0x01)
            setAlarmColor(1);
        if(alarm &0x02)
            setAlarmColor(2);
        if(alarm &0x04)
            setAlarmColor(3);
        if(alarm &0x08)
            setAlarmColor(4);
    }
}


/**
 * @brief 检查设备的报警状态
 * @param line
 */
void DS_DataDsplay::checkStatus(void)
{
    mAlarm = 0;
    int line = mDev.line;
    if(line > 0)
    {
        line--;
        PduObjData *lineData = mDataPacket->data->line;
        int ret = pdu_data_alarmCheck(lineData->cur, line);
        if(ret > 0)
            mAlarm |= (1<<0);
        else
            mAlarm &= ~(1<<0);

        ret = pdu_data_alarmCheck(lineData->vol, line);
        if(ret > 0)
            mAlarm |= (1<<1);
        else
            mAlarm &= ~(1<<1);
    }

    PduEnvData *envData = mDataPacket->data->env;
    int ret = pdu_data_checkAlarm(envData->tem, 0);  // 检查温度数据
    if(ret > 0)
        mAlarm |= (1<<2);
    else
        mAlarm &= ~(1<<2);

    ret = pdu_data_checkAlarm(envData->hum, 0);  // 检查温度数据
    if(ret > 0)
        mAlarm |= (1<<3);
    else
        mAlarm &= ~(1<<3);

    checkAlarm();
}

/**
 * @brief 设备离线显示
 */
void DS_DataDsplay::clearShow(void)
{
    devLab->setText(tr("此设备已离线"));

    QFont ft_com;
    ft_com.setFamily("微软雅黑");
    ft_com.setPointSize(12);

    mVolLcd->setFont(ft_com);
    mCurLcd->setFont(ft_com);
    mPowLcd->setFont(ft_com);

    mCurLcd->display("---");
    mVolLcd->display("---");
    mPowLcd->display("---");
    eleLab->setText(tr("电能：---kWh"));

    tempLab->setText(tr("温度：---"));
    humLab->setText(tr("湿度：---"));
}

void DS_DataDsplay::updateView()
{
    QString str, volStr;
    short line = mDev.line; // 0 表示统计
    if(line == 0) {
        volStr = tr("平均");
        str = tr("总");
    }

    mVolLcd->setTitle(tr("%1电压").arg(volStr));    
    mCurLcd->setTitle(tr("%1电流").arg(str));
    mPowLcd->setTitle(tr("%1功率").arg(str));

}

/**
 * @brief 读取设备数据信息
 */
void DS_DataDsplay::readData(void)
{
    memset(mPduData, 0, sizeof(sPduTGDataStr));
    short line = mDev.line; // 0 表示统计
    if(line)
    {
        line--;
        PduObjData *lineData = mDataPacket->data->line;
        mPduData->vol = lineData->vol->value->get(line); // 电压数据
        mPduData->cur = lineData->cur->value->get(line); // 电流数据
        mPduData->pow = lineData->pow->get(line); // 功率数据
        mPduData->ele = lineData->ele->get(line); // 电能数据
        mPduData->pf = lineData->pf->get(line); // 功率因数

        PduEnvData *envData = mDataPacket->data->env; // 温湿度
        mPduData->tem = envData->tem->value->get(0);
        mPduData->hum = envData->hum->value->get(0);
    }
    else
    {
        sPduTGDataStr *data = mPduTG->getTgData(mDataPacket); // 获取统计数据
        if(data)
        {
            memcpy(mPduData, data, sizeof(sPduTGDataStr));
            delete data;
        }
    }
    updateView();
}
/**
 * @brief
 */
void DS_DataDsplay::updataTh()
{
    if(mDataPacket->devType == PDU_TYPE_IP_PDU)  // IP设备才显示温湿度
    {
        int data = mPduData->tem;
        if(data >= 0)
            tempLab->setText(tr("温度：")+QString::number(data/COM_RATE_TEM)+"℃");
        else
            tempLab->setText(tr("温度：-- ℃"));
        data = mPduData->hum;
        if(data >= 0)
            humLab->setText(tr("湿度： ")+QString::number(data/COM_RATE_HUM)+"%");
        else
            humLab->setText(tr("湿度：-- %"));

        tempLab->setHidden(false);
        humLab->setHidden(false);
    } else {
        tempLab->setHidden(true);
        humLab->setHidden(true);
    }
}

/**
 * @brief 刷新数据显示
 */
void DS_DataDsplay::updateData(void)
{    
    QString str = "---";
    short line = mDev.line;
    if(line)
        lineLab->setText(tr("相数：L")+QString::number(line));
    else
        lineLab->clear();
    readData(); // 读取设备数据

    int data = mPduData->cur;
    if(data >= 0)
        mCurLcd->display((data/COM_RATE_CUR));
    else
        mCurLcd->display("---");

    data = mPduData->vol;
    if(data >= 0)
        mVolLcd->display((data/COM_RATE_VOL));
    else
        mVolLcd->display("---");

    data = mPduData->pow;
    if(data >= 0) {
        double temp = data/COM_RATE_POW;

        QString str = QString::number(temp);
        if(str.size() > 4) // 调整显示位数
            mPowLcd->setDigitCount(5);
        else
            mPowLcd->setDigitCount(4);
        mPowLcd->display((data/COM_RATE_POW));
    }
    else
        mPowLcd->display("---");

    data = mPduData->ele;
    if(data >= 0) {
        if(mDev.line==0)
            str = tr("总");
        else
            str.clear();
        eleLab->setText(tr("%1电能：").arg(str) +QString::number(data/COM_RATE_ELE)+"kWh");
    }
    else
        eleLab->setText(tr("电能：--- kWh"));

    updataTh(); // 更新温湿度数据
}

/**
 * @brief 设备检查是否在线
 *      1、设备必须在线
 */
bool DS_DataDsplay::checkDev(void)
{
    bool ret = false;

    if(mDataPacket)
    {
        if(mDataPacket->offLine > 0)
            ret = true;
    }

    if(ret == false)
        clearShow();

    return ret;
}


/**
 * @brief 定时器响应函数
 */
void DS_DataDsplay::timeoutDone(void)
{
    bool ret = checkDev();
    if(ret)
    {
        dsyDevInfo();
        updateData();

        checkStatus();
    }
}

/**
 * @brief 更新数据接口函数
 * @param dev
 */
void DS_DataDsplay::updateDsp(DS_DsyDev &dev)
{
    mDataPacket = pdu_hd_getPacket(dev.ip, dev.num);
    mDevStatus->updateWidget(dev.ip, dev.num, dev.line);
    mDev = dev;

    timeoutDone();
}
