/*
 * updatedevdlg.cpp
 * PDU设备升级类
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "updatedevdlg.h"
#include "ui_updatedevdlg.h"
#include "pdu/data/hash/read/pduhashread.h"
#include "pdu/dev/devType/pdudtname.h"
#include "pdu/dev/devList/pdudevlist.h"
#include "tftpupdate/tftpdialog.h"
#include "tcpupdate/up_tcpdialog.h"


UpdateDevDlg::UpdateDevDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDevDlg)
{
    ui->setupUi(this);
    mDevType = 0;

    btnBlue_style_sheet(this);
    groupBox_background_icon(this);
    this->setWindowTitle(tr("升级设备信息"));
}

UpdateDevDlg::~UpdateDevDlg()
{
    delete ui;
}

void UpdateDevDlg::setDevIp(QString &ip, int type)
{
    PduDataPacket *dataPacket = pdu_hd_get(type, ip, 0);
    if(dataPacket)
    {
        if(dataPacket->offLine > 0)
        {
            ui->ipLab->setText(ip);
            mDevType = dataPacket->devType;
            QString devName;

            PduDTName dtName;
            dtName.getName(mDevType,devName);
            ui->typeLab->setText(devName);
        } else {
            ui->okBtn->setEnabled(false);
        }
    }
}

void UpdateDevDlg::getDevList(QStringList &listIP)
{
    if(ui->checkBox->isChecked()) {
        //        pdu_hashData_getHash(mDevType)->list(listIP);
        PduDevList devList;
        devList.getLine(mDevType, listIP);
    }
    else
        listIP << ui->ipLab->text();
}

/**
 * @brief IP-PDU产品升级
 */
void UpdateDevDlg::updateIpPDU(int timeOut)
{
    QStringList listIP;
    getDevList(listIP);

    TftpDialog dlg(this);
    dlg.append(listIP);
    dlg.setMax(timeOut);
    dlg.exec();
}


void UpdateDevDlg::updateRPDU()
{
    QStringList listIP;
    getDevList(listIP);

    UP_TcpDialog dlg(this);
    dlg.append(listIP);
    dlg.exec();
}


void UpdateDevDlg::updateDev()
{
    switch (mDevType)
    {
    case PDU_TYPE_IP_PDU:
        updateIpPDU(150); // 升级一个IPPDU需要１.5秒
        break;

    case PDU_TYPE_M_PDU:
        updateIpPDU(500);// 升级一个MPDU需要5秒
        break;

    case PDU_TYPE_NPM_PDU:
    case PDU_TYPE_RPDU:
    case PDU_TYPE_ZPDU:
        updateRPDU();
        break;
    default:
        break;
    }
}

void UpdateDevDlg::on_okBtn_clicked()
{
    updateDev();
}
