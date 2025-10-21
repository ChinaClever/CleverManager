/*
 * tg_datathread.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_datathread.h"
#include "db_export/records/dbtgroom.h"
#include"services/ser_servicdlg.h"
#include "db_export/records/dbslaverecordsthread.h"

#define DC_SLAVE_TIME 30
bool gSlaveing = false;

TG_DataThread::TG_DataThread(QObject *parent) : QThread(parent)
{
    mID = -1;
    isRun = isSlave = false;
    mTimeCount = 0;

    mLock = new QReadWriteLock();
    mTGDataSt = new sTGDataSt();
    memset(mTGDataSt, 0, sizeof(sTGDataSt));

#if defined(CLOUD_MODULE)
    mRoomTg = new CloudRoomTg;
    mModule = new CloudModule;
    mCabinetTg = new CloudCabinetTg;
#endif
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    //timer->start(4500);
    timer->start(4000);

    int time = rand()%10+45;
    QTimer::singleShot(time*1000,this,SLOT(dbTimeoutDone()));


    connect(DbCabinetList::get(),SIGNAL(itemChanged(int,int)),this,SLOT(itemChangedSlot(int,int)));
}

TG_DataThread::~TG_DataThread()
{
    wait();
    clearList();
    if(mID > 0) {
        delete mTGDataSt->data;
        delete mTGDataSt->num;
    }
    delete mLock;

#if defined(CLOUD_MODULE)
    delete mModule;
    delete mCabinetTg;
    delete mTGDataSt;
#endif
}

void TG_DataThread::initData(sTGDataSt *tgDataSt)
{
    if(!tgDataSt->data) {
        tgDataSt->data = new TG_DataType();
        memset(tgDataSt->data, 0, sizeof(TG_DataType));

        tgDataSt->num = new pdu_devNumStr();
        memset(tgDataSt->num, 0, sizeof(pdu_devNumStr));
    }
}

/**
 * @brief 统计所有
 */
void TG_DataThread::allTG(void)
{
    m_pTgThread = new PduDataTgThread(this);
    m_pTgThread->tgData();

    mTGDataSt->data = m_pTgThread->getData();
    mTGDataSt->num = get_pdu_num();
}

/**
 * @brief 设置机房键值
 * @param id 机房ID键值
 * @return
 */
sTGDataSt *TG_DataThread::set(int id)
{
    if(id) {
        initData(mTGDataSt);
        readDataBase(id);
    } else
        allTG();
    mID = id;

    return mTGDataSt;
}


/**
 * @brief 清空列表
 */
void TG_DataThread::clearList(void)
{
    QWriteLocker locker(mLock);
    while(mList.size() > 0) {
        sTGListSt *list = mList.first();
        mList.removeFirst();
        delete list;
    }
    mList.clear();
}



/**
 * @brief 设备增加至链表
 * @param cItem
 */
void TG_DataThread::appendList(const CabinetItem& cItem, QList<sTGListSt *> &listSt)
{
    QWriteLocker locker(mLock);
    /* 主PDU　*/
    QString ip = cItem.main_ip;
    if(!ip.isEmpty()) {
        sTGListSt *list = new sTGListSt();
        list->ip = ip;
        list->num = cItem.main_num;
        listSt.append(list);
    }

    /*副PDU*/
    ip = cItem.spare_ip;
    if(!ip.isEmpty()) {
        sTGListSt *list = new sTGListSt();
        list->ip = ip;
        list->num = cItem.spare_num;
        listSt.append(list);
    }
}

/**
 * @brief 根据机房键值读取设备PDU列表
 * @param id
 */
void TG_DataThread::readDataBase(int id)
{
    try{
        clearList();
        QVector<CabinetItem> cItems = DbCabinetList::get()->selItemsByRoomId(id);
        foreach(const CabinetItem& cItem,cItems)
            appendList(cItem, mList);
    }catch(const SqlExecption& exception){
        qDebug() << exception.msg() << exception.type();
    }
}


/**
 * @brief 获取链表中所有设备的统计数据
 * @param devHash
 * @return
 */
void TG_DataThread::getTgData(QList<sTGListSt *> &listSt, sTGDataSt *tgDataSt)
{
    PduPacketTG packetTG;
    int allNum = 0, alarmNum=0, lineNum=0, offNum=0;

    QReadLocker locker(mLock);
    QList<sPduTGDataStr *> tgdataList;
    for(int k=0; k<listSt.size(); ++k)
    {
        PduDataPacket *packet = pdu_hd_getPacket(listSt.at(k)->ip,listSt.at(k)->num);
        if(packet)
        {
            sPduTGDataStr *tgData = packetTG.getTgData(packet);
            if(tgData)
                tgdataList.append(tgData);

            allNum++;
            if(packet->offLine > 0)
                lineNum++;
            else
                offNum++;

            if(packet->state)
                alarmNum++;
        }
        msleep(1);
    }

    sPduTGDataStr *ptr = pdu_tg_listData(tgdataList);
    if(ptr) {
        memcpy(tgDataSt->data, ptr, sizeof(sPduTGDataStr));
        delete ptr;
    }
    else
        memset(tgDataSt->data,0, sizeof(sPduTGDataStr));

    pdu_devNumStr *ptrNum = tgDataSt->num;
    ptrNum->num = allNum;
    ptrNum->line = lineNum;
    ptrNum->alarm = alarmNum;
    ptrNum->off = offNum;
}

void TG_DataThread::timeoutDone()
{
    if(isRun == false){
        start();
    }
}

void TG_DataThread::run(void)
{
    if(isRun == false){
        isRun = true;
        if(mID > 0)
            getTgData(mList, mTGDataSt);

        if(isSlave) {
            saveTgData(); // 机房统计数据插入到数据库中
        }
        isRun = false;
    }
}

/**
 * @brief 根据机柜ID来验证，是否是这一机房
 * @param id
 * @return true 表示这一机房有机柜修改信息
 */
bool TG_DataThread::getCabinetById(int id)
{
    bool ret = false;
    QVector<CabinetItem> items =  DbCabinetList::get()->findItemById(id); // 获取所有机房列表
    if(items.size() == 1) {
        CabinetItem it = items.first();
        if(it.room_id == mID)
            ret = true;
    }

    return ret;
}

void TG_DataThread::itemChangedSlot(int id,int)
{
    if(mID>0){
        bool ret = getCabinetById(id);
        if(ret) {
            readDataBase(mID);
        }
    }
}

TgRoomItem TG_DataThread::getTgItem(int id, sTGDataSt *tgDataSt)
{
    TgRoomItem item;

    if(id>0){ // 获取机房名
        item.roomName =  DbRoomList::get()->findItemById(mID).first().name;
    }

    item.vol = (int)(tgDataSt->data->vol/COM_RATE_VOL);
    item.cur = tgDataSt->data->cur/COM_RATE_CUR;
    item.pow = tgDataSt->data->pow/COM_RATE_POW;
    item.ele = tgDataSt->data->ele/COM_RATE_ELE;
    item.pf = tgDataSt->data->pf/COM_RATE_PF;
    item.tem = tgDataSt->data->tem/COM_RATE_TEM;
    item.hum = tgDataSt->data->hum/COM_RATE_HUM;

    item.allNum = tgDataSt->num->num;
    item.lineNum = tgDataSt->num->line;
    item.alarmNum = tgDataSt->num->alarm;
    item.offLineNum = tgDataSt->num->off;

    return item;
}


void TG_DataThread::cabinetTg(int flag)
{
    QVector<CabinetItem> cItems = DbCabinetList::get()->selItemsByRoomId(mID);
    foreach(const CabinetItem& cItem,cItems) {

        QList<sTGListSt *> listSt;
        appendList(cItem, listSt);

        sTGDataSt *tgDataSt = new sTGDataSt;
        memset(tgDataSt, 0, sizeof(sTGDataSt));
        initData(tgDataSt);
        getTgData(listSt, tgDataSt);
        TgRoomItem item = getTgItem(mID, tgDataSt);

        if(flag)
            mCabinetTg->save(cItem.name, item);
        else
            mCabinetTg->update(cItem.name, item);

        delete tgDataSt->data;
        delete tgDataSt->num;
        delete tgDataSt;
        msleep(30);
    }
}

void TG_DataThread::cabinetTg(const CabinetItem& cItem, TgCabItem &cabItem)
{
    PduPacketTG packetTG;
    QString ip = cItem.main_ip;
    int type = cItem.main_type;
    int num = cItem.main_num;

    if((!ip.isEmpty()) &&(type)) {
        PduDataPacket *packet = pdu_hd_get(type, ip, num);
        if(packet) {
            sPduTGDataStr *tgData = packetTG.getTgData(packet);
            if(tgData)
            {
                cabItem.vola = tgData->vol/COM_RATE_VOL;
                cabItem.cura = tgData->cur/COM_RATE_CUR;
                cabItem.elea = tgData->ele/COM_RATE_ELE;
                delete tgData;
            }
        }
    }

    ip = cItem.spare_ip;
    type = cItem.spare_type;
    num = cItem.spare_num;
    if((!ip.isEmpty()) &&(type)) {
        PduDataPacket *packet = pdu_hd_get(type, ip, num);
        if(packet) {
            sPduTGDataStr *tgData = packetTG.getTgData(packet);
            if(tgData)
            {
                cabItem.volb = tgData->vol/COM_RATE_VOL;
                cabItem.curb = tgData->cur/COM_RATE_CUR;
                cabItem.eleb = tgData->ele/COM_RATE_ELE;
                delete tgData;
            }
        }
    }
}

void TG_DataThread::cabinetTg(QString &room)
{
    QVector<CabinetItem> cItems = DbCabinetList::get()->selItemsByRoomId(mID);
    foreach(const CabinetItem& cItem,cItems) {

        QList<sTGListSt *> listSt;
        appendList(cItem, listSt);

        sTGDataSt *tgDataSt = new sTGDataSt;
        memset(tgDataSt, 0, sizeof(sTGDataSt));
        initData(tgDataSt);
        getTgData(listSt, tgDataSt);
        TgRoomItem item = getTgItem(mID, tgDataSt);

        TgCabItem cabItem;
        cabItem.roomName = room;
        cabItem.cabName = cItem.name;
        cabItem.vol = item.vol;
        cabItem.cur = item.cur;
        cabItem.pow = item.pow;
        if(cabItem.vol > 0) {
            cabinetTg(cItem, cabItem);
            DbSlaveRecordsThread::get()->appendCabItem(cabItem);
            msleep(30);
        }

        delete tgDataSt->data;
        delete tgDataSt->num;
        delete tgDataSt;

    }
}

/**
 * @brief 数据保存至数据库中
 */
void TG_DataThread::saveTgData(void)
{
    static QReadWriteLock lock;
    QWriteLocker locker(&lock);
    //sleep(1);

#if defined(CLOUD_MODULE)
    if(mID>0){
        mRoomTg->update(item);
        cabinetTg(0);
    } else {
        mModule->update(item); // 更新模块信息
    }
    if(mID>0){
        mRoomTg->save(item);
        cabinetTg(1);
    } else {
        mModule->save(item); // 保存模块信息
    }
#endif

    if(mID>0) {
        TgRoomItem item = getTgItem(mID, mTGDataSt);
        if(item.lineNum>0) {
            DbSlaveRecordsThread::get()->appendRoomItem(item);
            cabinetTg(item.roomName);
        }
    }

    msleep(5);
    isSlave = false;
}

void TG_DataThread::dbTimeoutDone()
{
    if(mTGDataSt->data)  {
        if(!isSlave) {
            int count = getTime()*60;
            if(mTimeCount++ > count)  //更新数据
            {
                mTimeCount = 0;
                isSlave = true;
            }
        }
    }

    QTimer::singleShot(1200,this,SLOT(dbTimeoutDone()));
}


