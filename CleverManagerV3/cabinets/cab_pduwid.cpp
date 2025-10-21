/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_pduwid.h"
#include "ui_cab_pduwid.h"
#include "common.h"

Cab_PduWid::Cab_PduWid(QWidget *parent) :
    QWidget(parent),ui(new Ui::Cab_PduWid)
{
    ui->setupUi(this);
    m_portWidget = new QWidget(this);
    connect(ui->switchBtn, SIGNAL(clicked()), this, SLOT(onSwitchBtnClick()));

    ui->switchBtn->hide(); //默认隐藏
    mPacket = NULL;

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(2500);
}

Cab_PduWid::~Cab_PduWid()
{
    delete ui;
}

/**
 * @brief WidgetPduDevice::setAlignType
 * 设置按钮靠左靠右。显示
 * @param left
 */
void Cab_PduWid::setAlignType(bool left)
{
    QLayout* layout = ui->widget_ports->layout();
    if(layout) layout->deleteLater();
    m_bLeft = left;
    QVBoxLayout* vLayout = new QVBoxLayout(ui->widget_ports);
    vLayout->addWidget(m_portWidget);
    vLayout->addStretch(1);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(1);
}

void Cab_PduWid::timeoutDone()
{
    if(mPacket) {
        updateLine(mPacket);
        updateOutput(mPacket);
        setIpAddr(mPacket->net.ip, mPacket->dev_num);
    }
}

void Cab_PduWid::updateLine(sDataPacket *packet)
{
    sObjData *line = &(packet->data.line);
    if(line->size > 3){
        ui->switchBtn->show();
    }else{
        ui->switchBtn->hide();
    }

    setZeroOffine();
    if(packet->offLine > 0) {
        QLabel *lns[] = {ui->label_l1n, ui->label_l2n, ui->label_l3n};
        QLabel *las[] = {ui->label_l1a, ui->label_l2a, ui->label_l3a};
        QLabel *lvs[] = {ui->label_l1v, ui->label_l2v, ui->label_l3v};
        for(int i=0, j=0; j<3 && j<line->size; ++j, ++i) {
            if(mLPage) i = 3;
            lns[i]->setText(tr("L%1:").arg(i + 1));
            las[i]->setText(QString::number((double)line->cur.value[i]/COM_RATE_CUR,'f',1)+"A");
            lvs[i]->setText(QString::number((double)line->vol.value[i]/COM_RATE_VOL,'f',1)+"V");
        }
    } else {
        this->setPortsCount(0);
    }
}

void Cab_PduWid::updateOutput(sDataPacket *packet)
{
    if(packet->devType != PDU_TYPE_IP_PDU && (packet->offLine > 0) && packet->devSpec != 1)
    {
        uchar *sw = packet->data.output.sw;
        sDataUnit *cur = &(packet->data.output.cur);
        int maxSize = cur->size;
        this->setPortsCount(maxSize);

        for(int i = 0;i < portsCount();++i)
        {
            int val = cur->value[i];

            Cab_PortWid* port = this->port(i);
            if(packet->devSpec != 3)
                port->setCurrentValue(val/COM_RATE_CUR);

            if(packet->devSpec == 2) {
                port->setSwitchDisabled();
            } else {
                port->setSwitchState(sw[i]-1);
            }
            val = cur->alarm[i]; if(val<0) val = 0;
            port->setWarningState(val);
        }
    }
}


void Cab_PduWid::updatePacket(sDataPacket *packet)
{
    mLPage = 0; //第一页
    mPacket = packet;
    timeoutDone();
    setEnabled(true);
}

void Cab_PduWid::setIpAddr(const QString &ip, const QString & num )
{
    QString addr = QString("%1---%2").arg(ip).arg(num);
    ui->label_pduSetting->setText(addr);
}



void Cab_PduWid::setTitle(bool is_left)
{
    if(is_left)
        ui->label_2->setText(tr("主要PDU:"));
    else
        ui->label_2->setText(tr("备用PDU:"));
}

/**
 * @brief WidgetPduDevice::portsCount
 * 端口总数
 */
int Cab_PduWid::portsCount()
{
    QLayout * l = m_portWidget->layout();
    if(l) {
        return l->count();
    }
    return 0;
}

/**
 * @brief WidgetPduDevice::port
 * 获取对应端口对象
 */
Cab_PortWid *Cab_PduWid::port(int index)
{
    if(index >= 0 && index < portsCount()){
        QLayoutItem *item = m_portWidget->layout()->itemAt(index);
        if(item)
            return qobject_cast<Cab_PortWid*>(item->widget());
    }
    return nullptr;
}

void Cab_PduWid::setEditable(bool b)
{
    m_portWidget->setEnabled(b);
}

void Cab_PduWid::buttonClick(bool checked)
{
    Cab_PortWid* widget = qobject_cast<Cab_PortWid*>(sender());
    int index = m_portWidget->layout()->indexOf(widget);
    if(index >= 0)
        emit buttonClicked(index,checked);
}

void Cab_PduWid::onSwitchBtnClick()
{
    if(!mLPage){
        mLPage = 1;
        ui->switchBtn->setText(tr("上一页"));
    }else{
        mLPage = 0;
        ui->switchBtn->setText(tr("下一页"));
    }
    if(mPacket) updatePacket(mPacket);
}

/**
 * @brief WidgetPduDevice::setPortsCount
 * 设置端口总数
 */
void Cab_PduWid::setPortsCount(int portsCount)
{
    QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(m_portWidget->layout());
    if(vLayout == nullptr) {
        vLayout = new QVBoxLayout(m_portWidget);
        vLayout->setContentsMargins(0,0,0,0);
    }

    int count = vLayout->count();
    if(count == portsCount)return;
    if(count < portsCount){
        for(int i = count; i < portsCount ;++i){
            Cab_PortWid* w = new Cab_PortWid(m_bLeft,this);
            connect(w,SIGNAL(clicked(bool)),SLOT(buttonClick(bool)));
            w->setIndex(i+1);
            w->setCurrentValue(0);
            vLayout->addWidget(w);
        }
    } else {
        for(int i = count-1; i >= portsCount ;--i)
            vLayout->removeItem(vLayout->itemAt(i));
    }
}


/**
 * @brief 如果离线，那么设置为---
 * @param label
 */
void Cab_PduWid::setZeroOffine()
{
    ui->label_l1a->setText("---");
    ui->label_l1v->setText("---");
    ui->label_l2a->setText("---");
    ui->label_l2v->setText("---");
    ui->label_l3a->setText("---");
    ui->label_l3v->setText("---");
}
