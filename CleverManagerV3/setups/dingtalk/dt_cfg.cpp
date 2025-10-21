#include "dt_cfg.h"

DT_Cfg::DT_Cfg()
{
    cfg = new Dt_sCfg();
    read();
}

DT_Cfg *DT_Cfg::bulid()
{
    static DT_Cfg* sington = nullptr;
    if(sington == nullptr)
        sington = new DT_Cfg();
    return sington;
}

void DT_Cfg::read()
{
    QString prefix = getPrefix();
    QString str = QString("%1_webhook").arg(prefix);
    cfg->webhook = com_cfg_readStr(str, prefix);
    if(cfg->webhook.isEmpty()) {
        QString url = "https://oapi.dingtalk.com/robot/send?access_token=56ff0885f3b0c5182bd59f402d702ce778addb3acc3060fcf3a0110d6bb7a880";
        cfg->webhook = url;
    }

    str = QString("%1_key").arg(prefix);
    cfg->key = com_cfg_readStr(str, prefix);
    if(cfg->key.isEmpty()) {
        cfg->key = QObject::tr("报警提醒");
    }

    str = QString("%1_mob").arg(prefix);
    str = com_cfg_readStr(str, prefix);
    if(str.isEmpty()) {
        cfg->mobiles << "18576689472";
    } else {
        cfg->mobiles << str;
    }

    str = QString("%1_en").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    cfg->en = ret;

    str = QString("%1_isatall").arg(prefix);
    ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    cfg->isAtAll = ret;
}

void DT_Cfg::write()
{
    QString prefix = getPrefix();
    QString str = QString("%1_webhook").arg(prefix);
    com_cfg_writeParam(str, cfg->webhook, prefix);

    str = QString("%1_key").arg(prefix);
    com_cfg_writeParam(str, cfg->key, prefix);

    str = QString("%1_mob").arg(prefix);
    com_cfg_writeParam(str, cfg->mobiles.at(0), prefix);

    int rtn = 0;
    if(cfg->en) rtn = 1;
    str = QString("%1_en").arg(prefix);
    com_cfg_writeParam(str, QString::number(rtn), prefix);

    rtn = 0;
    if(cfg->isAtAll) rtn = 1;
    str = QString("%1_isatall").arg(prefix);
    com_cfg_writeParam(str, QString::number(rtn), prefix);
}


