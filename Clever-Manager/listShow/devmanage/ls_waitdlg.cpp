#include "ls_waitdlg.h"
#include "ui_ls_waitdlg.h"

LS_waitDlg::LS_waitDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LS_waitDlg)
{
    ui->setupUi(this);

    counts = 0;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

LS_waitDlg::~LS_waitDlg()
{
    delete ui;
}

void LS_waitDlg::timeoutDone(void)
{
    ui->progressBar->setValue(counts++);
    if(counts > 10)
        this->close();
}
