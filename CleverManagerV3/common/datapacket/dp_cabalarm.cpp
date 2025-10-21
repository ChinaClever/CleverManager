/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dp_cabalarm.h"
#include "sound.h"
#include "dt_dingtalk.h"

Dp_CabAlarm::Dp_CabAlarm(QObject *parent) : QObject(parent)
{

}


Dp_CabAlarm *Dp_CabAlarm::bulid(QObject *parent)
{
    static Dp_CabAlarm* sington = NULL;
    if(sington == NULL) {
        sington = new Dp_CabAlarm(parent);
    }
    return sington;
}

void Dp_CabAlarm::alarm(sCabPacket *cab)
{
    if(cab->tg.pow > cab->pow) {
        if(cab->powAlarm == 0) {
            cab->powAlarm = 1;

            sDbCabAlarmItem item;
            item.room = cab->room;
            item.cab = cab->cab;
            item.modular = cab->modular;
            item.item = tr("功率过大");
            item.msg = tr("机柜最大功率 %1kW 实际功率 %2kW").arg(cab->pow/COM_RATE_POW).arg(cab->tg.pow/COM_RATE_POW);

            QString str = tr("机房：")+item.room+tr(" 机柜:")+item.cab+item.item+item.msg;
            DT_DingTalk::bulid(this)->talk(str);

            DbCabAlarm::bulid()->insertItem(item);
            Sound::bulid()->play();
        }
    } else {
         cab->powAlarm = 0;
    }
}
