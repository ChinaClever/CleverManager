/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "navbarwid.h"
#include "ui_navbarwid.h"

NavBarWid::NavBarWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavBarWid)
{
    ui->setupUi(this);
    QGridLayout *gridLayout = new QGridLayout(parent);//控制ToolBox自适应
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
    initBtnColour();
    on_tpBtn_clicked();
    initBtnQStr();
    initLabText();
    //set_background_icon(this,":/image/title_back.jpg");
}

NavBarWid::~NavBarWid()
{
    delete ui;
}

/**
 * @brief 主界面按键使能、锁定
 * @param en
 */
void NavBarWid::enableBtn()
{
    bool en = LandingUser::get()->land;
    ui->setupBtn->setEnabled(en);
    ui->logBtn->setEnabled(en);
}


/**
 * @brief 用户登录
 */
void NavBarWid::on_userBtn_clicked()
{
    if(mUserLand)
    {
        bool lang = LandingUser::get()->land;
        if(lang) {
            int ret = mUserLand->selectWork();
            if(ret == 1) // 用户切换
            {
                mUserLand->exec();
            }
            else if(ret == 2) // 用户退出
                mUserLand->quitWidget();
        } else {
            mUserLand->exec();
        }
    }

    enableBtn();
}

void NavBarWid::on_setupBtn_clicked()
{
    setBtnClicked(ui->setupBtn, "ser");
    emit navBarSig(4);
}

void NavBarWid::on_logBtn_clicked()
{
    setBtnClicked(ui->logBtn, "log");
    emit navBarSig(1);
}

void NavBarWid::on_pduBtn_clicked()
{
    setBtnClicked(ui->pduBtn, "dev");
    emit navBarSig(2);
}

void NavBarWid::on_roomBtn_clicked()
{
    setBtnClicked(ui->roomBtn, "tg");
    emit navBarSig(3);
}

void NavBarWid::on_tpBtn_clicked()
{
    setBtnClicked(ui->tpBtn, "topo");
    emit navBarSig(0);
}

void NavBarWid::on_cabBtn_clicked()
{
    setBtnClicked(ui->cabBtn, "detail");
    emit navBarSig(5);
}

void NavBarWid::setBtndefault(QPushButton *btn, const QString &str)
{
    QString strNormal = tr("QPushButton{border-image: url(:/toolbutton/toolbutton/%1_normal.png);}").arg(str);
    QString strSelect = tr("QPushButton:hover{border-image: url(:/toolbutton/toolbutton/%1_select.png);}").arg(str);
    QString strClicked = tr("QPushButton:pressed{border-image: url(:/toolbutton/toolbutton/%1_clicked.png);}").arg(str);

    btn->setStyleSheet(strNormal + strSelect + strClicked);
    btn->setText("");
}

/**
 * @brief 初始化按键颜色
 */
void NavBarWid::initBtnColour()
{
    setBtndefault(ui->tpBtn, "topo");
    setBtndefault(ui->roomBtn, "tg");
    setBtndefault(ui->cabBtn, "detail");
    setBtndefault(ui->pduBtn, "dev");
    setBtndefault(ui->setupBtn, "ser");
    setBtndefault(ui->logBtn, "log");
    setBtndefault(ui->userBtn, "usr"); 
}

void NavBarWid::setBtnClicked(QPushButton *btn, const QString &str)
{
    initBtnColour();
    QString strNormal = tr("QPushButton{border-image: url(:/toolbutton/toolbutton/%1_clicked.png);}").arg(str);
    QString strSelect = tr("QPushButton:hover{border-image: url(:/toolbutton/toolbutton/%1_select.png);}").arg(str);
    QString strClicked = tr("QPushButton:pressed{border-image: url(:/toolbutton/toolbutton/%1_clicked.png);}").arg(str);

    btn->setStyleSheet(strNormal + strSelect + strClicked);
}

void NavBarWid::initLabText()
{
    QLabel* lab = NULL;
    QMapIterator<QPushButton*, QString> i(mBtnQStr);
    while(i.hasNext()){
        i.next();
        lab = new QLabel(i.key());
        if(i.key() == ui->userBtn)
        {
            mUserLab = lab;
            mUserLab->setText(mUserName);
        }
        else
            lab->setText(i.value());
        lab->setStyleSheet("font:14px \"微软雅黑\";color: rgb(255, 255, 255);background-color: transparent;");
        lab->move(i.key()->width()/2-27,i.key()->height()/2+13);
    }
}

void NavBarWid::changeToCabSlot()
{
    setBtnClicked(ui->cabBtn, "detail");
}

void NavBarWid::initBtnQStr()
{
    QString str[7] = {tr("机房拓扑"),tr("机房统计"),tr("机柜列表"),tr("设备管理"),tr("服务设置"),tr("日志记录"),tr("用户登陆")};
    QPushButton* btn[7] = {ui->tpBtn,ui->roomBtn,ui->cabBtn,ui->pduBtn,ui->setupBtn,ui->logBtn,ui->userBtn};
    for(int i=0 ; i<7; i++ )
        mBtnQStr.insert(btn[i],str[i]);
}

void NavBarWid::recvUserNameSlot(QString str)
{
    mUserName = str;
    if(mUserLab)
        mUserLab->setText(mUserName);
}
