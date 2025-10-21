#ifndef UP_TCPWIDGET_H
#define UP_TCPWIDGET_H

#include <QWidget>
#include "up_tcpsentthread.h"

namespace Ui {
class UP_TcpWidget;
}

class UP_TcpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UP_TcpWidget(QWidget *parent = 0);
    ~UP_TcpWidget();

     void append(QStringList &ip){mTcpSent->append(ip);}

signals:
     void quitSig();

protected:
    bool checkFile(void);
    bool checkAcount(void);

private slots:
    void connectSlot(int);
    void sentSlot(int, QString &ip); // 1开始，2发送成功，3发送失败
    void progressSlot(int); // 百分制
    void sentFinshSlot(int,int, int);

    void on_openBtn_clicked();
    void on_updateBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::UP_TcpWidget *ui;
    UP_TcpSentThread *mTcpSent;

    sTcpUpdateStr mUpdateStr;
};

#endif // UP_TCPWIDGET_H
