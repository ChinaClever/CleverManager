#ifndef DIALOGREMOVERACK_H
#define DIALOGREMOVERACK_H

#include <QDialog>

namespace Ui {
class Cab_RemoveRackDlg;
}

class Cab_RemoveRackDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Cab_RemoveRackDlg(QWidget *parent = 0);
    ~Cab_RemoveRackDlg();
    void setAddress(int i);
    int address();
private:
    Ui::Cab_RemoveRackDlg *ui;
};

#endif // DIALOGREMOVERACK_H
