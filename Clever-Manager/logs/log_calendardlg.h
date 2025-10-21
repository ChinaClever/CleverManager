#ifndef LOG_CALENDARDLG_H
#define LOG_CALENDARDLG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class LOG_CalendarDlg;
}

class LOG_CalendarDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LOG_CalendarDlg(QWidget *parent = 0);
    ~LOG_CalendarDlg();

    QDate getDate() {return mDate;}

protected slots:
    void selectionChangedSLot();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LOG_CalendarDlg *ui;
    QDate mDate;
};

#endif // LOG_CALENDARDLG_H
