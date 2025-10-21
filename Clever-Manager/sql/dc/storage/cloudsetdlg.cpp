/*
 * cloudsetdlg.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "cloudsetdlg.h"
#include "ui_cloudsetdlg.h"
#include "common.h"
#include "cloudroom.h"

CloudSetDlg::CloudSetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloudSetDlg)
{
    ui->setupUi(this);
    CloudRoom::bulid();
    QTimer::singleShot(100,this,SLOT(initFunSLot())); //延时初始化
}

CloudSetDlg::~CloudSetDlg()
{
    delete ui;
}

void CloudSetDlg::initFunSLot()
{
    mParam = CloudSetParam::bulid();
    ui->domainEdit->setText(mParam->company.domain);
    ui->nameEdit->setText(mParam->company.name);
    if(mParam->company.id>0) {
        ui->domainEdit->setEnabled(false);
        ui->domainBtn->setText(tr("修改"));
        ui->nameEdit->setEnabled(false);
        ui->nameBtn->setText(tr("修改"));
    }

    ui->idcEdit->setText(mParam->idc.name);
    if(mParam->idc.id>0) {
        ui->idcEdit->setEnabled(false);
        ui->idcBtn->setText(tr("修改"));
    }

    ui->moduleEdit->setText(mParam->module.name);
    if(mParam->module.id>0) {
        ui->moduleEdit->setEnabled(false);
        ui->moduleBtn->setText(tr("修改"));
    }
}


bool CloudSetDlg::checkDomain()
{
    bool ret = false;
    QString str = ui->domainEdit->text();
    if(!str.isEmpty()) {
        DC_Companys *company = DC_Companys::bulid();
        int id = company->getId(str);
        if(id>0) {  // 域名已经存在
            QuMsgBox box(this, tr("公司域名已经存在，是否修改?"));
            if(box.Exec()) {
                mParam->company = company->findById(id);
                ui->nameEdit->setText(mParam->company.name);
                ret = true;
            }
        } else { //
            QuMsgBox box(this, tr("是否添加新域名?"));
            if(box.Exec()) {
                mParam->company.domain = str;
                mParam->company.name = "";
                company->insertItem(mParam->company);
                mParam->company.id = company->getId(str);
                ret = true;
            }
        }
    } else {
        CriticalMsgBox box(this,tr("域名不能为空！"));
    }

    return ret;
}

/**
 * @brief 域名提交
 */
void CloudSetDlg::on_domainBtn_clicked()
{
    if(ui->domainBtn->text().contains(tr("提交"))) {
        bool ret = checkDomain();
        if(ret) {
            ui->domainEdit->setEnabled(false);
            ui->domainBtn->setText(tr("修改"));

            ui->nameEdit->setEnabled(true);
            ui->nameBtn->setText(tr("提交"));

            int id = mParam->company.id;
            mParam->idc.companys_id = id;
            mParam->idc.id = -1;
            ui->idcEdit->setEnabled(true);
            ui->idcBtn->setText(tr("提交"));

            mParam->module.id = -1;
            ui->moduleEdit->setEnabled(true);
            ui->moduleBtn->setText(tr("提交"));
        }
    } else {
        ui->domainEdit->setEnabled(true);
        ui->domainEdit->selectAll();
        ui->domainBtn->setText(tr("提交"));
    }
}

bool CloudSetDlg::checkName()
{
    bool ret = false;
    QString str = ui->nameEdit->text();
    if(!str.isEmpty()) {
        if(mParam->company.id>0) {
            mParam->company.name = str;
            DC_Companys *company = DC_Companys::bulid();
            company->updateItem(mParam->company);
            ret = true;
        } else {
            CriticalMsgBox box(this, tr("请先填写公司域名!"));
        }
    } else {
        CriticalMsgBox box(this,tr("公司名不能为空！"));
    }

     return ret;
}

void CloudSetDlg::on_nameBtn_clicked()
{
    if(ui->nameBtn->text().contains(tr("提交"))) {
        bool ret = checkName();
        if(ret) {
            ui->nameEdit->setEnabled(false);
            ui->nameBtn->setText(tr("修改"));
        }
    } else {
        ui->nameEdit->setEnabled(true);
        ui->nameEdit->selectAll();
        ui->nameBtn->setText(tr("提交"));
    }
}

bool CloudSetDlg::checkIdc()
{
    bool ret = false;
    QString str = ui->idcEdit->text();
    if(!str.isEmpty()) {
        if(mParam->company.id>0) {
            int rid = mParam->company.id;
            DC_Idcs *idc = DC_Idcs::bulid();
            if(idc->containName(rid, str)) {
                QuMsgBox box(this, tr("IDC名称已经存在，是否修改?"));
                if(box.Exec()) {
                    mParam->idc.name = str;
                    mParam->idc.id = idc->getId(rid, str);
                    ret = true;
                }
            } else {
                QuMsgBox box(this, tr("是否添加新IDC?"));
                if(box.Exec()) {
                    mParam->idc.companys_id = rid;
                    mParam->idc.name = str;
                    idc->insertItem(mParam->idc);
                    mParam->idc.id = idc->getId(rid, str);
                    ret = true;
                }
            }
        } else {
            CriticalMsgBox box(this, tr("请先填写公司信息!"));
        }
    } else {
        CriticalMsgBox box(this,tr("IDC名称不能为空！"));
    }

     return ret;
}

void CloudSetDlg::on_idcBtn_clicked()
{
    if(ui->idcBtn->text().contains(tr("提交"))) {
        bool ret = checkIdc();
        if(ret) {
            ui->idcEdit->setEnabled(false);
            ui->idcBtn->setText(tr("修改"));

            mParam->module.id = -1;
            ui->moduleEdit->setEnabled(true);
            ui->moduleBtn->setText(tr("提交"));
        }
    } else {
        ui->idcEdit->setEnabled(true);
        ui->idcEdit->selectAll();
        ui->idcBtn->setText(tr("提交"));
    }
}

bool CloudSetDlg::checkModule()
{
    bool ret = false;
    QString str = ui->moduleEdit->text();
    if(!str.isEmpty()) {
        if(mParam->idc.id>0) {
            int rid = mParam->idc.id;
            DC_Modules *module = DC_Modules::bulid();
            if(module->containName(rid, str)) {
                QuMsgBox box(this, tr("模块名已经存在，是否修改?"));
                if(box.Exec()) {
                    mParam->module.name = str;
                    mParam->module.id = module->getId(rid, str);
                    ret = true;
                }
            } else {
                QuMsgBox box(this, tr("是否添加新模块?"));
                if(box.Exec()) {
                    mParam->module.idcs_id = rid;
                    mParam->module.name = str;
                    module->insertItem(mParam->module);
                    mParam->module.id = module->getId(rid, str);
                    ret = true;
                }
            }
        } else {
            CriticalMsgBox box(this, tr("请先填写IDC信息!"));
        }
    } else {
        CriticalMsgBox box(this,tr("模块名称不能为空！"));
    }

     return ret;
}


void CloudSetDlg::on_moduleBtn_clicked()
{
    if(ui->moduleBtn->text().contains(tr("提交"))) {
        bool ret = checkModule();
        if(ret) {
            ui->moduleEdit->setEnabled(false);
            ui->moduleBtn->setText(tr("修改"));
        }
    } else {
        ui->moduleEdit->setEnabled(true);
        ui->moduleEdit->selectAll();
        ui->moduleBtn->setText(tr("提交"));
    }
}



void CloudSetDlg::on_saveBtn_clicked()
{
    if(mParam->company.id<0) {
        CriticalMsgBox box(this,tr("请填写公司信息！"));
        return;
    }

    if(mParam->company.name.isEmpty()) {
        CriticalMsgBox box(this,tr("请填写公司名！"));
        return;
    }

    if(mParam->idc.id<0) {
        CriticalMsgBox box(this,tr("请填写IDC信息！"));
        return;
    }

    if(mParam->module.id<0) {
        CriticalMsgBox box(this,tr("请填写模块信息！"));
        return;
    }

    mParam->saveAll();
    this->accept();
}
