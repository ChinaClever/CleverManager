#ifndef UP_TCPDIALOG_H
#define UP_TCPDIALOG_H

#include <QDialog>
#include "up_tcpwidget.h"

namespace Ui {
class UP_TcpDialog;
}

class UP_TcpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UP_TcpDialog(QWidget *parent = 0);
    ~UP_TcpDialog();

    void append(QStringList &list){mTcpWidget->append(list);}

private:
    Ui::UP_TcpDialog *ui;

    UP_TcpWidget *mTcpWidget;
};

#endif // UP_TCPDIALOG_H
