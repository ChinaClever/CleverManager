/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "datapackets.h"
#include <QtSql>

DataPackets::DataPackets(QObject *parent) : Dp_BasicThread(parent)
{

}

sDataPacket *DataPackets::get(const QString &ip, const QString &dev_num)
{
    QString key = ip +";"+ dev_num;
    return insert(key);
}

sDataPacket *DataPackets::get(const QString &ip, int devNum)
{
    QString dev_num = tr("主机");
    if(devNum) dev_num = tr("副机%1").arg(devNum);

    sDataPacket * p = get(ip, dev_num);
    if(p) {
        p->id = devNum;
        p->dev_num = dev_num;
    }

    return p;
}


int DataPackets::tgDataPackts(sTgObjData *tg, QVector<sDataPacket *> &packs)
{
    int size=0, ret=0, ts=0;
    memset(tg, 0, sizeof(sTgObjData));
    for(int i=0; i<packs.size(); ++i)
    {
        sDataPacket *m = packs.at(i);
        if(!m) continue;

        if((m->offLine > 0) && m->en) {
            tg->vol += m->data.tg.vol;
            tg->cur += m->data.tg.cur;
            tg->pow += m->data.tg.pow;
            tg->ele += m->data.tg.ele;

            tg->activePow = m->data.tg.activePow;
            if(m->data.tg.vol > 50) size++;
            tg->tem += m->data.tg.tem;
            if(m->data.tg.tem) ts++;

            ret += m->alarm;
        }
    }

    if(size)  tg->vol /= size;
    if(ts) tg->tem /= ts;

    if(tg->activePow > 0)
        tg->pf = (tg->pow * 100.0 / tg->activePow);
    else
        tg->pf = 0;
    if(tg->pf>99) tg->pf = 99;

    return ret;
}

/**
 * @brief 获取平均数据 0视无效数据
 * @param data
 */
int DataPackets::averData(ushort *data, int len)
{
    int size=0, value=0,temp;

    for(int i=0; i< len; ++i)
    {
        temp = data[i];
        if(temp > 0) {
            size++;
            value += temp;
        } else
            temp = 0;
    }

    if(size > 0)
        value /= size;

    return value;
}


void DataPackets::tgDevData(sDevData &dev)
{
    sTgObjData *tg = &(dev.tg);
    sObjData *obj = &(dev.line);
    int size = obj->size;

    memset(tg, 0, sizeof(sTgObjData));
    for(int i=0; i<size; ++i)
    {
        tg->cur += obj->cur.value[i];
        tg->pow += obj->pow[i];
        tg->ele += obj->ele[i];
        tg->activePow += obj->activePow[i];
    }
    tg->vol = averData(obj->vol.value, size);
    tg->tem = averData(dev.env.tem.value, dev.env.size);
    if(tg->activePow > 0)
        tg->pf = (tg->pow * 100.0 / tg->activePow);
    else
        tg->pf = 0;
    if(tg->pf>99) tg->pf = 99;
    //    tg->pf = averData(obj->pf);
}

sDataPacket *DataPackets::getByPdu(uint id)
{
    sDataPacket *pack = nullptr;
    QHashIterator<QString, sDataPacket *> iter(mHash);
    while(iter.hasNext()) {
        pack = iter.next().value();
        if(pack) {
            if(pack->pdu_id == id) {
                if(pack->en)
                    break;
            }
        }
    }

    return pack;
}

void DataPackets::delPdu(uint id)
{
    sDataPacket *pack = getByPdu(id);
    if(pack) {
        pack->en = 0;
        pack->offLine = -1;
    }
}

QVector<sDataPacket *> DataPackets::getByCab(uint id)
{
    sDataPacket *pack = nullptr;
    QVector<sDataPacket *> packs;
    QHashIterator<QString, sDataPacket *> iter(mHash);

    while(iter.hasNext()) {
        pack = iter.next().value();
        if(pack) {
            if(pack->cab_id == id) {
                if(pack->en)
                    packs.append(pack);
            }
        }
    }

    return packs;
}

QVector<sDataPacket *> DataPackets::getByRoom(uint id)
{
    sDataPacket *pack = nullptr;
    QVector<sDataPacket *> packs;
    QHashIterator<QString, sDataPacket *> iter(mHash);

    while(iter.hasNext()) {
        pack = iter.next().value();
        if(pack) {
            if(pack->room_id == id) {
                if(pack->en)
                    packs.append(pack);
            }
        }
    }

    return packs;
}

void DataPackets::dels(QVector<sDataPacket *> &packs)
{
    sDataPacket *pack = nullptr;
    for(int i=0; i<packs.size(); ++i) {
        pack = packs.at(i);
        pack->en = 0;
        pack->offLine = -1;
    }
}

void DataPackets::delCab(uint id)
{
    QVector<sDataPacket *> packs = getByCab(id);
    dels(packs);
}

void DataPackets::delRoom(uint id)
{
    QVector<sDataPacket *> packs = getByRoom(id);
    dels(packs);
}
