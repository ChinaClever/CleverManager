#include "dialoginforequest.h"
#include "ui_dialoginforequest.h"
#include <QMessageBox>
#include "common.h"

DialogInfoRequest::DialogInfoRequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfoRequest)
{
    ui->setupUi(this);
}

DialogInfoRequest::DialogInfoRequest(QWidget *parent, const QString &title, const QString &text) :
    QDialog(parent),ui(new Ui::DialogInfoRequest)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->label_text->setText(text);
}

DialogInfoRequest::~DialogInfoRequest()
{
    delete ui;
}

void DialogInfoRequest::accept()
{
//    if(ui->widget_verify->isVerified())
        QDialog::accept();
//    else
//        QMessageBox::warning(this,tr("警告"),tr("用户账号和密码不匹配！"));
}
