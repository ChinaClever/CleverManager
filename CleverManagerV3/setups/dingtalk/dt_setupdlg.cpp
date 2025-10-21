#include "dt_setupdlg.h"
#include "ui_dt_setupdlg.h"
#include "msgbox.h"

DT_SetupDlg::DT_SetupDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DT_SetupDlg)
{
    ui->setupUi(this);
    initWid();
}

DT_SetupDlg::~DT_SetupDlg()
{
    delete ui;
}


void DT_SetupDlg::initWid()
{
    Dt_sCfg *cfg = DT_Cfg::bulid()->cfg;

    int en = 0;
    if(cfg->en) en = 1;
    ui->enBox->setCurrentIndex(en);
    ui->webhookEdit->setText(cfg->webhook);
    ui->keyEdit->setText(cfg->key);

    if(cfg->isAtAll) {
        ui->allRb->setChecked(true);
    } else {
        if(cfg->mobiles.size()) {
            ui->oneRb->setChecked(true);
            ui->mobEdit_1->setEnabled(true);
            ui->mobEdit_1->setText(cfg->mobiles.at(0));
        } else {
            ui->noRb->setChecked(true);
        }
    }
    button_style_sheet(ui->cancelBtn);
    button_style_sheet(ui->okBtn);
    button_style_sheet(ui->testBtn);
}

bool DT_SetupDlg::inputCheck()
{
    if(!ui->enBox->currentIndex()) return true;
    QString str = ui->webhookEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("Webhook地址不能为空"));
        return false;
    }

    str = ui->keyEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("关健字不能为空"));
        return false;
    }

    if(ui->oneRb->isChecked()) {
        str = ui->mobEdit_1->text();
        if(str.isEmpty()) {
            CriticalMsgBox box(this, tr("手机号不能为空"));
            return false;
        }
    }

    return true;
}

bool DT_SetupDlg::getCfg()
{
    bool ret = inputCheck();
    if(ret) {
        Dt_sCfg *cfg = DT_Cfg::bulid()->cfg;
        cfg->en = ui->enBox->currentIndex();
        cfg->webhook = ui->webhookEdit->text();
        cfg->key = ui->keyEdit->text();

        cfg->isAtAll = false;
        cfg->mobiles.clear();
        if(ui->allRb->isChecked()) {
            cfg->isAtAll = true;
        } else if(ui->oneRb->isChecked()) {
            cfg->mobiles << ui->mobEdit_1->text();
        }
    }

    return ret;
}


void DT_SetupDlg::on_oneRb_clicked(bool checked)
{
    ui->mobEdit_1->setEnabled(checked);
}

void DT_SetupDlg::on_okBtn_clicked()
{
    bool ret = getCfg();
    if(ret) {
        this->accept();
        InfoMsgBox(this, tr("设置成功!!"));
        DT_Cfg::bulid()->write();
    }
}

void DT_SetupDlg::on_testBtn_clicked()
{
    bool ret = getCfg();
    if(ret) {
        QString text = tr("这是一条测试信息!!!");
        ret = DT_DingTalk::bulid(this)->talk(text);
        if(ret) {
            InfoMsgBox(this, tr("测试信息发送成功!!"));
        } else {
            InfoMsgBox(this, tr("未启用钉钉推送功能!!"));
        }
    }
}

void DT_SetupDlg::on_cancelBtn_clicked()
{
    this->close();
}
