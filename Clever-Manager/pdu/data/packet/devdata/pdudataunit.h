#ifndef PDUDATAUNIT_H
#define PDUDATAUNIT_H
#include "pdudatabase.h"

class PduDataUnit
{
public:
    PduDataUnit();
    ~PduDataUnit();

    int num;      // 数量
    PduDataBase *value; // 当前值

    PduDataBase *min; // 最小值
    PduDataBase *max; // 最大值
    PduDataBase *alarm; // 报警值
    PduDataBase *flag; // 标志 0表示未记录 1表示已记录

    PduDataBase *crMin; // 临界最小值
    PduDataBase *crMax; // 临界最大值
    PduDataBase *crAlarm; // 临界报警值
    PduDataBase *crFlag;
};

#endif // PDUDATAUNIT_H
