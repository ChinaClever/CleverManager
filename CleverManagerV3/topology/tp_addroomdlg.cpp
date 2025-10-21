/*
 * dialogaddroom.cpp
 *  增加机房窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tp_addroomdlg.h"
#include "ui_tp_addroomdlg.h"
#include <QMessageBox>
#include <QListWidget>
#include "common.h"
#include "dp_basicthread.h"

Tp_AddRoomDlg::Tp_AddRoomDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tp_AddRoomDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint |Qt::WindowCloseButtonHint); //只有关闭按钮
}

Tp_AddRoomDlg::~Tp_AddRoomDlg()
{
    delete ui;
}

int Tp_AddRoomDlg::room(QString &name)
{
    name = ui->lineEdit_name->text();

    return save(name);
}

bool Tp_AddRoomDlg::inputCheck()
{
    bool ret = false;

    QString name = ui->lineEdit_name->text();
    if(name.isEmpty()){
        WaringMsgBox box(this,tr("请填写机房名字！"));
        box.Exec();
    } else {
        int rtn = DbRoomList::get()->contains(name);
        if(rtn > 0) {
            WaringMsgBox box(this,tr("该机房已经存在列表中！"));
            box.exec();
        } else {
            ret = true;
        }
    }

    return ret;
}

int Tp_AddRoomDlg::save(const QString &name)
{
    Db_Tran tran; // 数据库事物操作
    RoomItem item;
    item.room = name;
    DbRoomList::get()->insertItem(item);

    /*增加日志信息*/
    sUserLogItem log;
    log.remarks = tr("机房管理：");
    log.remarks += tr("添加机房:%1").arg(item.room);
    DbUserLog::bulid()->insertItem(log);

    return item.id;
}

void Tp_AddRoomDlg::on_okBtn_clicked()
{
    bool ret = inputCheck();
    if(ret) {
        accept();
    }
}

Tp_ModifyRoomDlg::Tp_ModifyRoomDlg(QWidget *parent) :
    Tp_AddRoomDlg(parent)
{

}

void Tp_ModifyRoomDlg::init(RoomItem &it)
{
    ui->lineEdit_name->setText(it.room);
    item = it;
}

int Tp_ModifyRoomDlg::save(const QString &name)
{
    Db_Tran tran; // 数据库事物操作
    item.room = name;
    DbRoomList::get()->updateItem(item);

    /*增加日志信息*/
    sUserLogItem log;
    log.remarks = tr("机房管理：");
    log.remarks += tr("修改机房:%1").arg(item.room);
    DbUserLog::bulid()->insertItem(log);

    return item.id;
}
