#include "traversalsnmp.h"
#include "signaldispatch.h"
#include "common_snmp.h"
#include "dbpducommandchannel.h"
#include "pdudevlistsave.h"

#include <QDebug>

TraversalSnmp::TraversalSnmp(QObject *parent):QThread(parent)
{
}

void TraversalSnmp::start(const CTarget& cTarget,const Oid& oid,PduDataPacket* packet)
{
    m_oid     = oid;
    m_ctarget = cTarget;
    m_packet  = packet;
    if(packet == nullptr)
    {
        return;
    }

    QThread::start();
}

/**
 * @brief TraversalSnmp::run
 * 执行线程查询snmp
 */
void TraversalSnmp::run()
{
    int status;
    Snmp snmp(status, 0, (m_ctarget.get_address().cast_udpaddress().get_ip_version() == Address::version_ipv6));
    if (status == SNMP_CLASS_SUCCESS)
    {
        Oid oid(m_oid);
        Vb vb(oid);
        vb.set_oid(oid);
        for(;;)
        {
            Pdu pdu(&vb,1);
            status = snmp.get_next(pdu, m_ctarget);
            if(status == SNMP_CLASS_SUCCESS)
            {
                pdu.get_vb(vb,0);
                if (vb.get_syntax() == sNMP_SYNTAX_ENDOFMIBVIEW ||
                        vb.get_syntax() == sNMP_SYNTAX_NOSUCHINSTANCE ||
                        vb.get_syntax() == sNMP_SYNTAX_NOSUCHOBJECT)
                {
                    break;
                }
                if(vb.get_oid().nCompare(oid.len(),oid)!=0)
                {
                    break;
                }
                //获取返回值
                m_packet->offLine = PDU_OFF_LINE_TIME*3;
                //qDebug() << "[MYDEBUG : INFO] - TraversalSnmp::run() oidString-3 = " << vb.get_printable_oid();
                pdu.get_vb(vb,0);
                MibConvert::padPacket(m_packet,vb);
            }
            else
            {
                qDebug() << "[MYDEBUG : ERROR] - TraversalSnmp::run()"
                         <<" oidString = " << vb.get_printable_oid()
                         << " error = " << snmp.error_msg(status);
                break;
            }
        }

        SignalDispatch::get()->dispatch(m_packet);
    }
}

//*****************************************************************************************************
TraversalSnmpManager::TraversalSnmpManager(QObject *parent) :
    QThread(parent),m_travels(50,nullptr),m_semaphore(50)
{
    m_ctarget.set_readcommunity("public");
    m_ctarget.set_writecommunity("private");
    m_ctarget.set_version(version2c);
    m_ctarget.set_retry(0);
    m_ctarget.set_timeout(50);

    m_timer.setSingleShot(false);
    m_timer.setTimerType(Qt::VeryCoarseTimer);
    QVector<TraversalSnmp*>::iterator iter = m_travels.begin();
    for(;iter!=m_travels.end();++iter)
    {
        *iter = new TraversalSnmp(this);
        connect(&m_timer,SIGNAL(timeout()),SLOT(start()));
        connect(*iter,SIGNAL(finished()),SLOT(recycleThread()));
    }
}

TraversalSnmpManager::~TraversalSnmpManager()
{
    m_travels.clear();
}

TraversalSnmpManager *TraversalSnmpManager::get()
{
    static TraversalSnmpManager* sington = new TraversalSnmpManager();
    return sington;
}

void TraversalSnmpManager::startTravel(int msec)
{
    m_timer.start(msec);
}

void TraversalSnmpManager::stopTravel()
{
    m_timer.stop();
}

/**
 * @brief TraversalSnmpManager::run
 */
void TraversalSnmpManager::run()
{
    QList<int> list;
    ::pdu_hashData_list(list);
    foreach(int type,list)
    {
        PduHashIP* phi = ::pdu_hashData_getHash(type);
        QStringList ipList;
        if(phi->list(ipList)<=0)continue;
        foreach(const QString& ip,ipList)
        {
            PduDevHash* devhash = phi->getDev(ip);
            DbPduCommandChannelItem item = DbPduCommandChannel::get()->selectItemByIp(ip);
            if((item.id > 0) && ((item.get_channel&CHANNEL_SNMP)==0))
            {
                PduDataPacket *packet = devhash->getPacket(0);
                if(packet)
                {
                    if(packet->offLine > 0)
                    {
                        //qDebug() << "[MYDEBUG : INFO] ---- PDU of ip:" << ip << " snmp get channel is close";
                    }
                }

                continue;
            }
            else
            {
                travelIp(ip,devhash);
            }
        }
    }
}

/**
 * @brief TraversalSnmpManager::travelIp
 * @param ip 对于ip地址
 * @param devhash
 */
void TraversalSnmpManager::travelIp(const QString &ip, PduDevHash *devhash)
{
    if(devhash == nullptr || devhash->size()<=0)
    {
        qDebug() << "[MYDEBUG : ERROR] - TraversalSnmpManager::travelIp() ip = "
                 << ip
                 << " devhash is empty or nullptr!" ;
        return;
    }
    //这里测试对应端口是否匹配
    UdpAddress address(ip.toUtf8().constData());
    address.set_port(161);
    m_ctarget.set_address(address);
    PduTypeItem item = MibConvert::address2OidType(m_ctarget);
    if(item.id>=0)
    {
        QList<int> numList;
        devhash->list(numList);
        foreach(int num,numList)
        {
            //找到对应的副机对应的oid起始处。
            QString name = MibConvert::deviceNumToString(num) + "Name";//获取到设备名称，即第一个开始遍历的oid
            QVector<OidItem> items = DbOidTable::get()->selectItemByName(item.id,name);//查找对应的主副机编号。
            if(items.size() == 1)
            {
                QString oidString = item.oid + items.first().oid;
                Oid oid(oidString.toUtf8().constData());
                oid.trim();//截短一位，从上级Oid开始遍历

                PduDataPacket *dev = NULL;

                dev = devhash->getPacket(num);
                if(dev)
                {
                    travelPdu(m_ctarget, oid, dev);
                }
                else
                {
                    /*qDebug() << "[MYDEBUG : INFO] - TraversalSnmpManager::travelIp() creat ip = "
                             << ip
                             << " slave_num = "
                             << num
                             << " PDU device.";*/

                    int dev_type = 0x01050101;

                    dev = devhash->get(num);
                    if(NULL == dev)
                    {
//                        qDebug() << "[MYDEBUG : INFO] - TraversalSnmpManager::travelIp() get ip = "
//                                 << ip
//                                 << " dev is empty!";

                        continue ;
                    }
                    else
                    {

                        dev->devType = dev_type;
                        dev->devNum = num;
                        dev->ip->set(ip);
                        dev->offLine = PDU_OFF_LINE_TIME;

                        if(devhash->isNew)
                        {
                            dev->info->addr->group->set("ZPDU");
                            dl_save_add(dev->ip->get());
                        }

                        PduDataPacket *packet_temp = pdu_hd_getPacket((QString &)ip, num);
                        if(NULL == packet_temp)
                        {
                            qDebug() << "[MYDEBUG : ERROR] - TraversalSnmpManager::travelIp() creat ip = "
                                     << ip
                                     << " device error!";

                            continue ;
                        }
                        else
                        {
                            travelPdu(m_ctarget, oid, dev);
                        }
                    }
                }
            }
        }
    }
}

void TraversalSnmpManager::travelPdu(const CTarget& ctarget,const Oid& oid, PduDataPacket *packet)
{
    if(packet==nullptr)
    {
        return;
    }

    //若所有的遍历线程都被占用，则等待。
    TraversalSnmp* thread = nullptr;

    QMutexLocker locker(&m_mutex);
    m_semaphore.acquire();
    thread = m_travels.takeLast();


    if(thread != nullptr)
    {
        thread->start(ctarget,oid,packet);
    }
}

/**
 * @brief TraversalSnmpManager::recycleThread
 * 用于回收利用完的线程。
 * 并获得完整返回值。
 */
void TraversalSnmpManager::recycleThread()
{
    TraversalSnmp* ts = qobject_cast<TraversalSnmp*>(sender());
    if(ts)
    {
        QMutexLocker locker(&m_mutex);
        m_travels.append(ts);
        m_semaphore.release();
    }
}
