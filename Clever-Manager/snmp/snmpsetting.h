#ifndef SNMPSETTING_H
#define SNMPSETTING_H

#include "QString"
#include <QVariant>
#include "snmp_pp/snmp_pp.h"
#if defined(Q_OS_WIN32)
using namespace Snmp_pp;
#endif

class SnmpSetting
{
    SnmpSetting();
public:
    static SnmpSetting *get();
    bool setSwitch(const QString& ip, int devNum, int portNum, int opened, int cmd_index = -1);
    int set_device_name(const QString channel_name, int slave_num, QString &name, int cmd_index = -1);
    int set_output_name(const QString channel_name, int slave_num, int output_num, QString &name, int cmd_index = -1);
    int set_output_threshold(const QString channel_name, int slave_num, int output_num, int type, int value, int cmd_index = -1);
    int set_line_config(const QString channel_name, int slave_num, int line_num, int fun_type, int type, int value, int cmd_index = -1);
    int set_temp_config(const QString channel_name, int slave_num, int senser_type, int senser_num, int value_type, int value, int cmd_index = -1);

protected:
    inline bool setOidValue(const QString& ip,QString& oid,const QVariant& var);
protected:
    CTarget m_ctarget;
};

#endif // SNMPSETTING_H
