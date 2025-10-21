#ifndef SNMPTRAP_H
#define SNMPTRAP_H

#include <QThread>
#include "snmp_pp/snmp_pp.h"

#if defined(Q_OS_WIN32)
using namespace Snmp_pp;
#endif

class SnmpTrap : public QThread
{
    Q_OBJECT
public:
    explicit SnmpTrap(QObject *parent = 0);
    ~SnmpTrap();
    void stop();
signals:
    void backInfo(const QString& str);
    void trapInfo(int errType,const QString& ipAddr,const QString& oid,const QString& info);
protected:
    void run();
    static void callback( int reason, Snmp *snmp, Pdu &pdu, SnmpTarget &target, void *cd);
protected:
    bool m_run;
};

#endif // SNMPTRAP_H
