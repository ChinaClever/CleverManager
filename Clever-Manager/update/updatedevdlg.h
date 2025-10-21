#ifndef UPDATEDEVDLG_H
#define UPDATEDEVDLG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class UpdateDevDlg;
}

class UpdateDevDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDevDlg(QWidget *parent = 0);
    ~UpdateDevDlg();
    void setDevIp(QString &ip, int type);

protected:
    void getDevList(QStringList &);
    void updateDev();
    void updateIpPDU(int timeOut);
    void updateRPDU();

private slots:
    void on_okBtn_clicked();

private:
    Ui::UpdateDevDlg *ui;
    int mDevType;
};

#endif // UPDATEDEVDLG_H
