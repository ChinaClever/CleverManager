/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "pdu_envwid.h"
#include "pdu_sethumdlg.h"

Pdu_EnvWid::Pdu_EnvWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mPacket = nullptr;
    initTableWidget();
}


void Pdu_EnvWid::initWid()
{
    QString title = tr("传感器状态信息");
    QStringList header;
    header << tr("传感器") << tr("传感器状态") <<tr("最小值") << tr("最大值");
    initTableWid(header, 0, title);
}


/**
 * @brief 初始化表格窗口
 */
void Pdu_EnvWid::initTableWidget()
{
    for(int i=0; i<SENOR_NUM; ++i) {
        QStringList tmps;
        tmps << tr("温度 %1").arg(i+1);
        appendTableRow(tmps);
    }

    for(int i=0; i<SENOR_NUM; ++i) {
        QStringList hums;
        hums << tr("湿度 %1").arg(i+1);
        appendTableRow(hums);
    }

    for(int i=0; i<2; ++i)
    {
        QStringList list;
        list << tr("门禁 %1").arg(i+1) << "---";
        appendTableRow(list);
    }

    {
        QStringList list;
        list << tr("水浸");
        appendTableRow(list);
    }

    {
        QStringList list;
        list << tr("烟雾");
        appendTableRow(list);
    }
}


void Pdu_EnvWid::setAlarm(int row, int id, sDataUnit &unit)
{
    int value = unit.alarm[id];
    if(value) value = 2;
    setItemColor(row, 1, value);
}


int Pdu_EnvWid::setDataUnit(int row, sDataUnit &unit, const QString &str)
{
    for(int i=0; i<unit.size && i<SENOR_NUM; ++i) {
        QStringList list;
        if(unit.value[i]) {
            double value = unit.value[i] / COM_RATE_TEM;
            list << QString::number(value) + str;

            value = unit.min[i] / COM_RATE_TEM;
            list << QString::number(value) + str;

            value = unit.max[i]/ COM_RATE_TEM;
            list << QString::number(value) + str;
        } else {
            list << "---" << "---" << "---";
        }

        setTableRow(row+i, 1, list);
        setAlarm(row+i, i, unit);
    }

    return row + SENOR_NUM;
}


/**
 * @brief 门禁
 */
int Pdu_EnvWid::setDoor(int row, uchar *data)
{
    for(int i=0; i<2; ++i)
    {
        QString str = "---";
        switch (data[i]) {
        case 1: str = tr("美"); break;
        case 2: str = tr("开"); break;
        }
        setTableItem(row, 1, str);

        int value = 0;
        if(data[i] == 2) value = 2;
        setItemColor(row, 1, value);
        row++;
    }

    return row;
}

int Pdu_EnvWid::setDataOther(int row, uchar *data)
{
    QString str = "---";
    switch (data[0]) {
    case 1: str = tr("正常"); break;
    case 2: str = tr("报警"); break;
    }
    setTableItem(row, 1, str);

    int value = 0;
    if(data[0] == 2) value = 2;
    setItemColor(row, 1, value);

    return row++;
}

int Pdu_EnvWid::updateDev(sEnvData &env)
{
    int row = 0;
    row = setDataUnit(row, env.tem, " ℃");
    row = setDataUnit(row, env.hum, " %");
    row = setDoor(row, env.door);
    row = setDataOther(row, env.smoke);
    row = setDataOther(row, env.water);

    return row;
}

void Pdu_EnvWid::updateData()
{
    if((mPacket->en) && (mPacket->offLine > 0)) {
        updateDev(mPacket->data.env);
    }
}


void Pdu_EnvWid::timeoutDone()
{
    if(mPacket) {
        updateData();
    }
}


void Pdu_EnvWid::itemDoubleSlot()
{
    int row = currentRow();
    // int coulumn = currentColumn();

    if(mPacket){
        if(row < 4) {
            Pdu_SetTemDlg dlg(this);
            dlg.init(row, mPacket);
            dlg.exec();
        } else if(row < 8) {
            Pdu_SetHumDlg dlg(this);
            dlg.init(row-4, mPacket);
            dlg.exec();
        }
        updateData();
    }
}
