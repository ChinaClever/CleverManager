#include "dbrtpdusavethread.h"


DbRtPduSaveThread::DbRtPduSaveThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    timer = new QTimer(this);
//    timer->start(1*1000); ////=== 暂时关闭此功能，每抄保存一下数据库
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));

//    mRecord = new DbRtPduDevRecord(this);
    mRecord = DbRtPduDevRecord::get(this);
}

DbRtPduSaveThread::~DbRtPduSaveThread()
{
    wait();
    delete mRecord;
}

DbRtPduSaveThread *DbRtPduSaveThread::get(QObject *parent)
{
    static DbRtPduSaveThread* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRtPduSaveThread(parent);
    return sington;
}

/**
 * @brief 保存数据
 */
void DbRtPduSaveThread::slaveData()
{
    while(mDevList.size() > 0)
    {
        sDbRtPduDevList *devNode = mDevList.first();
        mRecord->rtSlave(devNode->devType, devNode->ip, devNode->devNum);
        mDevList.removeFirst();
        delete devNode;
        msleep(10);
    }
}



void DbRtPduSaveThread::getDevList()
{
    QList<int> list;
    pdu_hashData_list(list); // 获取设备类型接口

    for(int i=0; i<list.size(); ++i)
    {
        PduHashIP *hashIP =  pdu_hashData_getHash(list.at(i)); // 获取某种设备类型的对象
        QStringList ipList;
        hashIP->list(ipList); // 根据设备类型，获取设备IP列表

        for(int j=0; j<ipList.size(); ++j)
        {
            PduDevHash *devHash = hashIP->getDev(ipList.at(j));// 获取设备对象
            QList<int> devList;
            devHash->list(devList); // 根据设备IP，获取设备地址列表 级联

            for(int k=0; k<devList.size(); ++k)
            {
                PduDataPacket *data = devHash->getPacket(devList.at(k)); // 获取设备数据
                if(data->offLine > 0)  // 必须在线才进行检查
                {
                    sDbRtPduDevList *devNode = new sDbRtPduDevList;
                    devNode->devType = data->devType;
                    devNode->ip = data->ip->get();
                    devNode->devNum = data->devNum;
                    mDevList.append(devNode);
                }
                msleep(10); // 延时1us 让其它线程优先执行
            }
        }
    }
}

void DbRtPduSaveThread::run()
{
    if(isRun == false)
    {
        isRun = true;
        getDevList();
        slaveData();
        isRun = false;
    }
}


void DbRtPduSaveThread::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}
