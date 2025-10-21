#ifndef TFTPDIALOG_H
#define TFTPDIALOG_H

#include <QDialog>
#include "tftpwidget.h"

namespace Ui {
class TftpDialog;
}

class TftpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TftpDialog(QWidget *parent = 0);
    ~TftpDialog();

    void append(QStringList &list){mTftpWidget->append(list);}
    void setMax(int max){mTftpWidget->setMax(max);}

private:
    Ui::TftpDialog *ui;

    TftpWidget *mTftpWidget;
};

#endif // TFTPDIALOG_H
