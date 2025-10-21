/*
 * ds_devstatus.cpp
 * 设备状态显示
 *  1、设备统计时，不显示状态信息
 *  2、当设备报警，或者断路器断开时才显示状态信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_devstatus.h"
#include "ui_ds_devstatus.h"
#include "common.h"

DS_DevStatus::DS_DevStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DS_DevStatus)
{
    ui->setupUi(this);
    mLine = 0;
    mDataPacket = NULL;
    setHiddenWid(mLine);

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(1000);
}

DS_DevStatus::~DS_DevStatus()
{
    delete ui;
}


/**
 * @brief 更改断路器名称
 */
void DS_DevStatus::updateSwTitle(int line)
{
    if(line) {
        char ch;
        switch (line) {
        case 1: ch = 'A';break;
        case 2: ch = 'B';break;
        case 3: ch = 'C';break;
        }
        QString str = QObject::tr("断路器（%1）:").arg(ch);
        ui->swTLab->setText(str);
    }
}


/**
 * @brief 隐藏窗口
 */
void DS_DevStatus::setHiddenWid(short line)
{
    if(line) {
         this->setHidden(false);
    } else {
        this->setHidden(true);
    }
}

void DS_DevStatus::updateWidget(QString &ip, short num, short line)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    mLine = line;

    setHiddenWid(line);
    updateSwTitle(line);
    timeoutDone();
}

/**
 * @brief 更新断路器状态
 */
void DS_DevStatus::updateSw()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::black);

    QString str = tr("接通");
    int sw = mDataPacket->data->line->sw->get(mLine-1);
    if(sw >= 0) {
        ui->swLab->setHidden(false);
        ui->swTLab->setHidden(false);
        if(sw == 0) {
            str = tr("断开");
            isAlarm = true;
            pa.setColor(QPalette::WindowText,Qt::red);
        }
    } else {
        ui->swLab->setHidden(true);
        ui->swTLab->setHidden(true);
    }

    ui->swLab->setText(str);
    ui->swLab->setPalette(pa);
}
/**
 * @brief 获取状态信息
 * @return
 */
QString DS_DevStatus::getStatus(PduDataUnit *dataUni)
{
    QString str;
    int line = mLine;
    if(line > 0)
    {
        line--;
        int ret = pdu_data_alarmCheck(dataUni, line);
        if(ret > 0)
        {
            switch (ret) {
            case PDU_DATA_ALARM_MIN:
                str = tr("过小");
                break;

            case PDU_DATA_ALARM_MAX:
                str = tr("过大");
                break;

            case PDU_DATA_ALARM_CRMIN:
                str = tr("小于临界下限");
                break;
            case PDU_DATA_ALARM_CRMAX:
                str = tr("大于临界上限");
                break;

            default:
                break;
            }
        }
    }
    return str;
}


/**
 * @brief 更新设备状态
 */
void DS_DevStatus::updateStatus()
{
    QString str;

    PduObjData *lineData = mDataPacket->data->line;
    QString volRetStr = getStatus(lineData->vol);
    if(!volRetStr.isEmpty()) {
        str = tr("电压") + volRetStr;
    }

    QString curRetStr = getStatus(lineData->cur);
    if(!curRetStr.isEmpty()) {
        if(!str.isEmpty()) // 加入分隔符
            str += "；";
        str = tr("电流") + curRetStr;
    }

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::black);
    if(!str.isEmpty()) {
        isAlarm = true;
         pa.setColor(QPalette::WindowText,Qt::red);
    }
     else
        str = tr("正常");

     ui->statusLab->setText(str);
     ui->statusLab->setPalette(pa);
}


void DS_DevStatus::timeoutDone()
{
    if(mDataPacket)
    {
        if(mLine>0) {
            isAlarm = false;
            updateStatus();
            updateSw();

            this->setHidden(!isAlarm); // 有报警才显示
        }
    }
}

