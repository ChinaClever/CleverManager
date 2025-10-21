/*
 * dt_detaildev.cpp
 * 设备明细列表，显示设备统计数据
 *  1、使用线程更新数据、定时器更新界面
 *  2、显示设备电流、电压、功率等统计数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_detaildev.h"
#include "dbpducommandchannel.h"

DT_DetailDev::DT_DetailDev(QTableWidget *parent)
{
    mid = -1;
    isRun = false;
    isFrist = true;
    mTableWidget = parent;

    mPacketData = NULL;
    mPcaketTg = new PduPacketTG();
    mTg = new sPduTGDataStr();
    memset(mTg, 0, sizeof(sPduTGDataStr));

    //    timer = new QTimer(this);
    //    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    //    timer->start(3*1000);
    int time = rand()%5+1;
    QTimer::singleShot(time*1000,this,SLOT(timeoutDone())); // 以免过多定时器同一时间开启

    initIcon();
    connect(mTableWidget,SIGNAL(destroyed(QObject*)),SLOT(resetTableWidget()));
}

DT_DetailDev::~DT_DetailDev()
{
    wait();

    delete mPcaketTg;
}

void DT_DetailDev::initIcon(void)
{
    QString path;
    icon_get_devStatus(0, path);
    mIcon.addFile(path);

    icon_get_devStatus(1, path);
    mAlarmIcon.addFile(path);

    icon_get_devStatus(2, path);
    mOffLineIcon.addFile(path);
}

/**
 * @brief 增加Item
 * @param row
 * @param column
 * @param content
 */
void DT_DetailDev::addItemContent(int row, int column, QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);

    if(column != 1)
        item->setTextAlignment(Qt::AlignCenter);
    else {
        item->setTextAlignment(Qt::AlignVCenter);
        item->setIcon(mIcon);
    }
    mTableWidget->setItem(row, column, item);
    mItemList.append(item);
}

void DT_DetailDev::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);
    mTableWidget->setRowHeight(row,30);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);
}

void DT_DetailDev::refreshData(QStringList &list)
{
    if(mTableWidget==nullptr)return;
    // QList<QTableWidgetItem*> list1 = mTableWidget->findItems("",Qt::MatchContains);

    if(mItemList.size() == list.size())
    {
        for(int i=0; i<list.size(); ++i)
            mItemList.at(i)->setText(list.at(i));
    }
    else
    {
        //qDebug() << "lzy ------------- err";
    }
}

/**
 * @brief 线程更新统计数据
 */
void DT_DetailDev::run(void)
{
    if(isRun == false)
    {
        isRun = true;
        sPduTGDataStr *tg = mPcaketTg->getTgData(mPacketData); // 统计数据
        if(tg) {
            memcpy(mTg, tg, sizeof(sPduTGDataStr));
            delete tg; // 删除
        }
        else{
            memset(mTg, 0, sizeof(sPduTGDataStr));
        }

        isRun = false;
    }
}

/**
 * @brief 获取图标
 * @param id
 */
void DT_DetailDev::setIcon(int id)
{
    if(mTableWidget==nullptr)return;
    if(mid != id)
    {
        mid = id;
        // QList<QTableWidgetItem*> list = mTableWidget->findItems("",Qt::MatchContains);
        if(mItemList.size() > 2)
        {
            if(id == 0)
                mItemList.at(1)->setIcon(mIcon);
            else if(id == 1)
                mItemList.at(1)->setIcon(mAlarmIcon);
            else
                mItemList.at(1)->setIcon(mOffLineIcon);
        }
    }
}

/**
 * @brief 显示总电流电压
 * @param ip
 * @param num
 */
void DT_DetailDev::devTotalVC(QStringList &list, PduDataPacket *data)
{
    int id = 0;

    if(data->offLine > 0) {
        if(data->state > 0)
        {
            id = 1;
            list << tr("异常");
        } else
            list << tr("正常");
    } else {
        id = 2;
        list << tr("离线");
    }

    sPduTGDataStr *tg = mTg;
    int ret = tg->vol; //平均电压
    if(ret >= 0)
        list << tr("%1V").arg((int)(ret/COM_RATE_VOL));
    else
        list << ("---");

    ret = tg->cur; //总电流
    if(ret >= 0)
        list <<  tr("%1A").arg(ret/COM_RATE_CUR);
    else
        list << ("---");

    ret = tg->pow; //总功率
    if(ret >= 0)
        list << tr("%1kW").arg(ret/COM_RATE_POW);
    else
        list << ("---");

    ret = tg->ele; //总电能
    if(ret >= 0)
        list << tr("%1kWh").arg(ret/COM_RATE_ELE);
    else
        list << ("---");

    ret = tg->tem; //温度
    if(ret >= 0)
        list <<  tr("%1℃").arg(ret/COM_RATE_TEM);
    else
        list << ("---");

    ret = tg->hum; //湿度
    if(ret >= 0)
        list <<  tr("%1%").arg(ret/COM_RATE_HUM);
    else
        list << ("---");

#if defined(ZEBRA_MODULE) || defined(SNMP_MODULE)
    DbPduCommandChannelItem item = DbPduCommandChannel::get()->selectItemByIp(data->ip->get());
    if(item.get_channel&CHANNEL_SNMP)
        list << "SNMP";
    else if(item.get_channel&CHANNEL_ZEBRA)
        list << "ZEBRA";
    else if(item.get_channel&CHANNEL_CLEVER)
        list << "default";
#endif
    if(isFrist)
    {
        addRowContent(list);
        isFrist = false;
    }
    else
        refreshData(list);

    setIcon(id);
}

/**
 * @brief 更新设备信息
 * @param ip
 * @return
 */
void DT_DetailDev::updateDev(void)
{
    QStringList list;
    int num = mPacketData->devNum;

    list << QString::number(num);
    QString name = mPacketData->info->type->name->get();

    QString str;
    if(num==0){
        str = tr(" 主机：");str += name;
    }
    else if(num > 0)
        str = tr("副机 %1：").arg(num);


    list << str;
    devTotalVC(list,mPacketData);
}

void DT_DetailDev::setDev(PduDataPacket *data)
{
    mPacketData = data;
    timeoutDone();
}

/**
 * @brief 定时器刷新界面
 */
void DT_DetailDev::timeoutDone()
{
    if(isRun == false)
    {
        isRun = true;
        if(mPacketData)
            updateDev();

        isRun = false;
    }

    QTimer::singleShot(4*1000,this,SLOT(timeoutDone()));
    start(); // 启动线程更新数据
}

void DT_DetailDev::resetTableWidget()
{
    mTableWidget = nullptr;
    deleteLater();
}
