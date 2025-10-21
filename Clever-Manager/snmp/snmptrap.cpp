#include "snmptrap.h"
#include <QDebug>
#include <QDateTime>


SnmpTrap::SnmpTrap(QObject *parent) :
    QThread(parent),m_run(false)
{

}

SnmpTrap::~SnmpTrap()
{
    stop();
    quit();
    wait();
}

//停止trap
void SnmpTrap::stop()
{
    m_run = false;
    requestInterruption();
}

void SnmpTrap::run()
{
    m_run = true;
    int status;
    Snmp snmp(status);
    if ( status != SNMP_CLASS_SUCCESS)
    {
        emit backInfo(QString("SNMP++ Session Create Fail:") + snmp.error_msg(status));
        return;
    }
    int trap_port=162;
    snmp.notify_set_listen_port(trap_port);
    OidCollection oidc;
    TargetCollection targetc;
    status = snmp.notify_register(oidc, targetc,callback,(const void*)this);
    if (status != SNMP_CLASS_SUCCESS)
    {
        emit backInfo(QString("Error register for notify") + snmp.error_msg(status));
        return;
    }

    while(!isInterruptionRequested())
    {
        snmp.get_eventListHolder()->SNMPProcessEvents(10);

    }

    emit backInfo("Stop Snmp trap.");
}

void SnmpTrap::callback( int reason, Snmp *snmp, Pdu &pdu, SnmpTarget &target, void *cd)
{
    SnmpTrap* thread = reinterpret_cast<SnmpTrap*>(cd);

    //当前时间
    //QString curTime= QDateTime::currentDateTime().toString("yy-M-d  h:m:s");

    //ip地址和端口
    QString ipAddr=target.get_address().get_printable();

    int errType = pdu.get_error_status();
    Vb vb;
    pdu.get_vb(vb, 0);
    QString oid = vb.get_printable_oid();//oid
    QString info= vb.get_printable_value();//告警信息
    thread->trapInfo(errType,ipAddr,oid,info);//发送信号出去，时间在外面给定
}
