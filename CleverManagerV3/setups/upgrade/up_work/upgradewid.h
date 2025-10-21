#ifndef TFTPWIDGET_H
#define TFTPWIDGET_H

#include "sqlexportdlg.h"
#include "tcpupgrade.h"
#include "httpupgrade.h"

namespace Ui {
class UpgradeWid;
}

class UpgradeWid : public QWidget
{
    Q_OBJECT

public:
    explicit UpgradeWid(QWidget *parent = 0);
    ~UpgradeWid();

protected:
    bool checkFile();

private slots:
    void timeoutDone();
    void on_openBtn_clicked();
    void on_updateBtn_clicked();
    void on_exportBtn_clicked();
    void on_breakBtn_clicked();

private:
    Ui::UpgradeWid *ui;
    Up_sDataPacket *mData;

    QTimer *timer;
    TcpUpgrade *mTcpThread;
    TftpUpgrade *mTftpThread;
    SqlExportDlg *mExportDlg;
    UpgradeThread *mUpgradeThread;
    HttpUpgrade *mHttpThread;
};

#endif // TFTPWIDGET_H
