#ifndef DBTEMPLATEELEC_H
#define DBTEMPLATEELEC_H
#include <QtCore>
#include "configbase.h"

template <typename T1,typename T2,typename T3>
bool elec_start_get(T1* db, T2 &it, T3 &ele, const QString &cmd)
{
    bool ret = true;
    int id = db->minId(cmd);
    if(id> 0) {
        it = db->findById(id);
        ele.startdt = it.date + " " + it.time;
        ele.startele = it.ele;
    } else {
        ret = false;
    }

    return ret;
}


template <typename T1,typename T2,typename T3>
bool elec_end_get(T1* db, T2 &it, T3 &ele, const QString &cmd)
{
    bool ret = true;
    int id = db->maxId(cmd);
    if(id> 0) {
        it = db->findById(id);
        ele.enddt = it.date + " " + it.time;
        ele.endele = it.ele;
    } else {
        ret = false;
    }

    return ret;
}

template <typename T3>
bool elec_fees_get(T3 &ele)
{
    bool ret = true;

    if(ele.endele >= ele.startele) {
        sConfigItem *item = ConfigBase::bulid()->item;
        ele.price = item->elePrice;
        ele.ele = ele.endele - ele.startele;
        ele.fees = ele.ele * ele.price;
    } else {
        return ret = false;
    }

    return ret;
}

template <typename T1,typename T2,typename T3>
bool elec_bulid_get(T1* db, T2 &it, T3 &ele, const QString &cmd)
{
    bool ret = elec_start_get(db, it, ele, cmd);
    if(ret) {
        ret = elec_end_get(db, it, ele, cmd);
        if(ret) {
            ret = elec_fees_get(ele);
        }
    }

    return ret;
}


#endif // DBTEMPLATEELEC_H
