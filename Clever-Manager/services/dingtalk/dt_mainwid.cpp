#include "dt_mainwid.h"
#include "ui_dt_mainwid.h"
#include "common.h"
DT_MainWid::DT_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DT_MainWid)
{
    ui->setupUi(this);
    initWid();
}

DT_MainWid::~DT_MainWid()
{
    delete ui;
}

void DT_MainWid::initWid()
{
    DT_DingTalk::bulid(this);
    Dt_sCfg *cfg = DT_Cfg::bulid()->cfg;
    QString str = tr("关闭");
    if(cfg->en) {
        str = tr("开启");
    }
    ui->statusLab->setText(str);
}

void DT_MainWid::on_btn_clicked()
{
    DT_SetupDlg dlg(this);
    int ret = dlg.exec();
    if(QDialog::Accepted == ret) {
        initWid();
    }
}
