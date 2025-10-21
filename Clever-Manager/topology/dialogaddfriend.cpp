#include <QMessageBox>
#include "dialogaddfriend.h"
#include "ui_dialogaddfriend.h"
#include "zebra/zebra_client.h"
#include "common.h"


DialogAddFriend::DialogAddFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddFriend)
{
    ui->setupUi(this);
    setWindowTitle(tr("添加通道"));
    setWindowIcon(QIcon(":/images/logo.jpg"));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),SLOT(reject()));
}

DialogAddFriend::~DialogAddFriend()
{
    delete ui;
}

QString DialogAddFriend::get_invite_code()
{
    return ui->lineEdit_invite_code->text();
}

QString DialogAddFriend::get_ipaddr()
{
    return ui->lineEdit_ip_addr->text();
}

void DialogAddFriend::on_pushButton_ok_clicked()
{
    if(ui->lineEdit_invite_code->text().trimmed().isEmpty()){
        QMessageBox::warning(this,tr("提醒"),tr("邀请码为空！"));
        return;
    }
    QString ipAddr = ui->lineEdit_ip_addr->text().trimmed();
    if(ipAddr.isEmpty()){
        QMessageBox::warning(this,tr("提醒"),tr("IP地址为空！"));
        return;
    } else if(cm::isValidIpAddress(ipAddr)==false){
        QMessageBox::warning(this,tr("提醒"),tr("IP地址无效！"));
        return;
    }
    accept();
}
