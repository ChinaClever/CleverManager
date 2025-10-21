#ifndef DIALOGREMOVERACK_H
#define DIALOGREMOVERACK_H

#include <QDialog>

namespace Ui {
class DialogRemoveRack;
}

class DialogRemoveRack : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRemoveRack(QWidget *parent = 0);
    ~DialogRemoveRack();
    void setAddress(int i);
    int address();
private:
    Ui::DialogRemoveRack *ui;
};

#endif // DIALOGREMOVERACK_H
