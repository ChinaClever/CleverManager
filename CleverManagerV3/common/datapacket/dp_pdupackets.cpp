/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dp_pdupackets.h"
#include "udp/sent/udpheartbeat.h"

Dp_PduPackets::Dp_PduPackets(QObject *parent) : DataPackets(parent)
{
    mDb = DbPduDevices::get();
    mPush = Json_Push::bulid(this);
    mHrs = Dp_PduHrsSave::bulid(this);
    mAlarm = Dp_PduAlarm::bulid(this);
    connect(mDb,SIGNAL(itemChanged(int,int)),SLOT(pduItemChange(int,int)));
}


Dp_PduPackets *Dp_PduPackets::bulid(QObject *parent)
{
    static Dp_PduPackets* sington = NULL;
    if(sington == NULL) {
        sington = new Dp_PduPackets(parent);
    }
    return sington;
}


sDataPacket *Dp_PduPackets::newDataPacket()
{
    sDataPacket *pack = new sDataPacket;
    if(pack) {
        sObjData *obj = &(pack->data.line);
        for(int i=0; i<LINE_NUM; ++i) {
            obj->name[i] = QString("Line %1").arg(i+1);
        }
        memset(&(obj->cur), 0, sizeof(sDataUnit));
        memset(&(obj->vol), 0, sizeof(sDataUnit));

        obj = &(pack->data.loop);
        for(int i=0; i<LOOP_NUM; ++i) {
            obj->name[i] = QString("Loop %1").arg(i+1);
        }
        memset(&(obj->cur), 0, sizeof(sDataUnit));
        memset(&(obj->vol), 0, sizeof(sDataUnit));

        obj = &(pack->data.output);
        for(int i=0; i<OUTPUT_NUM; ++i) {
            obj->name[i] = QString("Output %1").arg(i+1);
        }
        memset(&(obj->cur), 0, sizeof(sDataUnit));
        memset(&(obj->vol), 0, sizeof(sDataUnit));

    } else {
        qDebug() << "Dp_PduPackets new err";
    }


    return pack;
}

int Dp_PduPackets::getDevNum(const QString &dev_num)
{
    bool ok;
    QString str = dev_num.right(1);
    int ret = str.toInt(&ok);
    if(!ok) ret = 0;

    return ret;
}

void Dp_PduPackets::initPacket(PduDeviceItem &it)
{
    sDataPacket *pack = get(it.ip, it.dev_num);
    if(pack) {
        pack->en = 1;
        pack->count = 0;
        pack->pdu_id = it.id;
        pack->room_id = it.room_id;
        pack->cab_id = it.cab_id;

        pack->room = it.room;
        pack->cab = it.cab;
        pack->road = it.road;
        pack->net.ip = it.ip;
        pack->dev_num = it.dev_num;
        pack->dev_type = it.dev_type;
        pack->id = getDevNum(it.dev_num);
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents,1);
}

void Dp_PduPackets::initFun()
{
    QVector<int> ids = mDb->selectIds();
    for(int i=0; i<ids.size(); ++i) {
        pduItemChange(ids.at(i), 1);
    }
}

void Dp_PduPackets::pduItemChange(int id,int type)
{
    if(type) {
        PduDeviceItem it = mDb->findById(id);
        initPacket(it);
    } else {
        delPdu(id);
    }
}

void Dp_PduPackets::sendHeartBeat(sDataPacket *pack)
{
    if(pack->count%5) return;

    if((0 == pack->id) || (pack->dev_type == "SI-PDU"))
    {
        QString ip = pack->net.ip;
        UdpHeartBeat::bulid(this)->sent(ip);
        msleep(1+rand()%3);
    }
}

void Dp_PduPackets::pushData(sDataPacket *pack)
{
    bool ret = mPush->push(pack);
    if(ret) {
        msleep(2+rand()%3);
    }
}

void Dp_PduPackets::workDown(sDataPacket *pack)
{
    sendHeartBeat(pack);
    if(pack->offLine > 0) {
        pack->offLine--;

        if(pack->count>10)
            mAlarm->alarm(pack);
        mHrs->save(pack);
        tgDevData(pack->data);
        pushData(pack);
    }
}
