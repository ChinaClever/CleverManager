#ifndef TFTPWIDGET_H
#define TFTPWIDGET_H

#include <QWidget>
#include "tftpthread.h"

namespace Ui {
class TftpWidget;
}

class TftpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TftpWidget(QWidget *parent = 0);
    ~TftpWidget();

    void append(QStringList &);
    void setMax(int max);

signals:
    void quitSig();

protected slots:
    void sentSlot(int, QString ip); // 1开始，2发送成功，3发送失败
    void progressSlot(int); // 百分制
    void sentFinshSlot(int,int, int);
    void timeoutDone(void);

private slots:
    void on_openBtn_clicked();

    void on_updateBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::TftpWidget *ui;
    TftpThread *mTftpThread;
    int mProgress, mProMax;
    QTimer *timer;
};

#endif // TFTPWIDGET_H
