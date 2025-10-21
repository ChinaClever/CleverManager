#include "ipsort.h"
#include "common.h"

/**
   * @brief 设备编号算法
   * @param ip
   * @return
   */
static uint numMethod(QString &ip)
{
    uint num=0;
    bool ret = isIPaddress(ip);
    if(ret)
    {
        QStringList ipList = ip.mid(4).split(".");
        for(int i=0; i<ipList.size(); ++i)
        {
            num *= 100;
            num += ipList[i].toInt();
        }
    }

    return num;
}


/**
  * @brief ip地址排序
  * @param list
  * @return
  */
bool ip_sort_method(QStringList &list)
{
    QMap<uint,QString> sortMap;

    uint num;
    for(int i=0; i<list.count(); ++i)
    {
        num = numMethod(list[i]);
        if(num > 0)
            sortMap.insert(num,list[i]);
    }
    list.clear();

    QMap<uint,QString>::const_iterator it;
    for (it = sortMap.constBegin(); it != sortMap.constEnd(); ++it)
        list << it.value();

    return true;
}
