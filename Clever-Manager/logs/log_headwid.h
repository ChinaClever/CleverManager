#ifndef LOG_HEADWID_H
#define LOG_HEADWID_H

#include <QWidget>
#include "common.h"
#include "log_historywid.h"

namespace Ui {
class LOG_HeadWid;
}

class LOG_HeadWid : public QWidget
{
    Q_OBJECT

public:
    explicit LOG_HeadWid(QWidget *parent = 0);
    ~LOG_HeadWid();

signals:
    void selectSig(int);

private slots:
    void selectSlot(QString);

private:
    Ui::LOG_HeadWid *ui;
};

#endif // LOG_HEADWID_H
