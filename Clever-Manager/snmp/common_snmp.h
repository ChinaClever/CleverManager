#ifndef COMMON_SNMP_H
#define COMMON_SNMP_H

#include <QString>
#include "snmp_pp/snmp_pp.h"
#include "pdudatapacket.h"
#include "dbpdutypelist.h"
#include "dboidtable.h"

#if defined(Q_OS_WIN32)
using namespace Snmp_pp;
#endif
namespace MibConvert {

//#define SNMP_DEBUG_OUTPUT(X) qDebug() << X;
#define SNMP_DEBUG_OUTPUT(X)

bool takeoff(QString& str,const QString& key);

int takeoffNum(QString &str);

int takeoffMSNum(QString& str);

QString deviceNumToString(int i);

QString numToString(int i);

OidItem oid2Item(QString oid);
OidItem name2OidItem(int pduId,const QString& name);

int padPacket(PduDataPacket * packet,const Vb& vb);
int padPacket(const QString& ip, const QString &oid, const QVariant var);
inline int padPacketByOid(PduDataPacket * packet,const QString& oid,const QVariant var);
int padPacketByMibName(PduDataPacket * packet,QString& mibName,const QVariant var);

float variant2float(const QVariant& var);
QString variant2String(const QVariant& var);

QVariant vbToVariant(const Vb& vb);

bool setVbValue(Vb& vb,const QVariant& var);

PduTypeItem address2OidType(const CTarget &ctarget);
PduTypeItem address2OidType(const QString& address);
}

#endif // COMMON_SNMP_H
