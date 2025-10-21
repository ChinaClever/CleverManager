/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "up_errstabwid.h"

Up_ErrsTabWid::Up_ErrsTabWid(QWidget *parent) : Up_IpsTabWid(parent)
{
    initWid();
}

void Up_ErrsTabWid::initWid()
{
    QString title = tr("升级失败列表");
    QStringList header;
    header <<title;

    initTableWid(header, 0, title);
}


void Up_ErrsTabWid::timeoutDone()
{
    updateData(mData->errs);
}
