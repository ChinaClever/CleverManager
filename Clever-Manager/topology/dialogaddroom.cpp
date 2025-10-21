/*
 * dialogaddroom.cpp
 *  增加机房窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dialogaddroom.h"
#include "ui_dialogaddroom.h"
#include <QMessageBox>
#include <QListWidget>
#include "common.h"

DialogAddRoom::DialogAddRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddRoom)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint |Qt::WindowCloseButtonHint); //只有关闭按钮
}

DialogAddRoom::~DialogAddRoom()
{
    delete ui;
}

QString DialogAddRoom::roomName()
{
    return ui->lineEdit_name->text();
}

void DialogAddRoom::accept()
{
    WaringMsgBox *box=new WaringMsgBox(this,tr("请填写机房名字！"));
    if(ui->lineEdit_name->text().isEmpty()){
        box->Exec();    
    } else {
        QListWidget* listWidget = qobject_cast<QListWidget*>(this->parentWidget());
        if(listWidget==nullptr || listWidget->findItems(ui->lineEdit_name->text(),Qt::MatchExactly).isEmpty()){
            QDialog::accept();
        } else {
            WaringMsgBox *box1=new WaringMsgBox(this,tr("该机房已经存在列表中！"));
            box1->exec();
        }
    }
}
