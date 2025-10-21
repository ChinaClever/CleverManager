#ifndef DT_CFG_H
#define DT_CFG_H
#include "configbase.h"

struct Dt_sCfg
{
    bool en;
    QString webhook;
    QString key;
    QString content;
    bool isAtAll;
    QStringList mobiles;
};

class DT_Cfg
{
    DT_Cfg();
public:
     static DT_Cfg *bulid();
     Dt_sCfg *cfg;

     void read();
     void write();

protected:
     virtual QString getPrefix(){return "dt";}
};

#endif // DT_CFG_H
