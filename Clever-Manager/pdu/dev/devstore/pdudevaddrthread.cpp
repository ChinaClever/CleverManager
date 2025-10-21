/*
 * pdudevaddrthread.cpp
 * 设备地址 机房、机柜
 *  1、自动初始化设备地址信息
 *  2、当机房、机柜信息改变时，自动更新设备地址
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudevaddrthread.h"
#include "pdudevlistread.h"


PduDevAddrThread::PduDevAddrThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    QTimer::singleShot(11*1000,this,SLOT(initFunSLot())); //延时初始化
    connect(DbRoomList::get(), SIGNAL(itemChanged(int,int)), this,SLOT(itemChangedSlot(int,int)));
    connect(DbCabinetList::get(),SIGNAL(itemChanged(int,int)),this,SLOT(itemChangedSlot(int,int)));
}

void PduDevAddrThread::initFunSLot()
{
    start();
}

/**
 * @brief 清除所有PDU的地址信息
 */
void PduDevAddrThread::clearAllPduAddr(void)
{
    QList<int> list;
    pdu_hashData_list(list); // 获取设备类型接口

    for(int i=0; i<list.size(); ++i)
    {
        PduHashIP *hashIP = pdu_hashData_getHash(list.at(i)); // 获取某种设备类型的对象
        QStringList ipList;
        hashIP->list(ipList); // 根据设备类型，获取设备IP列表
        for(int j=0; j<ipList.size(); ++j)
        {
            PduDevHash *devHash = hashIP->getDev(ipList.at(j));
            QList<int> devList;
            devHash->list(devList); // 获取副机设备列表 级联设备
            for(int k=0; k<devList.size(); ++k)
            {
                PduDataPacket *data = devHash->getPacket(devList.at(k)); // 获取设备数据
                if(data) {
                    data->info->addr->area->clear();
                    data->info->addr->cab->clear();
                }
                usleep(1);
            }
        }
    }
}

void PduDevAddrThread::setPduAddr(int type, const QString &ip, int num, const QString &room,const QString& cab)
{
    QString ipAddr = ip;
    PduDataPacket *packet = pdu_hd_get(type, ipAddr, num);
    if(packet) {
        packet->info->addr->area->set(room);
        packet->info->addr->cab->set(cab);
    }
}

void PduDevAddrThread::setPduAddr(const QString &room, const CabinetItem& cItem)
{
    if(!cItem.main_ip.isEmpty())   /* 主PDU　*/
        setPduAddr(cItem.main_type, cItem.main_ip, cItem.main_num, room, cItem.name);

    if(!cItem.spare_ip.isEmpty())  /*副PDU*/
        setPduAddr(cItem.spare_type, cItem.spare_ip, cItem.spare_num, room, cItem.name);
}

/**
 * @brief 设置PDU地址信息
 */
void PduDevAddrThread::setPduAddr(void)
{
    QVector<RoomItem> roomItems = DbRoomList::get()->allItems(); // 获取所有机房列表
    foreach(const RoomItem& roomIt,roomItems){
        QVector<CabinetItem> cItems = DbCabinetList::get()->selItemsByRoomId(roomIt.id);
        foreach(const CabinetItem& cItem,cItems) {
            setPduAddr(roomIt.name, cItem);
            usleep(1);
        }
    }
}

/**
 * @brief 启动新的线程
 */
void PduDevAddrThread::run(void)
{
    if(isRun == false){
        isRun = true;
        clearAllPduAddr();
        setPduAddr();
        isRun = false;
    }
}

void PduDevAddrThread::itemChangedSlot(int,int)
{
    start(); // 启动线程来处理
}
