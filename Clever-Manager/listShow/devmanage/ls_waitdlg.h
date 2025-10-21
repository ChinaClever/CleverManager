#ifndef LS_WAITDLG_H
#define LS_WAITDLG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class LS_waitDlg;
}

class LS_waitDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LS_waitDlg(QWidget *parent = 0);
    ~LS_waitDlg();

public slots:
    void timeoutDone(void);

private:
    Ui::LS_waitDlg *ui;
    QTimer *timer;
    int counts;
};

#endif // LS_WAITDLG_H
