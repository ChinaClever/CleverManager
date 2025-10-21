/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_roomelecquerydlg.h"
#include "ui_log_roomhrsquerydlg.h"

Log_RoomElecQueryDlg::Log_RoomElecQueryDlg(QWidget *parent) : Log_RoomHrsQueryDlg(parent)
{

}


QStringList Log_RoomElecQueryDlg::getRooms()
{
    QStringList list;

    QString room = ui->roomEdit->text();
    if(!room.isEmpty()) {
        list << QString(" and room like '%%1%'").arg(room);
    } else {
        QString item = mDateBar->getDate();
        QString cmd = QString(" where %1").arg(item);
        QStringList rets = DbRoomHrs::bulid()->listColumn("room", cmd);
        for(int i=0; i<rets.size(); i++) {
            list << QString(" and room like '%%1%'").arg(rets.at(i));
        }
    }

    return list;
}

QStringList Log_RoomElecQueryDlg::getCmds()
{
    QString cmd = mDateBar->getDate();
    QStringList list, rooms = getRooms();
    for(int i=0; i<rooms.size(); ++i) {
        list << cmd + rooms.at(i);
    }

    return list;
}


QString Log_RoomElecQueryDlg::getCmd()
{
    QStringList list = getCmds();
    DbRoomElec::bulid()->elec(list);

    return "";
}
