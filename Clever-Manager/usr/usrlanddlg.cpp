/*
 * usrlanddlg.cpp
 *  登录
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "usrlanddlg.h"
#include "ui_usrlanddlg.h"
#include <QApplication>

bool usr_landFlag = false;
// bool is_close=false;
UsrStruct *usr_landstruct = NULL;
int versionumber;

/**
 * @brief usr_land_update
 */
static void usr_land_update(void)
{
    UsrMaptype *usrMap = &g_usrMap;
    QString name = usr_landstruct->name;

    bool ret = usrMap->contains(name);
    if(ret)
        usr_landstruct = usrMap->find(name).value();
    else
    {
        usr_landFlag = false;

        UsrStruct *usr = new UsrStruct();
        usr->jurisdiction = false;
        usr_landstruct = usr;
    }
}

/**
 * @brief 获取登录用户名
 * @return
 */
QString usr_land_name(void)
{
    usr_land_update();
    return usr_landstruct->name;
}

/**
 * @brief 获取用户名手机号
 * @return
 */
QString usr_land_telephone(void)
{
    usr_land_update();
    return usr_landstruct->telephone;
}

QString usr_land_email(void)
{
    usr_land_update();
    return usr_landstruct->email;
}

/**
 * @brief 获取登录用户的权限
 * @return
 */
bool usr_land_jur(void)
{
    bool ret = usr_landFlag;
    if(ret) {
        usr_land_update();
        ret = usr_landstruct->jurisdiction;
    }
    return ret;
}

UsrLandDlg::UsrLandDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsrLandDlg)
{

    ui->setupUi(this);
    this->setWindowTitle(tr("用户登录"));

    setWindowIcon(QIcon(":/images/logo.jpg"));
    m_usrMap = &g_usrMap;
    QPixmap map1(":/images/usr_land.jpg");
    QPixmap map=map1.scaled(800,600);
    QPalette palette;
    //    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/usr_land.jpg")));
    palette.setBrush(QPalette::Background, QBrush(map));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    setWindowFlags(Qt::FramelessWindowHint);
    //ui->label_3->setText(tr("PDU Manager"));//中性
//    ui->label_3->setText(tr("OLABS Manager"));//中性
//    ui->label_4->hide();//中性
//    ui->label_5->hide();//中性

    ui->landBtn->setFlat(true);
    QFont font=ui->label_5->font();
    font.setLetterSpacing(QFont::AbsoluteSpacing,100);
    //    font.setWordSpacing(10);
    //  setStyleSheet("QPushButton:hover{background-color:white; color: black;}"
    //                        "QPushButton:pressed{background-color:rgb(85, 170, 255);}");
}

UsrLandDlg::~UsrLandDlg()
{
    delete ui;
}

/**
 * @brief 执行登录窗口
 * @return
 */
bool UsrLandDlg::landWidget(void)
{
    this->exec();

    return usr_landFlag;
}

/**
 * @brief 选择工作模式
 * @return
 */
int UsrLandDlg::selectWork(void)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("操作选择"));
    //    com_setBackColour(tr("信息提示"),&msgBox);
    msgBox.setText(tr("信息提示"));
    msgBox.setText("\n" + tr("您是想切换用户，还是想退出？") + "\n");
    msgBox.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel | QMessageBox::Open);
    msgBox.setButtonText (QMessageBox::Ok,QString(tr("切换用户")));
    msgBox.setButtonText (QMessageBox::Open,QString(tr("退出登录")));
    msgBox.setButtonText (QMessageBox::Cancel,QString(tr("取 消")));
    msgBox.setDefaultButton(QMessageBox::Ok);
    int res = msgBox.exec();
    switch (res) {
    case QMessageBox::Ok:
        res = 1; /*切换用户*/
        break;
    case QMessageBox::Open:
        res = 2; /*退出登录*/
        break;
    case QMessageBox::Cancel:
        res = 0;
        break;
    default:
        res = 0;
        break;
    }

    return res;
}

/**
 * @brief 用户退出
 * @return
 */
bool UsrLandDlg::quitWidget(void)
{
    QuMsgBox box(this,tr("是否退出当前用户:%1").arg(usr_landstruct->name));
    bool ret = box.Exec();
    if(ret)
        usr_landFlag = false;

    return ret;
}

/**
 * @brief 用户登录验证
 */
void UsrLandDlg::usrLand(void)
{
    QString str;
    QString name = ui->nameLineEdit->text();
    if(!name.isEmpty())
    {
        bool ret = m_usrMap->contains(name);  //确认账号是否存在[实际存在json文件内]
        if(ret){
            QString pwd = ui->pwdLineEdit->text();
            if(!pwd.isEmpty()) {
                UsrStruct *usr = m_usrMap->find(name).value();  //该账号的结构体
                if(pwd == usr->pwd){ //确认密码是否正确
                    usr_landFlag = true;
                    usr_landstruct  = usr;
                    this->accept();
                }else str = tr("密码错误");
            }else str = tr("密码不能为空");
        }else str = tr("账号有误");
    }else str = tr("账号不能为空");
    ui->stateLabel->setText(str);
}

/**
 * @brief 登录响应函数
 */
void UsrLandDlg::on_landBtn_clicked()
{
    usrLand();
}

void UsrLandDlg::on_pushButton_clicked()
{
    if(usr_landFlag||usr_landstruct)//添加避免主程序已经运行，但是用户点击登录界面的X按钮杀死进程 pmd 2018-12-11
        this->close();
    else
        exit(0);
}


