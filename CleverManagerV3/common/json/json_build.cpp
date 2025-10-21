/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "json_build.h"


Json_Build::Json_Build()
{

}

Json_Build *Json_Build::bulid()
{
    static Json_Build* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Build();
    }
    return sington;
}

bool Json_Build::getJson(sDataPacket *packet, QJsonObject &json)
{
    bool ret = true;

    if(packet->offLine > 0) {
        head(packet, json);
        devData(packet, json);
        //'saveJson("cc", json);
    } else {
        ret = false;
    }

    return ret;
}

bool Json_Build::saveJson(const QString &name, QJsonObject &json)
{
    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();
    QFile file("F:/" + name+".json");
    bool ret = file.open(QIODevice::WriteOnly);
    if(ret) {
        file.write(ba);
        file.close();
    } else {
        qDebug() << "write json file failed";
    }

    return ret;
}

void Json_Build::head(sDataPacket *packet,QJsonObject &obj)
{
    obj.insert("company", "CLEVER");
    obj.insert("version", JSON_VERSION);

    uutInfo(packet, obj);
    pduInfo(packet, obj);
}

void Json_Build::uutInfo(sDataPacket *packet, QJsonObject &json)
{
    QJsonObject obj;

    obj.insert("room", packet->room);
    obj.insert("cabinet", packet->cab);
    obj.insert("road", packet->road);

    json.insert("uut_info", QJsonValue(obj));
}

void Json_Build::pduInfo(sDataPacket *packet, QJsonObject &json)
{
    QJsonObject obj;
    obj.insert("pdu_ip",  packet->net.ip);
    obj.insert("pdu_num", packet->dev_num); //

    obj.insert("pdu_type", packet->dev_type);
    obj.insert("pdu_phase", packet->phase);/////////////////////////////////////暂时写死，可能没有值  pmd 2020-3-11
    obj.insert("pdu_spec", packet->devSpec);
    obj.insert("pdu_name", packet->name);
    obj.insert("pdu_dc", packet->dc);
    obj.insert("pdu_hz", packet->hz);
    obj.insert("pdu_ver", packet->version);

    json.insert("pdu_info", QJsonValue(obj));
}



void Json_Build::thresholdItem(const QString &str,int i, sDataUnit &unit, double rate, QJsonObject &json)
{
    sConfigItem *item = ConfigBase::bulid()->item;
    if(item->thresholdEn) {
        json.insert(str+"_min", unit.min[i] / rate);
        json.insert(str+"_max", unit.max[i] / rate);

        if(unit.crMax[i]) {
            json.insert(str+"_cr_max", unit.crMax[i] / rate);
            json.insert(str+"_cr_min", unit.crMin[i] / rate);
        }
    }
}

int Json_Build::objData(const QString &str, sObjData &ObjData, QJsonObject &obj)
{
    QJsonArray jsonArray;
    int num = ObjData.size;
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("id", i+1);
        subObj.insert("name", ObjData.name[i]);

        int value = ObjData.vol.value[i];
        if(value > 0) {
            subObj.insert("vol", value / COM_RATE_VOL);
            thresholdItem("vol", i, ObjData.vol, COM_RATE_VOL, subObj);
        }

        value = ObjData.cur.value[i];
        if(value >= 0) {
            subObj.insert("cur", value / COM_RATE_CUR);
            thresholdItem("cur", i, ObjData.cur, COM_RATE_CUR, subObj);
        }

        value = ObjData.pow[i];
        if(value >= 0) subObj.insert("pow", value / COM_RATE_POW);

        value = ObjData.ele[i];
        if(value >= 0) subObj.insert("ele", value / COM_RATE_ELE);

        value = ObjData.pf[i];
        if(value >= 0) subObj.insert("PF", value / COM_RATE_PF);

        value = ObjData.sw[i];
        if(value >= 0) subObj.insert("switch", value);

        jsonArray.append(subObj);
    }
    if(num > 0) obj.insert(QString("%1_item_list").arg(str), QJsonValue(jsonArray));

    return num;
}


void Json_Build::envItem(const QString &str, sDataUnit &unit, double rate, QJsonObject &obj)
{
    QJsonArray jsonArray;
    for(int i=0; i<unit.size; ++i) {
        QJsonObject json;
        json.insert("id",  i+1);
        json.insert("name",  QString(str+" %1").arg(i+1));
        json.insert("status", unit.value[i] / rate);
        thresholdItem(str, i, unit, rate, json);
        jsonArray.append(json);
    }
    if(unit.size) obj.insert(QString("%1_items").arg(str), QJsonValue(jsonArray));
}

void Json_Build::envItem(const QString &str, uchar* index, QJsonObject &obj)
{
    QJsonArray jsonArray;
    int i = 0;

    QJsonObject json;
    json.insert("id",  i+1);
    json.insert("name",  QString(str+" %1").arg(i+1));
    json.insert("status", index[i]);
    jsonArray.append(json);
    obj.insert(QString("%1_items").arg(str), QJsonValue(jsonArray));
}


void Json_Build::envs(sEnvData &ObjData, QJsonObject &json)
{
    QJsonObject obj;
    switch(ObjData.type_index)
    {
        case 1:envItem("tem", ObjData.tem , COM_RATE_TEM, obj );break;
        case 2:envItem("hum", ObjData.hum , COM_RATE_HUM, obj );break;
        case 3:envItem("door", ObjData.door , obj );break;
        case 4:break;
        case 5:envItem("water", ObjData.water , obj );break;
        case 6:envItem("smoke", ObjData.smoke , obj );break;
    }

    json.insert("env_item_list", QJsonValue(obj));
}

void Json_Build::netAddr(sIpAddr &net, QJsonObject &json)
{
    QJsonObject obj;

    obj.insert("mode", net.mode);
    obj.insert("ip", net.ip);
    obj.insert("dns", net.dns);
    obj.insert("gw", net.gw);
    obj.insert("mask", net.mask);

    json.insert("net_addr", QJsonValue(obj));
}

void Json_Build::devData(sDataPacket *packet, QJsonObject &obj)
{
    sDevData *devData = &(packet->data);
    objData("line", devData->line, obj);
    objData("loop", devData->loop, obj);
    objData("output", devData->output, obj);

    sConfigItem *item = ConfigBase::bulid()->item;
    if(item->envEn) {
        envs(devData->env, obj);
    }

    if(item->netAddrEn) {
        netAddr(packet->net, obj);
    }

    for(int i=0; i<6; ++i)  {
        obj.insert("segment"+QString::number(i+1), "");
    }
}

