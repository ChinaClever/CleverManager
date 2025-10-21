/*
 * dt_detailtable.cpp
 *  设备明细表格显示
 *     1、 创建一个表格
 *      2、
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dt_detailtable.h"
#include "common.h"
#include "pdudlobject.h"
#include <QHeaderView>
#include<qlabel.h>
#include <QScrollBar>

extern PduDevHash *pdu_hd_get(QString &ip);
extern PduDataPacket *pdu_hd_getPacket(QString &ip, int num);


DT_DetailTable::DT_DetailTable(QWidget *parent) : QWidget(parent)
{
    mTableWidget = new QTableWidget(this);
    QGridLayout *layout = new QGridLayout(this); //网格布局



    layout->addWidget(mTableWidget);

    //    mTableWidget->setSortingEnabled(true);
    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(mTableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(itemDoubleSlot(QTableWidgetItem *))); //双击信号
    mRoomID = 0;
    initWidget();
}

DT_DetailTable::~DT_DetailTable()
{

}

void DT_DetailTable::closeSlot(void)
{
    clearDev();
    delete this;
}

void DT_DetailTable::remove_detailDev()
{
    DT_DetailDev *dev = dynamic_cast<DT_DetailDev*>(sender()); //获取信号源
    if(dev)
        mDevList.removeOne(dev);
}

/**
 * @brief 初始化窗口
 */
void DT_DetailTable::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/

    QStringList header;
    header<<tr("设备号") << tr("设备名")<<tr("状态")<<tr("平均电压")<<tr("总电流")
         <<tr("总功率")<< tr("总电能")<<tr("温度")<<tr("湿度")
       #if defined(ZEBRA_MODULE) || defined(SNMP_MODULE)
        <<tr("通道")
      #endif
          ;
    QFont ft;
    ft.setPixelSize(12);
   mTableWidget->horizontalHeader()->setStyleSheet("background-color:rgb(241,241,241)");
//    mTableWidget->horizontalHeader()->setFixedHeight(30);
//    mTableWidget->horizontalHeader()->setFont(QFont("微软雅黑",10));
   mTableWidget->verticalHeader()->setStyleSheet("background-color:rgb(241,241,241)");
//    mTableWidget->verticalHeader()->setFixedWidth(30);
//    mTableWidget->verticalHeader()->setFont(QFont("微软雅黑",10));
//    mTableWidget->setFont(QFont("微软雅黑",10));


    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->setHorizontalHeaderLabels(header);

    mTableWidget->setColumnWidth(0,0); //第一列隐藏

    mTableWidget->verticalScrollBar()->setEnabled(false);  //隐藏滚动条
    mTableWidget->horizontalHeader()->setHighlightSections(false);
     mTableWidget->verticalHeader()->setHighlightSections(false);
   // mTableWidget->horizontalHeader()->setVisible(false);
    mTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

/**
 * @brief 释放内存
 */
void DT_DetailTable::clearDev(void)
{
    for(int i=0; i<mDevList.size(); ++i)
    {
        DT_DetailDev *dev = mDevList.at(i);
        delete dev;
    }
    mDevList.clear();
}


/**
 * @brief 更新设备信息
 * @param ip
 * @return
 */
void DT_DetailTable::updateDev(QString &ip)
{
    PduDLObject dlObj;

    QList<int> numList;
    dlObj.line(ip, numList); // 获取在线整机列表

    for(int i=0; i<numList.size(); ++i)
    {
        PduDataPacket *data = pdu_hd_getPacket(ip, numList.at(i));
        if(data) {
            DT_DetailDev *dev = new DT_DetailDev(mTableWidget);
            dev->setDev(data);
            connect(dev,SIGNAL(destroyed(QObject*)),SLOT(remove_detailDev()));
            mDevList.append(dev);
        }
    }
}


/**
 * @brief 设置窗口高度
 */
void DT_DetailTable::setHeight(void)
{
    int height = mTableWidget->rowCount()*30 + 45;

    this->setMinimumHeight(height);
    this->setMaximumHeight(height);
}


/**
 * @brief 根据机房来显示设备
 * @param roomID 机房键值
 * @param ipList ip列表
 * @param numList 设备号列表
 */
void DT_DetailTable::updateWidget(int roomID, QStringList &ipList, QList<int> &numList)
{
    for(int i=0; i<ipList.size(); ++i)
    {
        PduDataPacket *data = pdu_hd_getPacket(ipList[i], numList.at(i));
        if(data) {
            DT_DetailDev *dev = new DT_DetailDev(mTableWidget);
            dev->setDev(data);
            connect(dev,SIGNAL(destroyed(QObject*)),SLOT(remove_detailDev()));
            mDevList.append(dev);
        }
    }
    setHeight();

    mRoomID = roomID;
    mIpList = ipList;
    mNumList = numList;
}


/**
 * @brief 更新窗口
 * @param ip
 */
void DT_DetailTable::updateWidget(QString &ip)
{
    m_ip = ip;
    updateDev(ip);
    setHeight();
}


/**
 * @brief 选择设备
 * @param dev
 * @return
 */
bool DT_DetailTable::selectDev(short &num)
{
    bool ret = true;

    int row = mTableWidget->rowCount();
    if(row >0)
    {
        row = mTableWidget->currentItem()->row();//当前选中行
        if(row >= 0)
            num = mTableWidget->item(row, 0)->text().toInt(); //获取某一格内容
        else
            ret = false;
    }
    else
        ret = false;

    return ret;
}

/**
 * @brief 双击选择设备响应函数
 */
void DT_DetailTable::itemDoubleSlot(QTableWidgetItem *)
{
    if(mRoomID==0) // 显示所有设备时，
    {
        short num;
        bool ret = selectDev(num);
        if(ret)
            emit selectSig(m_ip, num);
    }
    else // 机房列表显示时
    {
        int row = mTableWidget->rowCount();
        if(row >0){
            row = mTableWidget->currentItem()->row();//当前选中行
            if(row >= 0)
                emit selectSig(mIpList.at(row), mNumList.at(row));
        }
    }
}
