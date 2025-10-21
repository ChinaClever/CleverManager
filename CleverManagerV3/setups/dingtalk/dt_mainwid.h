#ifndef DT_MAINWID_H
#define DT_MAINWID_H

#include <QWidget>
#include "dt_setupdlg.h"

namespace Ui {
class DT_MainWid;
}

class DT_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit DT_MainWid(QWidget *parent = nullptr);
    ~DT_MainWid();

protected:
    void initWid();

private slots:
    void on_btn_clicked();

private:
    Ui::DT_MainWid *ui;
};

#endif // DT_MAINWID_H
