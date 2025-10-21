#ifndef DEVSELECTWID_H
#define DEVSELECTWID_H

#include <QWidget>
#include "up_datapacket.h"

namespace Ui {
class Up_DevSelectWid;
}

class Up_DevSelectWid : public QWidget
{
    Q_OBJECT

public:
    explicit Up_DevSelectWid(QWidget *parent = 0);
    ~Up_DevSelectWid();

protected:
    bool checkInput();
    void setenabled(bool e);

private slots:
    void timeoutDone(void);
    void on_okBtn_clicked();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Up_DevSelectWid *ui;
    Up_sDataPacket *mData;
    QTimer *timer;
    int mCount;
};

#endif // DEVSELECTWID_H
