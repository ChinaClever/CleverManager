#ifndef DIALOGADDROOM_H
#define DIALOGADDROOM_H

#include <QDialog>

namespace Ui {
class DialogAddRoom;
}

class DialogAddRoom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddRoom(QWidget *parent = 0);
    ~DialogAddRoom();
    QString roomName();
public slots:
    void accept();
protected:
    Ui::DialogAddRoom *ui;
};

#endif // DIALOGADDROOM_H
