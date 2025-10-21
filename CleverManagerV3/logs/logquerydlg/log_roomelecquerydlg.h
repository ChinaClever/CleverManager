#ifndef LOG_ROOMELECQUERYDLG_H
#define LOG_ROOMELECQUERYDLG_H

#include "dbroomelec.h"
#include "log_roomhrsquerydlg.h"

namespace Ui {
class Log_RoomHrsQueryDlg;
}

class Log_RoomElecQueryDlg : public Log_RoomHrsQueryDlg
{
    Q_OBJECT
public:
    explicit Log_RoomElecQueryDlg(QWidget *parent = nullptr);

    QString getCmd();

protected:
    QStringList getRooms();
    QStringList getCmds();
};

#endif // LOG_ROOMELECQUERYDLG_H
