#ifndef PDUDEVLISTSAVE_H
#define PDUDEVLISTSAVE_H
#include "pdudevlistread.h"

typedef struct
{
    int type; // 设备种类
    QString IP; // 设备IP
}dl_saveStruct;

void dl_save_add(QString &ip);
int dl_save_devData(void);

#endif // PDUDEVLISTSAVE_H
