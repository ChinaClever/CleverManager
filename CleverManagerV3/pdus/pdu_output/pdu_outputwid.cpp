/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "pdu_outputwid.h"
#include "pdu_setoutputdlg.h"

Pdu_OutputWid::Pdu_OutputWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mPacket = nullptr;
}


void Pdu_OutputWid::initWid()
{
    QString title = tr("输出位状态信息");
    QStringList header;
    header << tr("名称")<<tr("开关") << tr("电流") << tr("功率")
           << tr("功率因数") << tr("电能");

    initTableWid(header, 1, title);
}

void Pdu_OutputWid::setAlarm(int id, sObjData &unit)
{
    int column=1, swAlarm=0;

    int value = unit.sw[id];
    if(value == 1) swAlarm = 2;
    setItemColor(id, column++, swAlarm);

    value = unit.cur.alarm[id];
    if(value) value = 2;
    else value = unit.cur.crAlarm[id];
    setItemColor(id, column++, value);
}

void Pdu_OutputWid::setObjUnit(int id, sObjData &unit)
{
    QStringList list;
    list << unit.name[id];

    QString  str = tr("接通");;
    if(1 == unit.sw[id]) {
        str = tr("断开");
    }
    list << str;

    str = "---";
    double value = unit.cur.value[id] / COM_RATE_CUR;
    list << QString::number(value) + "A";

    if(unit.pow[id]) {
        value = unit.pow[id] / COM_RATE_POW;
        list << QString::number(value) + "kW";
    } else {
        list << "---";
    }

    if(unit.pf[id]) {
        value = unit.pf[id] / COM_RATE_PF;
        list << QString::number(value);
    } else {
        list << "---";
    }

    if(unit.ele[id]) {
        value = unit.ele[id] / COM_RATE_ELE;
        list << QString::number(value) + "kWh";
    } else {
        list << "---";
    }

    setTableRow(id, list);
    setAlarm(id, unit);
}

int Pdu_OutputWid::updateDev(sDevData &dev)
{
    sObjData *obj = &(dev.output);
    for(int i=0; i<obj->size; ++i)
    {
        setObjUnit(i, dev.output);
    }

    return obj->size;
}

/**
 * @brief 数据更新入口函数
 */
void Pdu_OutputWid::updateData()
{
    int row = 0;
    if((mPacket->en) && (mPacket->offLine > 0)) {
        row = updateDev(mPacket->data);
    }

    checkTableRow(row);
}


void Pdu_OutputWid::timeoutDone()
{
    if(mPacket) {
        updateData();
    }
}


/**
* @brief 输出位开关控制
*/
void Pdu_OutputWid::switchControl(int row)
{
    Pdu_SetSwitchDlg dlg(this);
    dlg.setMode(row, mPacket);
    int ret = dlg.exec();
    if(ret == QDialog::Accepted)
        updateData();
}

/**
* @brief 设置输出位阈值
*/
void Pdu_OutputWid::setOutput(int row)
{
    Pdu_SetOutputDlg dlg(this);
    dlg.setMode(row, mPacket);
    int ret = dlg.exec();
    if(ret == QDialog::Accepted)
        updateData();
}



void Pdu_OutputWid::itemDoubleSlot()
{
    int row = currentRow();
    int coulumn = currentColumn();

    if(mPacket)
    {
        if((coulumn == 1) && (mPacket->devSpec != 2)) //B系列无开关控制
            switchControl(row);
        else if((coulumn == 2) && (mPacket->devSpec != 3))
            setOutput(row);
    }
}
