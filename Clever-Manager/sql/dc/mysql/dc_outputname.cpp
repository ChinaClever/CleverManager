/*
 * dc_outputname.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_outputname.h"

DC_OutputName::DC_OutputName()
{

}

DC_OutputName *DC_OutputName::bulid()
{
    static DC_OutputName* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_OutputName();
    return sington;
}

bool DC_OutputName::insertItem(const DC_OutputNameItem& item)
{
    QString cmd = "insert into %1 (pdu_devices_id, output_bit,name) "
                  "values(:pdu_devices_id, :output_bit,:name)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_OutputName::updateItem(const DC_OutputNameItem& item)
{
    QString cmd = "update %1 set "
                  "pdu_devices_id       = :pdu_devices_id,"
                  "output_bit           = :output_bit,"
                  "name                 = :name,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_OutputName::modifyItem(const DC_OutputNameItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":pdu_devices_id",item.pdu_devices_id);
        query.bindValue(":output_bit",item.output_bit);
        query.bindValue(":name",item.name);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_OutputName::selectItem(QSqlQuery &query, DC_OutputNameItem &item)
{
    item.id = query.value("id").toInt();
    item.pdu_devices_id = query.value("pdu_devices_id").toInt();

    item.output_bit = query.value("output_bit").toInt();
    item.name = query.value("name").toString();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

/**
 * @brief 输出位的个数
 * @param dev_id 设备索引号
 * @return
 */
int DC_OutputName::countOutput(int dev_id)
{
    int ret = count("output_bit", QString("where pdu_devices_id = %1 ").arg(dev_id));
    return ret;
}

/**
 * @brief 输出位是否存在
 * @param dev_id 设备索引号
 * @param bit 输出位
 * @return
 */
bool DC_OutputName::containOutput(int dev_id,int bit)
{
    int ret = count("id", QString("where pdu_devices_id = %1 and output_bit = %2").arg(dev_id).arg(bit));
    return (ret > 0) ? true:false;
}

QVector<int> DC_OutputName::listOutput(int dev_id)
{
     return listColumnToInt("output_bit", QString("where pdu_devices_id = %1").arg(dev_id));
}

/**
 * @brief 获取输出位的名称
 * @param dev_id 设备索引号
 * @param bit 输出位
 * @return
 */
QString DC_OutputName::getOutputName(int dev_id,int bit)
{
    QStringList list = listColumn("name",QString("where pdu_devices_id = %1 and output_bit = %2").arg(dev_id).arg(bit));
    return list.first();
}

/**
 * @brief 设置输出位的名称
 * @param dev_id 设备索引号
 * @param bit 输出位
 * @param name 输出位的名称
 * @return
 */
bool DC_OutputName::setOutputName(int dev_id,int bit,const QString& name)
{
    return updateColumn("name",name,QString("where pdu_devices_id = %1 and output_bit = %2").arg(dev_id).arg(bit));
}




