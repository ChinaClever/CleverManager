/*
 * newusrdlg.cpp
 *  用户增加界面
 *  新用户信息增加
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "newusrdlg.h"
#include "ui_newusrdlg.h"
#include "common.h"
#include "sql/records/db_sqlquery.h"

NewUsrDlg::NewUsrDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewUsrDlg)
{
    ui->setupUi(this);

    groupBox_background_icon(this);
    button_style_sheet(this);/*设置按键样式*/

    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->TelephonelineEdit);
    ui->TelephonelineEdit->setValidator( validator );
}

NewUsrDlg::~NewUsrDlg()
{
    delete ui;
}

/**
 * @brief 修改标题
 * @param str
 */
void NewUsrDlg::editTitle(QString &str)
{
    ui->NamelineEdit->setEnabled(false);
    ui->Titlelabel->setText(str);
    this->setWindowTitle(str);

    checkUsr();
}

/**
 * @brief 编辑时，如果只有一个账户，这个账户只能是管理员
 */
void NewUsrDlg::checkUsr(void)
{
    UsrClass c_usr;
    int ret = c_usr.getCount();
    if(ret <= 1)
    {
        ui->authcomboBox->setCurrentIndex(1);
        ui->authcomboBox->setDisabled(true);
    }
}


/**
 * @brief 用户信息加载
 * @param usr
 */
void NewUsrDlg::loadUsrInfo(UsrStruct *usr)
{
    ui->NamelineEdit->setText(usr->name);
    ui->PwdlineEdit->setText(usr->pwd);
    ui->pwd2lineEdit->setText(usr->pwd);
    ui->authcomboBox->setCurrentIndex(usr->jurisdiction);
    ui->TelephonelineEdit->setText(usr->telephone);
    ui->EmaillineEdit->setText(usr->email);
    ui->RemarkstextEdit->setText(usr->remarks);
}


/**
 * @brief 输入信息检查
 * @return
 */
bool NewUsrDlg::informationCheck(void)
{
    QString str;

    str = ui->NamelineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL, tr("账号不能为空"));
        return false;
    }

    str = ui->PwdlineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL,  tr("密码不能为空"));
        return false;
    }

    if(str != ui->pwd2lineEdit->text())
    {
        InfoMsgBox(NULL,  tr("密码两次输入不一致"));
        return false;
    }

    str = ui->EmaillineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL,  tr("请输入Email地址"));
        return false;
    }
    if(!str.contains("@"))
    {
        InfoMsgBox(NULL,  tr("请输入合法Email地址"));
        return false;
    }

    str = ui->TelephonelineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL, tr("请输入电话"));
        return false;
    }

   return true;
}


bool NewUsrDlg::saveUsrInfo(UsrStruct *s_temp)
{
    UsrClass *c_usr = new UsrClass;
    bool ret = c_usr->addUsr(s_temp);
    if(ret) {
        /*增加日志信息*/
        sLandLog log;
        log.title = tr("新增");
        log.operation = tr("新增用户");
        log.msg = tr("新增用户:%1").arg(s_temp->name);
        sql_insert_land(log);
    }
    delete c_usr;

    return ret;
}

void NewUsrDlg::on_saveBtn_clicked()
{
    if(informationCheck())
    {
        UsrStruct *s_temp = new UsrStruct;

        s_temp->name = ui->NamelineEdit->text();
        s_temp->pwd = ui->PwdlineEdit->text();
        s_temp->jurisdiction = ui->authcomboBox->currentIndex();
        s_temp->telephone = ui->TelephonelineEdit->text();
        s_temp->email = ui->EmaillineEdit->text();
        s_temp->remarks = ui->RemarkstextEdit->toPlainText();

        if(saveUsrInfo(s_temp))
            accept();
        else
            InfoMsgBox(NULL,  tr("账号已存在"));

    }
}
