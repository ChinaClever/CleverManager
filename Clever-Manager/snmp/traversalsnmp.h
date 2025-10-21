#ifndef TRAVERSALSNMP_H
#define TRAVERSALSNMP_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include "pduhaship.h"
#include "pduhashdata.h"
#include "dbpdutypelist.h"
#include "dboidtable.h"
#include "snmp_pp/snmp_pp.h"
#include <stdint.h>

#if defined(Q_OS_WIN32)
using namespace Snmp_pp;
#endif

/**
 * @brief The TraversalSnmp class
 * 用于控制Snmp自动遍历并写数据到对应结构体。
 * 最多十个线程接收snmp消息。并发获取。
 */

class TraversalSnmp : public QThread
{
    Q_OBJECT
public:
    explicit TraversalSnmp(QObject *parent = 0);
    void start(const CTarget& cTarget,const Oid& oid,PduDataPacket * packet);
protected:
    void run();
protected:
    Oid                 m_oid;
    CTarget             m_ctarget;
    PduDataPacket *     m_packet;
};

class TraversalSnmpManager : public QThread
{
    Q_OBJECT
    explicit TraversalSnmpManager(QObject *parent = 0);
    ~TraversalSnmpManager();
public:
    static TraversalSnmpManager* get();
    void startTravel(int msec = 3000);
    void stopTravel();
    void run();
protected:
    void travelIp(const QString& ip,PduDevHash* devhash);
    void travelPdu(const CTarget& ctarget,const Oid& oid,PduDataPacket * packet);
protected slots:
    void recycleThread();
protected:
    QTimer          m_timer;
    QVector<TraversalSnmp*> m_travels;
    QMutex          m_mutex;
    QSemaphore      m_semaphore;

    CTarget m_ctarget;
};

#endif // TRAVERSALSNMP_H
