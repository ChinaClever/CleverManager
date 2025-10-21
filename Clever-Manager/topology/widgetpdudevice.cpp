#include "widgetpdudevice.h"
#include "ui_widgetpdudevice.h"
#include <QDebug>
#include "common.h"

WidgetPduDevice::WidgetPduDevice(QWidget *parent) :
    QWidget(parent),ui(new Ui::WidgetPduDevice)
{
    ui->setupUi(this);
    m_portWidget = new QWidget(this);
#if !defined(ZEBRA_MODULE) && !defined(SNMP_MODULE)
    ui->pushButton_setchannel->setVisible(false);
#endif
    connect(ui->pushButton_setchannel,SIGNAL(clicked()),this,SIGNAL(buttonChannelManager_clicked()));
    connect(ui->switchBtn, SIGNAL(clicked()), this, SLOT(onSwitchBtnClick()));
    mLPage = 0; //第一页
    ui->switchBtn->hide(); //默认隐藏
    mPacket = NULL;
}

WidgetPduDevice::~WidgetPduDevice()
{
    delete ui;
}

/**
 * @brief WidgetPduDevice::setAlignType
 * 设置按钮靠左靠右。显示
 * @param left
 */
void WidgetPduDevice::setAlignType(bool left)
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

void WidgetPduDevice::updatePacket(PduDataPacket *packet)
{
    if(packet->devType != PDU_TYPE_IP_PDU && packet->offLine && packet->devSpec != 1) //只有当设备不为ippdu时，输出位才是有效的
    {//2018-11-20 pmd 添加离线状态
        PduDataUnit *cur = packet->data->output->cur;
        PduDataBase *sw = packet->data->output->sw;
        int maxSize = qMax(cur->value->size(),sw->size());
        this->setPortsCount(maxSize);
        for(int i = 0;i < portsCount();++i)
        {
            int val = cur->value->get(i);
            FormPowerPort* port = this->port(i);
            if(packet->devSpec != 3)
                port->setCurrentValue(val/COM_RATE_CUR);

            if(packet->devSpec == 2) {
                port->setSwitchDisabled();
            } else {
                port->setSwitchState(sw->get(i)-1);
            }
            val = cur->alarm->get(i); if(val<0) val = 0;
            port->setWarningState(val);
        }
    }

    //隐藏或显示按钮
    mPacket = packet;
    int phaseNum = packet->data->line->cur->value->size();
    if(phaseNum > 3){
        ui->switchBtn->show();
    }else{
        ui->switchBtn->hide();
    }
    //更新三项di电流电压
    PduObjData *line = packet->data->line;
    int i;
    if(!mLPage){ //第一页
        i = 0;
        ui->label_l1n->setText(tr("L%1:").arg(i + 1));
        ui->label_l1a->setText(QString::number((double)line->cur->value->get(i)/10,'f',1)+"A");
        ui->label_l1v->setText(QString::number((double)line->vol->value->get(i++)/10,'f',1)+"V");
        ui->label_l2n->setText(tr("L%1:").arg(i + 1));
        ui->label_l2a->setText(QString::number((double)line->cur->value->get(i)/10,'f',1)+"A");
        ui->label_l2v->setText(QString::number((double)line->vol->value->get(i++)/10,'f',1)+"V");
        ui->label_l3n->setText(tr("L%1:").arg(i + 1));
        ui->label_l3a->setText(QString::number((double)line->cur->value->get(i)/10,'f',1)+"A");
        ui->label_l3v->setText(QString::number((double)line->vol->value->get(i++)/10,'f',1)+"V");
    }else{
        i = 3;
        ui->label_l1n->setText(tr("L%1:").arg(i + 1));
        ui->label_l1a->setText(QString::number((double)line->cur->value->get(i)/10,'f',1)+"A");
        ui->label_l1v->setText(QString::number((double)line->vol->value->get(i++)/10,'f',1)+"V");
        ui->label_l2n->setText(tr("L%1:").arg(i + 1));
        ui->label_l2a->setText(QString::number((double)line->cur->value->get(i)/10,'f',1)+"A");
        ui->label_l2v->setText(QString::number((double)line->vol->value->get(i++)/10,'f',1)+"V");
        ui->label_l3n->setText(tr("L%1:").arg(i + 1));
        ui->label_l3a->setText(QString::number((double)line->cur->value->get(i)/10,'f',1)+"A");
        ui->label_l3v->setText(QString::number((double)line->vol->value->get(i++)/10,'f',1)+"V");
    }

    //如果机柜中设备不存在，为-1，则置---
    setZero(ui->label_l1a);
    setZero(ui->label_l1v);
    setZero(ui->label_l2a);
    setZero(ui->label_l2v);
    setZero(ui->label_l3a);
    setZero(ui->label_l3v);
    //如果设备离线，为0，则置---
    //qDebug() << "IP:"<< packet->ip->get() << "offine" << packet->offLine;
    if( packet->offLine == 0 )
    {
        //        qDebug() << "设置为0";
        this->setPortsCount(0);//2018-11-20 pmd 离线时删除开关键电流标签
        setZeroOffine();
    }
}

void WidgetPduDevice::setIpAddr(const QString &ip, int num )
{
    QString addr = QString("%1---%2").arg(ip).arg(num);
    //    ui->label_pdu->setText(addr);
    ui->label_pduSetting->setText(addr);
    //    ui->label_2->setText(title);
    if(num > 0)/////2019-3-13 pmd 副机时隐藏切换通道对话框
        ui->pushButton_setchannel->hide();
}

void WidgetPduDevice::setTitle(bool is_left)
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
int WidgetPduDevice::portsCount()
{
    QLayout * l = m_portWidget->layout();
    if(l)
    {
        return l->count();
    }
    return 0;
}

/**
 * @brief WidgetPduDevice::port
 * 获取对应端口对象
 */
FormPowerPort *WidgetPduDevice::port(int index)
{
    if(index >= 0 && index < portsCount()){
        QLayoutItem *item = m_portWidget->layout()->itemAt(index);
        if(item)
            return qobject_cast<FormPowerPort*>(item->widget());
    }
    return nullptr;
}

void WidgetPduDevice::setEditable(bool b)
{
    ui->pushButton_setchannel->setEnabled(b);
    m_portWidget->setEnabled(b);
}

void WidgetPduDevice::buttonClick(bool checked)
{
    FormPowerPort* widget = qobject_cast<FormPowerPort*>(sender());
    int index = m_portWidget->layout()->indexOf(widget);
    if(index >= 0)
        emit buttonClicked(index,checked);
}

void WidgetPduDevice::onSwitchBtnClick()
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
void WidgetPduDevice::setPortsCount(int portsCount)
{
    QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(m_portWidget->layout());
    if(vLayout == nullptr) {
        vLayout = new QVBoxLayout(m_portWidget);
        vLayout->setContentsMargins(0,0,0,0);
//        vLayout->setSpacing(1);
    }

    int count = vLayout->count();
    if(count == portsCount)return;
    if(count < portsCount){
        for(int i = count; i < portsCount ;++i){
            FormPowerPort* w = new FormPowerPort(m_bLeft,this);
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
 * @brief WidgetPduDevice::setZero
 * 如果接收数据小于0，那么就将其置零
 */
void WidgetPduDevice::setZero(QLabel *label)
{
    //    qDebug()<<label->text()<<"come on hello world";
    if(label->text() == "-0.1A")
        label->setText("---");
    else if(label->text() == "-0.1V")
        label->setText("---");
}

/**
 * @brief 如果离线，那么设置为---
 * @param label
 */
void WidgetPduDevice::setZeroOffine()
{
    ui->label_l1a->setText("---");
    ui->label_l1v->setText("---");
    ui->label_l2a->setText("---");
    ui->label_l2v->setText("---");
    ui->label_l3a->setText("---");
    ui->label_l3v->setText("---");
}
