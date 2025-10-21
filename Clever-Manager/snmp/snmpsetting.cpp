#include "snmpsetting.h"
#include "common_snmp.h"
#include <QDebug>
#include <iostream>
#include <sstream>
#include <stdint.h>

using namespace std;

SnmpSetting::SnmpSetting()
{
    m_ctarget.set_readcommunity("public");
    m_ctarget.set_writecommunity("private");
    m_ctarget.set_version( version2c);
    m_ctarget.set_retry( 1);
    m_ctarget.set_timeout(200);
}

SnmpSetting *SnmpSetting::get()
{
    static SnmpSetting* sington = new SnmpSetting();
    return sington;
}
/**
 * @brief SnmpSetting::setSwitch
 * 设置开关状态
 * @param ip
 * @param devNum
 * @param portNum
 * @param opened
 * @return
 */
bool SnmpSetting::setSwitch(const QString &ip,
                            int devNum,
                            int portNum,
                            int opened,
                            int cmd_index)
{
//    qDebug() << "[MYDEBUG & INFO] - SnmpSetting::setSwitch() :"
//             << " devNum = " << devNum
//             << " portNum = " << portNum
//             << " state = " << opened
//             << " cmd_index = " << cmd_index;

    PduTypeItem tItem = MibConvert::address2OidType(ip);

    QString mibName = MibConvert::deviceNumToString(devNum)
                      + "OutputStatus"
                      + MibConvert::numToString(portNum);

    OidItem oItem = MibConvert::name2OidItem(tItem.id,mibName);

    QString oidString = tItem.oid+oItem.oid;

    if(cmd_index == -1)
    {
        return setOidValue(ip, oidString, opened);
    }
    else
    {
        if(opened > 0x00ffffff)
        {
            qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::setSwitch() value > 0x00ffffff!" ;
        }
        else
        {
            int data = opened | (cmd_index << 24);

            return setOidValue(ip, oidString, data);
        }
    }
}

int SnmpSetting::set_device_name(const QString channel_name,
                                 int slave_num,
                                 QString &name,
                                 int cmd_index)
{
//    qDebug() << "[MYDEBUG & INFO] - SnmpSetting::set_device_name() :"
//             << " slave_num = " << slave_num
//             << " name = " << name
//             << " cmd_index = " << cmd_index;

    QString oid = ".1.3.6.1.4.1.30966.7."
                  + QString::number(slave_num+1) + ".1.1" ;

    if(cmd_index == -1)
    {
        setOidValue(channel_name, oid, name);
    }
    else
    {
        stringstream str_s;
        str_s << cmd_index;
        string cmd_str = str_s.str();

        QString data = QString::fromStdString(cmd_str) + "#" + name;

        setOidValue(channel_name, oid, data);
    }

    return 0;
}

int SnmpSetting::set_output_name(const QString channel_name,
                                 int slave_num,
                                 int output_num,
                                 QString &name,
                                 int cmd_index)
{
//    qDebug() << "[MYDEBUG & INFO] - SnmpSetting::set_output_name() :"
//             << " slave_num = " << slave_num
//             << " output_num = " << output_num
//             << " name = " << name
//             << " cmd_index = " << cmd_index;

    QString oid = ".1.3.6.1.4.1.30966.7."
                  + QString::number(slave_num+1)
                  + ".1.8."
                  + QString::number(output_num);

    if(cmd_index == -1)
    {
        setOidValue(channel_name, oid, name);
    }
    else
    {
        stringstream str_s;
        str_s << cmd_index;
        string cmd_str = str_s.str();

        QString data = QString::fromStdString(cmd_str) + "#" + name;

        setOidValue(channel_name, oid, data);
    }

    return 0;
}

int SnmpSetting::set_output_threshold(const QString channel_name,
                                      int slave_num,
                                      int output_num,
                                      int type,
                                      int value,
                                      int cmd_index)
{
//    qDebug() << "[MYDEBUG & INFO] - SnmpSetting::set_output_threshold() :"
//             << " slave_num = " << slave_num
//             << " output_num = " << output_num
//             << " type = " << type
//             << " value = " << value
//             << " cmd_index = " << cmd_index;

    QString oid = ".1.3.6.1.4.1.30966.7." + QString::number(slave_num+1);

    if(1 == type)/* 输出位电流最小值 */
    {
        oid += ".1.11." + QString::number(output_num);
    }
    else if(2 == type)/* 输出位电流最大值 */
    {
        oid += ".1.12." + QString::number(output_num);
    }
    else if(3 == type)/* 输出位电流预警最小值 */
    {
        oid += ".1.20." + QString::number(output_num);
    }
    else if(4 == type)/* 输出位电流预警最大值 */
    {
        oid += ".1.21." + QString::number(output_num);
    }

    if(cmd_index == -1)
    {
        stringstream str_s;
        str_s << value;
        string data_str = str_s.str();
        QString data = QString::fromStdString(data_str);

        setOidValue(channel_name, oid, data);
    }
    else
    {
        stringstream str_s;
        str_s << cmd_index;
        string cmd_str = str_s.str();

        QString data = QString::fromStdString(cmd_str) + "#";

        stringstream str_s_tmp;
        cmd_str.clear();
        str_s_tmp << value;
        cmd_str = str_s_tmp.str();

        data += QString::fromStdString(cmd_str);

        setOidValue(channel_name, oid, data);
    }

    return 0;
}

int SnmpSetting::set_line_config(const QString channel_name,
                                 int slave_num,
                                 int line_num,
                                 int fun_type,
                                 int type,
                                 int value,
                                 int cmd_index)
{
//    qDebug() << "[MYDEBUG & INFO] - SnmpSetting::set_line_config() :"
//             << " slave_num = " << slave_num
//             << " line_num = " << line_num
//             << " fun_type = " << fun_type
//             << " type = " << type
//             << " value = " << value
//             << " cmd_index = " << cmd_index;

    QString str_line_num;
    if(1 == line_num)
    {
        str_line_num = "4.1";
    }
    else if(2 == line_num)
    {
        str_line_num = "4.2";
    }
    else if(3 == line_num)
    {
        str_line_num = "5.1";
    }
    else if(4 == line_num)
    {
        str_line_num = "5.2";
    }
    else if(5 == line_num)
    {
        str_line_num = "6.1";
    }
    else if(6 == line_num)
    {
        str_line_num = "6.2";
    }
    else
    {
        qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::set_line_config() error line_num!" << line_num;

        return -1;
    }

    QString oid = ".1.3.6.1.4.1.30966.7."
                  + QString::number(slave_num+1)
                  + ".1." + str_line_num;

    if(1 == fun_type)
    {
        if(1 == type)/* 该回路的该相的电流最小值 */
        {
            oid += ".6";
        }
        else if (2 == type)/* 该回路的该相的电流最大值 */
        {
            oid += ".7";
        }
    }
    else if(2 == fun_type)
    {
        if(1 == type)/* 该回路的该相的电压最小值 */
        {
            oid += ".8";
        }
        else if (2 == type)/* 该回路的该相的电压最大值 */
        {
            oid += ".9";
        }
    }
    else
    {
        qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::set_line_config() error fun type!" ;
    }

    if(cmd_index == -1)
    {
        if(1 == fun_type)
        {
            stringstream str_s;
            str_s << value;
            string data_str = str_s.str();
            QString data = QString::fromStdString(data_str);

            setOidValue(channel_name, oid, data);
        }
        else if(2 == fun_type)
        {
            setOidValue(channel_name, oid, value);
        }
    }
    else
    {
        if(1 == fun_type)
        {
            stringstream str_s;
            str_s << cmd_index;
            string cmd_str = str_s.str();

            QString data = QString::fromStdString(cmd_str) + "#";

            stringstream str_s_tmp;
            cmd_str.clear();
            str_s_tmp << value;
            cmd_str = str_s_tmp.str();
            data += QString::fromStdString(cmd_str);

            setOidValue(channel_name, oid, data);
        }
        else if(2 == fun_type)
        {
            if(value > 0x00ffffff)
            {
                qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::set_line_config() value > 0x00ffffff!" ;
            }
            else
            {
                int data = value | (cmd_index << 24);
                setOidValue(channel_name, oid, data);
            }
        }
    }

    return 0;
}

int SnmpSetting::set_temp_config(const QString channel_name,
                                 int slave_num,
                                 int senser_type,
                                 int senser_num,
                                 int value_type,
                                 int value,
                                 int cmd_index)
{
//    qDebug() << "[MYDEBUG & INFO] - SnmpSetting::set_line_config() :"
//             << " slave_num = " << slave_num
//             << " senser_type = " << senser_type
//             << " senser_num = " << senser_num
//             << " value_type = " << value_type
//             << " value = " << value
//             << " cmd_index = " << cmd_index;

    QString oid = ".1.3.6.1.4.1.30966.7." + QString::number(slave_num+1) + ".1.7.";
    if(1 == senser_type)/* 温度传感器 */
    {
        oid += QString::number(senser_num);
    }
    else if (2 == senser_type)/* 湿度传感器 */
    {
        oid += QString::number(senser_num+2);
    }
    else
    {
        //qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::set_temp_config() error senser type!" ;
    }

    if(1 == value_type)
    {
        oid += ".1";
    }
    else if(2 == value_type)
    {
        oid += ".2";
    }
    else if(3 == value_type)
    {
        oid += ".3";
    }
    else if(4 == value_type)
    {
        oid += ".4";
    }
    else
    {
        //qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::set_temp_config() error value type!" ;
    }
    //qDebug() << "[MYDEBUG : INFO] - oid = " << oid;

    if(cmd_index == -1)
    {
        setOidValue(channel_name, oid, value);
    }
    else
    {
        if(value > 0x00ffffff)
        {
            //qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::set_temp_config() value > 0x00ffffff!" ;
        }
        else
        {
            int data = value | (cmd_index << 24);
            setOidValue(channel_name, oid, data);
        }
    }

    return 0;
}

bool SnmpSetting::setOidValue(const QString &ip,QString &oidString, const QVariant &var)
{
    if(!oidString.endsWith(".0"))oidString+=".0";
    Oid oid(oidString.toUtf8().constData());

    UdpAddress address(ip.toUtf8().constData());
    address.set_port(161);
    m_ctarget.set_address(address);

    int status;
    Vb vb(oid);//包中一个对应oid-value对。也是返回包参数
    Pdu pdu(&vb,1);
    Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));
    if ( status == SNMP_CLASS_SUCCESS)
    {
        status = snmp.get(pdu,m_ctarget);
        if(status == SNMP_CLASS_SUCCESS)
        {
            pdu.get_vb(vb,0);
            if(MibConvert::setVbValue(vb,var))
            {
                pdu.set_vb(vb,0);
                if(SNMP_CLASS_SUCCESS == snmp.set( pdu, m_ctarget))
                    return true;
            }
        }
    }
    else
    {
//        qDebug() << "[MYDEBUG : ERROR] - SnmpSetting::setOidValue()"
//                 <<" oidString = " << vb.get_printable_oid()
//                 << " error = " << snmp.error_msg(status);
        return false;
    }
}
