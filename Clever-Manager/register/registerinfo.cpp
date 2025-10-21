#include "registerinfo.h"
#include "ui_registerinfo.h"
#include"registrykey.h"
#include<qstring.h>
#include<QDate>
#include<QPalette>
#include"activeprocess.h"
#include<QDateTime>
#include<QMessageBox>


RegisterInfo::RegisterInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterInfo)
{    
    ui->setupUi(this);

    RegistryKey key ;
    QDate date=key.readDate();
    ui->dateEdit->setDate(date);

    int i=key.readCount();
    QString strr1=QString::number(i,10); //数量
    QString strr2=key.readMachineCode(); //机器码
    QString strr3=key.readActiveCode(); //激活码

    ui->label_7->setText(strr1);
    ui->label_6->setText(strr2);
    ui->label_5->setText(strr3);
    groupBox_background_icon(this);
}

RegisterInfo::~RegisterInfo()
{
    delete ui;
}

/*
 * 注销
 */
void RegisterInfo::on_pushButton_clicked()
{
    act.clearinfo();
    InfoMsgBox(this,tr("注销成功"));
}

/*
 * 修改注册信息按钮
 */
void RegisterInfo::on_pushButton_2_clicked()
{
    ui->label_5->setReadOnly(false);
    ui->label_7->setReadOnly(false);
    ui->dateEdit->setReadOnly(false);
    ui->pushButton_2->setVisible(false);

}

/*
 * 修改注册信息
 */
void RegisterInfo::chande_info()
{
    is_late=false;
    is_numright=false;
    is_actcode=false;

    QDateTime current_dt=QDateTime::currentDateTime();
    QDate current_date=current_dt.date();
    edit_date=ui->dateEdit->date();

    edit_num=ui->label_7->text();

    edit_actcode=ui->label_5->text();

    if(!edit_date.toString().isEmpty() && !edit_num.isEmpty() && !edit_actcode.isEmpty())
    {
        is_actcode=true;

        if(edit_date.operator >(current_date))
            is_late=true;
        else
            QMessageBox::warning(this,tr("warning"),tr("时间错误，请重新选择时间!"),QMessageBox::Ok);

        if(is_late && act.is_number(ui->label_7->text()))
            is_numright=true;
        else if(is_late && !act.is_number(ui->label_7->text()))
            QMessageBox::warning(this,tr("warning"),tr("数量有误,请修改！"),QMessageBox::Ok);
    }
    else
        QMessageBox::warning(this,tr("warning"),tr("注册信息请填入完整！"),QMessageBox::Ok);
}

/*
 * 修改注册信息后的激活按钮
 */
void RegisterInfo::on_pushButton_3_clicked()
{
    chande_info(); //进行时间数量判断
    if(is_late && is_numright && is_actcode)
    {
        QString calc_activecode=active.createActiveCode(edit_date,edit_num.toInt(),ui->label_6->text());
        if(calc_activecode == edit_actcode)
        {
            InfoMsgBox(this,tr("恭喜你，注册信息修改成功"));

            register_info();

            ui->label_5->setReadOnly(true);
            ui->label_7->setReadOnly(true);
            ui->dateEdit->setReadOnly(true);
            ui->pushButton_2->setVisible(true);
        }
        else
            QMessageBox::warning(this,tr("warning"),tr("激活码错误，请检查！"),QMessageBox::Ok);
    }
}

/*
 * 将注册信息写入注册表
 */
void RegisterInfo::register_info()
{
    RegistryKey key;
    QString edit_codestr=ui->label_6->text();
    key.writeDate(edit_date);
    key.writeCount(edit_num.toInt());
    key.writeMachineCode(edit_codestr);
    key.writeActiveCode(edit_actcode);//激活码
}
