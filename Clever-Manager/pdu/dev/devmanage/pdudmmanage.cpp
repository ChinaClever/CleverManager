#include "pdudmmanage.h"
#include "pdu/data/hash/read/pduhashread.h"
#include "pdu/dev/devstore/pdudevdel.h"

/**
 * @brief 删除一个设备,  删除设备分二步，
 *          第一步：先在Hash中删除设备数据
 *          第二步：删除数据库数据
 * @param type 设备类型
 * @param ip  设备IP
 * @return
 */
bool pdu_dm_del(int type, const QString &ip)
{
    bool ret = true;

    PduHashIP *hashIP = pdu_hashData_getHash(type);
    if(hashIP)
    {
        ret = hashIP->del(ip); // 删除设备
        if(ret) {
            dl_del_add(ip);
        }
    }
    else
        ret = false;

    return ret;
}

/**
 * @brief 删除设备
 * @param ip
 * @return
 */
bool pdu_dm_delDev(QString &ip)
{
    int type = pdu_hd_getDevType(ip);
    return pdu_dm_del(type, ip);
}



/**
 * @brief 设备更改组名
 * @param ip
 * @param group
 * @return
 */
void pdu_dm_renameGroup(QString &ip, QString &group)
{
    PduDevHash *devHash = pdu_hd_get(ip);
    if(devHash)
    {
        QList<int> devList;
        devHash->list(devList);
        for(int i=0; i<devList.size(); ++i)
        {
            int num = devList.at(i);
            PduDataPacket *packet = devHash->get(num);

            packet->info->addr->group->set(group);
            dl_save_add(ip); // 修改数据库信息
        }
    }
}



