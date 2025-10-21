/*
 *
 *
 *  Created on: 2020年3月9日
 *      Author: Lzy
 */
#include "json_recv.h"
#include "net_dataanalyze.h"

Json_Recv::Json_Recv(QObject *parent) : QObject(parent)
{
    mUdpSocket = new QUdpSocket(this);
    bool ret = mUdpSocket->bind(4545);
    if(ret)
        connect(mUdpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
    else
        qDebug()<<"socket bind err";
}

Json_Recv::~Json_Recv()
{

}

Json_Recv *Json_Recv::bulid(QObject *parent)
{
    static Json_Recv* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Recv(parent);
    }
    return sington;
}

QJsonValue Json_Recv::getValue(QJsonObject &object, const QString &key)
{
    QJsonValue value;
    if (object.contains(key))  {
        value = object.value(key);
    }
    return value;
}

QString Json_Recv::getString(QJsonObject &object, const QString &key)
{
    QString str;
    QJsonValue value = getValue(object, key);
    if(value.isString()) {
        str = value.toString();
    }

    return str;
}

double Json_Recv::getData(QJsonObject &object, const QString &key)
{
    double ret = -1;
    QJsonValue value = getValue(object, key);
    if(value.isDouble()) {
        ret = value.toDouble();
    }

    return ret;
}


QJsonObject Json_Recv::getObject(QJsonObject &object, const QString &key)
{
    QJsonObject obj;
    if (object.contains(key)){
        QJsonValue value = object.value(key);
        if (value.isObject()){
            obj = value.toObject();
        }
    }
    return obj;
}

QJsonArray Json_Recv::getArray(QJsonObject &object, const QString &key)
{
    QJsonArray array;
    if (object.contains(key)) {
        QJsonValue value = object.value(key);
        if (value.isArray()) {
            array = value.toArray();
        }
    }

    return array;
}


bool Json_Recv::company(QJsonObject &object)
{
    bool ret = false;
    QString strName = getString(object, "company");
    ret = strName.contains("CLEVER");

    return ret;
}


bool Json_Recv::versionNumber(QJsonObject &object)
{
    bool ret = company(object);
    if(ret) {
        int v = getData(object, "version");
        if(v == JSON_VERSION) ret = true;
    }

    return ret;
}



bool Json_Recv::headInfo(QJsonObject &object, sJsonCmd &cmd)
{
    bool ret = true;
    QJsonObject obj = getObject(object, "pdu_info");
    cmd.ip = getString(obj, "pdu_ip");
    cmd.dev_type = getString(obj, "pdu_type");
    cmd.dev_num = getData(obj, "pdu_num");
    cmd.dev_spec = getData(obj, "pdu_spec");
    cmd.dev_name = getString(obj, "pdu_name");

    int type = PDU_TYPE_SI_PDU;
    QString str = cmd.dev_type;
    if(str.contains("SI-PDU")) type = PDU_TYPE_SI_PDU;
    if(str.contains("IP-PDU")) type = PDU_TYPE_IP_PDU;
    if(str.contains("NPM")) type = PDU_TYPE_NPM_PDU;
    if(str.contains("MPDU")) type = PDU_TYPE_MPDU;
    if(str.contains("RPDU")) type = PDU_TYPE_RPDU;
    if(str.contains("ZPDU")) type = PDU_TYPE_ZPDU;
    cmd.pduType = type;

    return ret;
}

uchar *Json_Recv::getData(sJsonCmd &cmd)
{
    static uchar buf[32] = {0};
    if((cmd.min >= 0) && (cmd.max >= 0)){
        cmd.len = 8;
    } else {
        cmd.len = 0;
        return buf;
    }

    int offset = 0;
    int min = cmd.min * cmd.rate;
    buf[offset++] = (min >> 8)&0xFF;
    buf[offset++] = min & 0xFF;

    int max = cmd.max * cmd.rate;
    buf[offset++] = (max >> 8)&0xFF;
    buf[offset++] = max & 0xFF;

    if((cmd.cr_min >= 0) && (cmd.cr_max >= 0)) {
        min = cmd.cr_min * cmd.rate;
        buf[offset++] = (min >> 8)&0xFF;
        buf[offset++] = min & 0xFF;

        max = cmd.cr_max * cmd.rate;
        buf[offset++] = (max >> 8)&0xFF;
        buf[offset++] = max & 0xFF;
    } else {
        cmd.len = 4;
    }

    return buf;
}


bool Json_Recv::sentData(sJsonCmd &cmd)
{
    Net_sDevData pkt;

    pkt.addr = cmd.dev_num;
    pkt.fn[0] = cmd.fn;
    pkt.fn[1] = cmd.id;
    pkt.data = getData(cmd);
    pkt.len = cmd.len;
    if(pkt.len < 2) return false;

    QString ip = cmd.ip;
    if(ip.isEmpty()) { // 广播数据包
        pkt.addr = 0xff;
    }

    uchar buf[64] = {0};
    NetPackData *pack = NetPackData::bulid();
    int len = pack->net_data_packets(cmd.pduType, &pkt, buf);
    if(ip.isEmpty()) {
        UdpBDSent::bulid(this)->sent(buf, len);
    } else {
        UdpSentSocket::bulid(this)->sentData(ip, buf, len);
        Sleep(10);
    }

    return true;
}

bool Json_Recv::itemValues(QJsonObject &obj, sJsonCmd &cmd, const QString &key)
{
    cmd.id = getData(obj, "id");
    cmd.min = getData(obj, QString("%1_min").arg(key));
    cmd.max = getData(obj, QString("%1_max").arg(key));
    cmd.cr_min = getData(obj, QString("%1_critical_min").arg(key));
    cmd.cr_max = getData(obj, QString("%1_critical_max").arg(key));

    return sentData(cmd);
}

bool Json_Recv::lineItem(QJsonObject &obj, sJsonCmd &cmd)
{
    cmd.fn = 1;
    itemValues(obj,cmd, "vol");

    cmd.fn = 2;
    itemValues(obj,cmd, "cur");

    return true;
}

bool Json_Recv::lineItemList(QJsonObject &object, sJsonCmd &cmd)
{
    QJsonArray array = getArray(object, "line_item_list");
    int nSize = array.size();
    for (int i = 0; i < nSize; ++i)
    {
        QJsonObject obj = array.at(i).toObject();
        lineItem(obj, cmd);
    }

    return true;
}


/**
 * @brief 发送设备数据
 * @return true
 */
bool Json_Recv::setSwitch(sJsonCmd &cmd)
{
    uchar data[4] = {0};
    if(cmd.sw < 0) {
        return false;
    } else {
        data[0] = cmd.sw;
        cmd.sw = -1;
    }

    Net_sDevData pkt;
    pkt.addr = cmd.dev_num;
    pkt.fn[0] = cmd.fn;
    pkt.fn[1] = cmd.id;
    pkt.data = data;
    pkt.len = 1;
    pkt.fn[0] = 13; // 输出位功能码

    QString ip = cmd.ip;
    if(ip.isEmpty()) { // 统一设置
        pkt.fn[0] = 18;  ////====
        pkt.addr = 0xff;
    }

    uchar buf[64] = {0};
    NetPackData *pack = NetPackData::bulid();
    int len = pack->net_data_packets(cmd.pduType, &pkt, buf);
    if(ip.isEmpty()) {
        UdpBDSent::bulid(this)->sent(buf, len);
    } else {
        UdpSentSocket::bulid(this)->sentData(ip, buf, len);
    }

    return true;
}


bool Json_Recv::outputItem(QJsonObject &obj, sJsonCmd &cmd)
{
    cmd.fn = 0;
    itemValues(obj,cmd, "cur");
    cmd.sw = getData(obj, "switch");

    return setSwitch(cmd);
}

bool Json_Recv::outputItemList(QJsonObject &object, sJsonCmd &cmd)
{
    QJsonArray array = getArray(object, "output_item_list");
    int nSize = array.size();
    for (int i = 0; i < nSize; ++i)
    {
        QJsonObject obj = array.at(i).toObject();
        outputItem(obj, cmd);
    }

    return true;
}


bool Json_Recv::temItemList(QJsonObject &object, sJsonCmd &cmd)
{
    QJsonArray array = getArray(object, "tem_item_list");
    int nSize = array.size();
    for (int i = 0; i < nSize; ++i)
    {
        cmd.fn = 3;
        QJsonObject obj = array.at(i).toObject();
        itemValues(obj, cmd, "tem");
    }

    return true;
}

bool Json_Recv::humItemList(QJsonObject &object, sJsonCmd &cmd)
{
    QJsonArray array = getArray(object, "hum_item_list");
    int nSize = array.size();
    for (int i = 0; i < nSize; ++i)
    {
        cmd.fn = 4;
        QJsonObject obj = array.at(i).toObject();
        itemValues(obj, cmd, "hum");
    }

    return true;
}


bool Json_Recv::envInfo(QJsonObject &object, sJsonCmd &cmd)
{
    QJsonObject obj = getObject(object, "env_info");
    temItemList(obj, cmd);
    humItemList(obj, cmd);

    return true;
}

bool Json_Recv::analyticalData(QJsonObject &object)
{
    sJsonCmd cmd;
    cmd.rate = 10;

    bool ret = headInfo(object, cmd);
    if(ret) {
        lineItemList(object, cmd);
        outputItemList(object, cmd);
        envInfo(object, cmd);
    }

    return ret;
}

bool Json_Recv::recv(const QString &msg)
{
    bool ret = false;
    QJsonParseError jsonerror;
    QJsonDocument doc = QJsonDocument::fromJson(msg.toLatin1(), &jsonerror);
    if (!doc.isNull() && jsonerror.error == QJsonParseError::NoError) {
        if(doc.isObject()) {
            QJsonObject object = doc.object();
            if(versionNumber(object)) {
                ret = analyticalData(object);
            }
        }
    }

    return ret;
}

void Json_Recv::processPendingDatagram() //处理等待的数据报
{
    if(mUdpSocket->hasPendingDatagrams()) //拥有等待的数据报
    {
        QByteArray datagram; //拥于存放接收的数据报
        datagram.resize(mUdpSocket->pendingDatagramSize());
        mUdpSocket->readDatagram(datagram.data(),datagram.size());
        recv(datagram);

        //        unsigned short size = datagram.size();
        //        unsigned char msg[10] = {0};
        //        memcpy(msg, datagram.data(), size);
        //        qDebug()<<msg[0]<<msg[1]<<msg[2]<<msg[3];
    }
}

void Json_Recv::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
