#ifndef DIALOGEDITCABINETMAP_H
#define DIALOGEDITCABINETMAP_H

#include "tp_addroomdlg.h"

namespace Ui {
class Tp_RequestDlg;
}

class Tp_RequestDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Tp_RequestDlg(QWidget *parent = 0);
    explicit Tp_RequestDlg(QWidget *parent ,const QString &title,const QString& text);
    ~Tp_RequestDlg();

    void delRoom(int id, const QString &name);

private slots:
    void on_okBtn_clicked();

private:
    Ui::Tp_RequestDlg *ui;
};

#endif // DIALOGEDITCABINETMAP_H
