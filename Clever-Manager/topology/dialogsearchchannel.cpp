#include "dialogsearchchannel.h"
#include "ui_dialogsearchchannel.h"
#include "common/signaldispatch.h"

dialogsearchchannel::dialogsearchchannel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogsearchchannel)
{
    ui->setupUi(this);

    this->resize(250, 150);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowIcon(QIcon(":/images/logo.jpg"));
    this->setWindowTitle(QString::fromLocal8Bit("通道搜索"));

    ui->title_label->setGeometry(50, 10, 160, 25);
    ui->title_label->setText(QString::fromLocal8Bit("请输入要查询的通道名称或IP"));

    ui->ip_label->setGeometry(40, 50, 30, 25);
    ui->ip_label->setText("IP :");

    ui->ip_lineEdit->setGeometry(75, 50, 135, 25);

    ui->ok_pushButton->setGeometry(65, 95, 50, 25);
    ui->cancle_pushButton->setGeometry(135, 95, 50, 25);
}

dialogsearchchannel::~dialogsearchchannel()
{
    delete ui;
}

void dialogsearchchannel::on_ok_pushButton_clicked()
{
    SignalDispatch::get()->get_search_channel_key(ui->ip_lineEdit->text());

    this->close();

    return ;
}

void dialogsearchchannel::on_cancle_pushButton_clicked()
{
    this->close();

    return ;
}
