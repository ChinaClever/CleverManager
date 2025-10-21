/*
 * clouduseractions.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "clouduseractions.h"
#include "cloudsetparam.h"

CloudUserActions::CloudUserActions()
{

}


CloudUserActions *CloudUserActions::bulid()
{
    static CloudUserActions* sington = nullptr;
    if(sington == nullptr)
        sington = new CloudUserActions();
    return sington;
}

void CloudUserActions::slaveLog(QString &name, QString &op, QString &msg)
{
    int rid = CloudSetParam::bulid()->module.id;
    if(rid>0) {
        DC_UserActionsItem item;
        item.modules_id = rid;
        item.name = name;
        item.operation = op;
        item.content = msg;

#if defined(CLOUD_MODULE)
        DC_UserActions::bulid()->insertItem(item);
#endif
    }

}
