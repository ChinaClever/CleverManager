#include "common_snmp.h"
#include "common.h"
#include "pdu/data/hash/read/pduhashread.h"
#include "pdudevlistsave.h"

/**
 * @brief MibConvert::takeoff
 * 查找字符串字首是否有key对应的字符串，有的话，截去并返回true
 * @param str
 * @param key
 * @return
 */
bool MibConvert::takeoff(QString &str, const QString &key)
{
    if(str.startsWith(key)){
        str = str.mid(key.size());
        return true;
    }
    return false;
}

/**
 * @brief MibConvert::takeoffNum
 * 将字符串自首中1~24的字符串表示转换为int值，并从字符串截去数字表示部分。
 * @param str
 * @return
 */
int MibConvert::takeoffNum(QString &str)
{
    if(takeoff(str,"teen"))     return 10;
    if(takeoff(str,"One"))      return 1;
    if(takeoff(str,"Two"))      return 2;
    if(takeoff(str,"Three"))    return 3;
    if(takeoff(str,"Four"))     return 4 + takeoffNum(str);
    if(takeoff(str,"Five"))     return 5;
    if(takeoff(str,"Six"))      return 6 + takeoffNum(str);
    if(takeoff(str,"Seven"))    return 7 + takeoffNum(str);
    if(takeoff(str,"Eight")){
        if(takeoff(str,"een"))  return 18;
        return 8;
    }
    if(takeoff(str,"Nine"))     return 9 + takeoffNum(str);
    if(takeoff(str,"Ten"))      return 10;
    if(takeoff(str,"Eleven"))   return 11;
    if(takeoff(str,"Twelve"))   return 12;
    if(takeoff(str,"Thirteen")) return 13;
    if(takeoff(str,"Fifteen"))  return 15;
    if(takeoff(str,"Twenty"))   return 20 + takeoffNum(str);
    return 0;
}

/**
 * @brief MibConvert::takeoffMSNum
 * 截去主副机编号
 * @param str
 * @return
 */
int MibConvert::takeoffMSNum(QString &str)
{
    if(takeoff(str,"Master")) return 0;
    if(takeoff(str,"slave")) return takeoffNum(str);
}

/**
 * @brief MibConvert::deviceNumToString
 * 从序列化转换为主副机字符串表示
 * @param i
 * @return
 */
QString MibConvert::deviceNumToString(int i)
{
    switch (i) {
    case 0:return "Master";
    case 1:return "slaveOne";
    case 2:return "slaveTwo";
    case 3:return "slaveThree";
    case 4:return "slaveFour";
    case 5:return "slaveFive";
    case 6:return "slaveSix";
    case 7:return "slaveSeven";
    case 8:return "slaveEight";
    case 9:return "slaveNine";
    }
    return "";
}

QString MibConvert::numToString(int i)
{
    switch (i) {
    case 1: return "One";
    case 2: return "Two";
    case 3: return "Three";
    case 4: return "Four";
    case 5: return "Five";
    case 6: return "Six";
    case 7: return "Seven";
    case 8: return "Eight";
    case 9: return "Nine";
    case 10:return "Ten";
    case 11:return "Eleven";
    case 12:return "Twelve";
    case 13:return "Thirteen";
    case 14:return "Fourteen";
    case 15:return "Fifteen";
    case 16:return "Sixteen";
    case 17:return "Seventeen";
    case 18:return "Eighteen";
    case 19:return "Nineteen";
    case 20:return "Twenty";
    case 21:return "TwentyOne";
    case 22:return "TwentyTwo";
    case 23:return "TwentyThree";
    case 24:return "TwentyFour";
    }
    return "";
}

/**
 * @brief MibConvert::oid2Name
 * 在数据库中通过oid找到对应名称
 * @param oid
 * @return
 */
OidItem MibConvert::oid2Item(QString oid)
{

    static QMutex mutex;
    static QMap<QString,OidItem> s_map;

    if(!oid.startsWith('.'))oid = '.' + oid;
    if(oid.endsWith(".0"))oid.truncate(oid.size()-2);

    QMutexLocker locker(&mutex);
    OidItem& oItem = s_map[oid];
    if(oItem.id < 0){
        QVector<OidItem> items = DbOidTable::get()->selectItemByOid(oid);
        if(items.size() == 1)
            oItem = items.first();
    }

    if(oItem.id < 0)
    {
        qDebug() << QString("Invalid Oid %1 that can't be parsed...").arg(oid);
    }

    return oItem;
}

OidItem MibConvert::name2OidItem(int pduId, const QString &name)
{
    static QMap<QPair<int,QString>,OidItem> s_map;
    OidItem& oItem = s_map[QPair<int,QString>(pduId,name)];
    if(oItem.id < 0){
        QVector<OidItem> items = DbOidTable::get()->selectItemByName(pduId,name);
        if(items.size() == 1)
            oItem = items.first();
    }
    return oItem;
}

/**
 * @brief MibConvert::padPacket
 * 填充oid对应数据到packet
 * @param packet
 * @param oid
 * @param vb
 */
int MibConvert::padPacket(PduDataPacket *packet, const Vb &vb)
{
    QString oid = vb.get_printable_oid();
    if(0 == oid.compare(".1.3.6.1.4.1.30966.7.1.1.14"))
    {
        return 1;
    }

    //将Vb类型值转换为QVariant类型。
    QVariant var = vbToVariant(vb);
    SNMP_DEBUG_OUTPUT("vb Value:" << vb.get_printable_value());
    return padPacketByOid(packet,vb.get_printable_oid(),var);
}

int MibConvert::padPacket(const QString &ip, const QString &oid, const QVariant var)
{

    //从oid得到mib对应名称

    QString mibName = oid2Item(oid).name;
    SNMP_DEBUG_OUTPUT("padPacket:" << oid << mibName << var);
    QString numString = mibName;
    int num = takeoffMSNum(numString);
    PduDataPacket *packet = pdu_hd_getPacket((QString &)ip,num);
    if(packet == NULL)
    {
        /*qDebug() << "[MYDEBUG : INFO] - MibConvert::padPacket() creat ip = "
                 << ip
                 << " slave_num = "
                 << num
                 << " PDU device.";*/

        PduHashIP *hashIP = NULL;
        PduDevHash *devHash = NULL;
        PduDataPacket *dev = NULL;
        int dev_type = 0x01050101;

        hashIP =  pdu_hashData_get(dev_type);
        if(NULL == hashIP)
        {
//            qDebug() << "[MYDEBUG : INFO] - MibConvert::padPacket() get ip = "
//                     << ip
//                     << " hashIP is empty!";

            return 1;
        }
        devHash = hashIP->get(ip);
        if(NULL == devHash)
        {
//            qDebug() << "[MYDEBUG : INFO] - MibConvert::padPacket() get ip = "
//                     << ip
//                     << " devHash is empty!";

            return 1;
        }
        dev = devHash->get(num);
        if(NULL == dev)
        {
//            qDebug() << "[MYDEBUG : INFO] - MibConvert::padPacket() get ip = "
//                     << ip
//                     << " dev is empty!";

            return 1;
        }

        dev->devType = dev_type;
        dev->devNum = num;
        dev->ip->set(ip);
        dev->offLine = PDU_OFF_LINE_TIME;

        if(devHash->isNew)
        {
            dev->info->addr->group->set("ZPDU");
            dl_save_add(dev->ip->get());
        }

        PduDataPacket *packet_temp = pdu_hd_getPacket((QString &)ip,num);
        if(NULL == packet_temp)
        {
            qDebug() << "[MYDEBUG : ERROR] - MibConvert::padPacket() creat ip = "
                     << ip
                     << " device error!";

            return 1;

        }

        return padPacketByMibName(dev, mibName, var);
    }
    else
    {
        return padPacketByMibName(packet, mibName, var);
    }
}

int MibConvert::padPacketByOid(PduDataPacket *packet,const QString &oid, const QVariant var)
{
    if(0 == oid.compare("1.3.6.1.4.1.30966.7.1.1.14.0"))
    {
        return 1;
    }

    //从oid得到mib对应名称
    QString mibName = oid2Item(oid).name;
    SNMP_DEBUG_OUTPUT("padPacket:" << oid << mibName << packet->devNum << var);
    return padPacketByMibName(packet,mibName,var);
}

int MibConvert::padPacketByMibName(PduDataPacket *packet,QString& mibName,const QVariant var)
{
    /*qDebug() << "[MYDEBUG : INFO] MibConvert::padPacketByOid() -"
             << " mibName = " << mibName
             << " var = " << var;*/
    //截去主副机字符串编码
    if(takeoffMSNum(mibName) != packet->devNum)
    {
        qDebug() << QString("mib name(%1) is confilct with the packet's device number(%2)").arg(mibName).arg(packet->devNum);
        return -1;
    }

    if(mibName == "Name")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Name = " << var.toString();
        packet->info->type->name->clear();
        packet->info->type->name->set(var.toString());
    }
    else if(mibName == "Addr")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Addr = " << var.toString();
        //packet->ip->set(var.toString());
    }
    else if(mibName == "Type")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Type = " << var.toInt();
        int device_type = var.toInt();
        if((1 <= device_type) && (device_type <= 12))
        {
            packet->devType = 0x01050101;
        }
        else
        {
            qDebug() << "[MYDEBUG : ERR] - MibConvert::padPacketByMibName()"
                     << " error devType = " << device_type;
            packet->devType = 0x01050101;
        }
    }
    else if(mibName == "Language")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Language = " << var.toString();
    }
    else if(mibName == "OutputNumber")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- OutputNumber = " << var.toInt();
        /* 目前管理端没用到 */
    }
    else if(mibName == "Frequency")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Frequency = " << var.toString();
    }
    else if(mibName == "Buzzer")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Buzzer = " << var.toString();
    }
    else if(mibName == "Light")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Light = " << var.toString();
    }
    else if(mibName == "BreakerNum")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- BreakerNum = " << var.toString();
    }
    else if(mibName == "Linebit")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- Linebit = " << var.toString();
    }
    else if(takeoff(mibName,"Line"))
    {
        PduObjData *line = packet->data->line;

        //判断是哪相的
        int index = takeoffNum(mibName)-1;
        if(index < 0)
        {
            return -2;
        }
        else
        {
            //填充对应相数据
            if(mibName == "Power")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " Power = " << variant2float(var);
                line->pow->set(index,variant2float(var)*COM_RATE_POW);
            }
            else if(mibName == "PF")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " PF = " << variant2float(var);
                line->pf->set(index,variant2float(var)*COM_RATE_PF);
            }
            else if(mibName == "Energy")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " Energy = " << variant2float(var);
                line->ele->set(index,variant2float(var)*COM_RATE_ELE);
            }
            else if(mibName == "CurrentMinCrit")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " CurrentMinCrit = " << variant2float(var);
                line->cur->crMin->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "CurrentMin")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " CurrentMin = " << variant2float(var);
                line->cur->min->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "CurrentMaxCrit")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " CurrentMaxCrit = " << variant2float(var);
                line->cur->crMax->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "CurrentMax")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " CurrentMax = " << variant2float(var);
                line->cur->max->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "Current")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " Current = " << variant2float(var);
                line->cur->value->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "VoltageMinCrit")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " VoltageMinCrit = " << variant2float(var);
                line->vol->crMin->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "VoltageMin")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " VoltageMin = " << variant2float(var);
                line->vol->min->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "VoltageMaxCrit")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " VoltageMaxCrit = " << variant2float(var);
                line->vol->crMax->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "VoltageMax")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " VoltageMax = " << variant2float(var);
                line->vol->max->set(index,variant2float(var)*COM_RATE_VOL);
            }
            else if(mibName == "Voltage")
            {
                //qDebug() << "[MY DEBUG : INFO] -------- line_index = " << index << " Voltage = " << variant2float(var);
                line->vol->value->set(index,variant2float(var));
            }
            else
            {
                qDebug() << "[MY DEBUG : ERROR] -------- line_index = " << index << " error type! ";
            }
        }
    }
    else if(takeoff(mibName,"Temp"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -3;
        }
        else
        {
            if("" == mibName)
            {
                //qDebug() << "[MY DEBUG : INFO] -------- temp_index = " << index << " cr = " << variant2float(var);
                packet->data->env->tem->value->set(index,variant2float(var)*COM_RATE_TEM);
            }
            else if (takeoff(mibName,"Min"))
            {
                if("" == mibName)
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- temp_index = " << index << " min = " << variant2float(var);
                    packet->data->env->tem->min->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else if (takeoff(mibName,"Crit"))
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- temp_index = " << index << " crmin = " << variant2float(var);
                    packet->data->env->tem->crMin->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else
                {
                    qDebug() << "[MYDEBUG : ERROR] ---- padPacketByMibName() error temp min mibname = " << mibName;
                }
            }
            else if (takeoff(mibName,"Max"))
            {
                if("" == mibName)
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- temp_index = " << index << " max = " << variant2float(var);
                    packet->data->env->tem->max->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else if (takeoff(mibName,"Crit"))
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- temp_index = " << index << " crmax = " << variant2float(var);
                    packet->data->env->tem->crMax->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else
                {
                    qDebug() << "[MYDEBUG : ERROR] ---- padPacketByMibName() error temp max mibname = " << mibName;
                }
            }
            else
            {
                qDebug() << "[MYDEBUG : ERROR] ---- padPacketByMibName() error temp mibname = " << mibName;
            }
        }
    }
    else if(takeoff(mibName,"Hum"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -4;
        }
        else
        {
            if("" == mibName)
            {
                //qDebug() << "[MY DEBUG : INFO] -------- hum_index = " << index << " cr = " << variant2float(var);
                packet->data->env->hum->value->set(index,variant2float(var)*COM_RATE_HUM);
            }
            else if (takeoff(mibName,"Min"))
            {
                if("" == mibName)
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- hum_index = " << index << " min = " << variant2float(var);
                    packet->data->env->hum->min->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else if (takeoff(mibName,"Crit"))
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- hum_index = " << index << " crmin = " << variant2float(var);
                    packet->data->env->hum->crMin->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else
                {
                    qDebug() << "[MYDEBUG : ERROR] ---- padPacketByMibName() error hum min mibname = " << mibName;
                }
            }
            else if (takeoff(mibName,"Max"))
            {
                if("" == mibName)
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- hum_index = " << index << " max = " << variant2float(var);
                    packet->data->env->hum->max->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else if (takeoff(mibName,"Crit"))
                {
                    //qDebug() << "[MY DEBUG : INFO] -------- hum_index = " << index << " crmax = " << variant2float(var);
                    packet->data->env->hum->crMax->set(index,variant2float(var)*COM_RATE_TEM);
                }
                else
                {
                    qDebug() << "[MYDEBUG : ERROR] ---- padPacketByMibName() error hum max mibname = " << mibName;
                }
            }
            else
            {
                qDebug() << "[MYDEBUG : ERROR] ---- padPacketByMibName() error hum mibname = " << mibName;
            }
        }
    }
    else if(takeoff(mibName,"Door"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -5;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- door_index = " << index << " value = " << variant2float(var);
        packet->data->env->door->set(index,variant2float(var));
    }
    else if(mibName == "Smoke")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- smoke_index = " << 0 << " value = " << variant2float(var);
        packet->data->env->smoke->set(0,variant2float(var));
    }
    else if(mibName == "Water")
    {
        //qDebug() << "[MY DEBUG : INFO] -------- smoke_index = " << 0 << " value = " << variant2float(var);
        packet->data->env->water->set(0,variant2float(var));
    }
    else if(takeoff(mibName,"OutputName"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return - 5;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputName = " << var.toString();
        packet->output->name->set(index,var.toString());
    }
    else if(takeoff(mibName,"OutputStatus"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -6;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputStatus = " << var.toString();
        packet->data->output->sw->set(index,var.toString() == "1");
    }
    else if(takeoff(mibName,"OutputCurrentMinCrit"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -10;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputCurrentMinCrit = " << variant2float(var);
        packet->data->output->cur->crMin->set(index,variant2float(var)*COM_RATE_CUR);
    }
    else if(takeoff(mibName,"OutputCurrentMaxCrit"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -11;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputCurrentMaxCrit = " << variant2float(var);
        packet->data->output->cur->crMax->set(index,variant2float(var)*COM_RATE_CUR);
    }
    else if(takeoff(mibName,"OutputCurrentMin"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -7;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputCurrentMin = " << variant2float(var);
        packet->data->output->cur->min->set(index,variant2float(var)*COM_RATE_CUR);
    }
    else if(takeoff(mibName,"OutputCurrentMax"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -8;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputCurrentMax = " << variant2float(var);
        packet->data->output->cur->max->set(index,variant2float(var)*COM_RATE_CUR);
    }
    else if(takeoff(mibName,"OutputCurrentEnergy"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -9;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputCurrentEnergy = " << variant2float(var);
        packet->data->output->ele->set(index,variant2float(var));
    }
    else if(takeoff(mibName,"OutputCurrent"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -10;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputCurrent = " << variant2float(var);
        packet->data->output->cur->value->set(index,variant2float(var)*COM_RATE_CUR);
    }
    else if(takeoff(mibName,"OutputPowerFactor"))
    {
        int index = takeoffNum(mibName) - 1;
        if(index < 0)
        {
            return -12;
        }
        //qDebug() << "[MY DEBUG : INFO] -------- output_index = " << index << " OutputPowerFactor = " << variant2float(var);
        packet->data->output->pf->set(index,variant2float(var));
    }
    else if(takeoff(mibName,"OutputDelay"))
    {

    }
    else
    {
        qDebug() << "[MYDEBUG : ERROR] ---- padPacketByMibName() error mibname = " << mibName;
    }

    return 0;
}

//很多QVariant都是QString类型，需要间接转换为int型
float MibConvert::variant2float(const QVariant &var)
{
    float val = 0;
    if(var.type() == QVariant::Int)
        val = var.toInt();
    else if(var.type()== QVariant::String)
        val = var.toString().toFloat();
    else if(var.type()== QMetaType::Float)
        val = var.toFloat();
    return val;
}

QString MibConvert::variant2String(const QVariant &var)
{
    if(var.type() == QVariant::String)return var.toString();
    if(var.type() == QVariant::Int)return QString::number(var.toInt());
    if(var.type() == QVariant::LongLong)return QString::number(var.toLongLong());
}

/**
 * @brief MibConvert::vbToVariant
 * 将snmp++中的vb返回类型转换为Qt常用类型QVariant
 * @param vb
 * @return
 */
QVariant MibConvert::vbToVariant(const Vb &vb)
{
    SmiUINT32 type = vb.get_syntax();
    switch(type){
    case sNMP_SYNTAX_INT:
        //    case sNMP_SYNTAX_INT32:
    {
        int val;
        vb.get_value(val);
        return val;
    }
    case sNMP_SYNTAX_OCTETS:
    case sNMP_SYNTAX_BITS:
    case sNMP_SYNTAX_OPAQUE:
    case sNMP_SYNTAX_IPADDR:
    {
        OctetStr octetstr;
        vb.get_value(octetstr);
        return QString(QByteArray((const char*)octetstr.data(),(int)octetstr.len()));
    }
//    case sNMP_SYNTAX_UINT32:
    case sNMP_SYNTAX_CNTR32:
    case sNMP_SYNTAX_GAUGE32:
    case sNMP_SYNTAX_TIMETICKS:
    {
        unsigned long val;
        vb.get_value(val);
        return (qulonglong)val;
    }
    case sNMP_SYNTAX_NULL:
    case sNMP_SYNTAX_NOSUCHOBJECT:
    case sNMP_SYNTAX_NOSUCHINSTANCE:
    case sNMP_SYNTAX_ENDOFMIBVIEW:
        break;
    case sNMP_SYNTAX_OID:
        break;
    case sNMP_SYNTAX_CNTR64:
    {
#if defined(Q_OS_WIN32)
        pp_uint64 val;
        vb.get_value(val);
        return val;
#elif defined(Q_OS_LINUX)
    /////====SNMP==
#endif
    }
    }
    return QVariant();
}

bool MibConvert::setVbValue(Vb &vb, const QVariant &var)
{
    switch (vb.get_syntax())
    {
        case sNMP_SYNTAX_INT:
            //    case sNMP_SYNTAX_INT32:
        {
            vb.set_value(var.toInt());
            return TRUE;
        }
        case sNMP_SYNTAX_OCTETS:
        case sNMP_SYNTAX_BITS:
        case sNMP_SYNTAX_OPAQUE:
        case sNMP_SYNTAX_IPADDR:
        {
            OctetStr octetstr(variant2String(var).toUtf8().constData());
            if ( octetstr.valid()) {
                vb.set_value( octetstr);
                return TRUE;
            } else {
                return FALSE;
            }
        }
    //    case sNMP_SYNTAX_UINT32:
        case sNMP_SYNTAX_CNTR32:
            //    case sNMP_SYNTAX_GAUGE32:
        case sNMP_SYNTAX_TIMETICKS:
        {
            Counter32 counter32(variant2float(var));
            if ( counter32.valid()) {
                vb.set_value( counter32);
                return TRUE;
            } else {
                return FALSE;
            }
        }
        case sNMP_SYNTAX_GAUGE32:
        {
            Gauge32 gauge32(variant2float(var));
            if ( gauge32.valid()) {
                vb.set_value( gauge32);
                return TRUE;
            } else {
                return FALSE;
            }
        }
        case sNMP_SYNTAX_NULL:
        case sNMP_SYNTAX_NOSUCHOBJECT:
        case sNMP_SYNTAX_NOSUCHINSTANCE:
        case sNMP_SYNTAX_ENDOFMIBVIEW:
            return false;
        case sNMP_SYNTAX_OID:
            return false;
        case sNMP_SYNTAX_CNTR64:
        {
            int i = variant2float(var);
            Counter64 counter64;
            counter64.set_low(i);
            counter64.set_high(i);
            if ( counter64.valid()) {
                vb.set_value( counter64);
                return TRUE;
            } else {
                return FALSE;
            }
        }
    }
    return FALSE;
}

/**
 * @brief MibConvert::address2OidType
 * 测试对应ip为哪种类型的mib文件。
 * @param ctarget CTarget设定
 * @return
 */
PduTypeItem MibConvert::address2OidType(const CTarget &ctarget)
{
    //静态map,包含已经遍历过的所有ip地址映射结果
    static QMap<QString,PduTypeItem> s_map;
    UdpAddress addr = ctarget.get_address().cast_udpaddress();
    PduTypeItem& pItem = s_map[addr.get_printable()];
    if(pItem.id>=0)
    {
        return pItem;
    }
    int status;
    Snmp snmp(status, 0, (addr.get_ip_version() == Address::version_ipv6));
    if ( status == SNMP_CLASS_SUCCESS)
    {
        QVector<PduTypeItem> items = DbPduTypeList::get()->allItems();
        if(items.isEmpty())
        {
            return pItem;
        }

        foreach(const PduTypeItem& item,items)
        {
            Oid oid(item.oid.toUtf8().constData());
            Vb vb(oid);//包中一个对应oid-value对。也是返回包参数
            Pdu pdu(&vb,1);

            status = snmp.get_next(pdu,ctarget);
            if(status == SNMP_CLASS_SUCCESS)
            {
                pdu.get_vb(vb,0);
                //若是为对应匹配的主oid.则继续下去。
                if(vb.get_oid().nCompare(oid.len(),oid)==0)
                {
                    pItem = item;
                    break;
                }
            }
        }
    }
    return pItem;
}

PduTypeItem MibConvert::address2OidType(const QString &ip)
{
    UdpAddress address(ip.toUtf8().constData());
    address.set_port(161);
    CTarget ctarget;
    ctarget.set_readcommunity("public");
    ctarget.set_writecommunity("private");
    ctarget.set_version( version2c);
    ctarget.set_retry( 1);
    ctarget.set_timeout(200);
    ctarget.set_address(address);
    return address2OidType(ctarget);
}
