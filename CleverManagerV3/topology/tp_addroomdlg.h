#ifndef DIALOGADDROOM_H
#define DIALOGADDROOM_H

#include <QDialog>
#include "dbroomlist.h"
#include "dbuserlog.h"

namespace Ui {
class Tp_AddRoomDlg;
}

class Tp_AddRoomDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Tp_AddRoomDlg(QWidget *parent = 0);
    ~Tp_AddRoomDlg();

    int room(QString &name);

protected:
    bool inputCheck();
    virtual int save(const QString &name);

private slots:
    void on_okBtn_clicked();

protected:
    Ui::Tp_AddRoomDlg *ui;
};

class Tp_ModifyRoomDlg : public Tp_AddRoomDlg
{
public:
    explicit Tp_ModifyRoomDlg(QWidget *parent = 0);
    void init(RoomItem &it);

protected:
     int save(const QString &name);

protected:
     RoomItem item;
};

#endif // DIALOGADDROOM_H
