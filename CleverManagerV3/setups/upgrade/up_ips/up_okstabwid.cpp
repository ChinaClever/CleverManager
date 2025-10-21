/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "up_okstabwid.h"

Up_OksTabWid::Up_OksTabWid(QWidget *parent) : Up_ErrsTabWid(parent)
{
    initWid();
}


void Up_OksTabWid::initWid()
{
    QString title = tr("升级成功列表");
    QStringList header;
    header <<title;

    initTableWid(header, 0, title);
}

void Up_OksTabWid::timeoutDone()
{
    updateData(mData->oks);
}
