/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_cabinetwid.h"
#include "ui_cab_cabinetwid.h"
#include "cab_adddevicedlg.h"
#include "cab_removerackdlg.h"
#include <QMessageBox>
#include "backcolourcom.h"

Cab_CabinetWid::Cab_CabinetWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cab_CabinetWid)
{
    ui->setupUi(this);

    ui->widget_left->setTitle(true);
    ui->widget_right->setTitle(false);
    m_menuAdd.addAction(ui->action_add);
    m_menuModify.addAction(ui->action_modify);
    m_menuModify.addAction(ui->action_delete);

    ui->widget_left->setAlignType(true);
    ui->widget_right->setAlignType(false);

    connect(ui->tableWidget_cabinet,SIGNAL(currentAddressChange(int)),SLOT(tableAddressChange(int)));
    connect(ui->widget_left,SIGNAL(buttonClicked(int,bool)),SLOT(buttonClicked(int,bool)));
    connect(ui->widget_right,SIGNAL(buttonClicked(int,bool)),SLOT(buttonClicked(int,bool)));

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Cab_CabinetWid::~Cab_CabinetWid()
{
    delete ui;
}

void Cab_CabinetWid::init(const CabinetItem& item)
{
    QString title = tr("机房：%1 机柜：%2 位置:(%3,%4)").arg(item.room).arg(item.cab)
            .arg(item.row+1).arg(item.column+1);
    ui->titleLab->setText(title);

    int rouCount = item.height;
    ui->tableWidget_cabinet->setRowCount(rouCount);
    for(int i=0 ; i < rouCount ; i+=2){
        ui->tableWidget_cabinet->setItem(i,0,new QTableWidgetItem(QString::number(rouCount-i)));
        if(i+1 < rouCount){
            ui->tableWidget_cabinet->setItem(i+1,0,new QTableWidgetItem(QString::number(rouCount-i-1)));
            ui->tableWidget_cabinet->setSpan(i,0,2,1);
        }
    }
    QVector<ConsumerDeviceItem>  items = DbConsumerDevices::get()->selectItemsByCabinetId(item.id);
    ui->tableWidget_cabinet->setItems(items);

    sCabPacket *cab = Dp_CabPackets::bulid(this)->getByCab(item.id);
    if(item.isValideMain()) {
        ui->widget_left->updatePacket(cab->m);
    } else {
        ui->widget_left->setEnabled(false);
    }

    if(item.isValideSpare()) {
        ui->widget_right->updatePacket(cab->s);
    } else {
        ui->widget_right->setEnabled(false);
    }

//    setEditable(false);
}

void Cab_CabinetWid::selectSlot(int id)
{
    m_cabinetitem = DbCabinetList::get()->findById(id);
    init(m_cabinetitem);
}

void Cab_CabinetWid::setEditable(bool e)
{
    ui->widget_left->setEditable(e);
    ui->widget_right->setEditable(e);
    ui->tableWidget_cabinet->setEnabled(e);
}


//弹出右键菜单
void Cab_CabinetWid::on_tableWidget_cabinet_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tableWidget_cabinet->currentIndex();
    if(index.isValid()){
        if(ui->tableWidget_cabinet->deviceItem(ui->tableWidget_cabinet->currentAddress()).id<0)
            m_menuAdd.exec(ui->tableWidget_cabinet->mapToGlobal(pos));
        else
            m_menuModify.exec(ui->tableWidget_cabinet->mapToGlobal(pos));
    }
}

//添加对应机柜
void Cab_CabinetWid::on_action_add_triggered()
{
    ConsumerDeviceItem item;
    item.address = ui->tableWidget_cabinet->currentAddress();
    item.cabinet_id = m_cabinetitem.id;

    Cab_AddDeviceDlg dlg(this);
    dlg.init(item,ui->widget_left->portsCount(),ui->widget_right->portsCount(),m_cabinetitem.height);
    if(QDialog::Accepted == dlg.exec()){
        if(!ui->tableWidget_cabinet->addItem(dlg.getItem())){
            QMessageBox::warning(this,tr("警告"),tr("当前机架占用范围(位置:%1,高度:%2)有机架设备！").arg(item.address).arg(item.occupy_height));
        }
    }
}


//删除对应机柜
void Cab_CabinetWid::on_action_delete_triggered()
{
    Cab_RemoveRackDlg dlg;
    dlg.setAddress(m_deviceItem.address);
    if(QDialog::Accepted == dlg.exec()){
        ConsumerDeviceItem item = ui->tableWidget_cabinet->deviceItem(dlg.address(),true);
        if(item.id < 0){
            QMessageBox::warning(this,tr("警告"),tr("位置%1未放置机架设备！").arg(dlg.address()));
        } else {
            ui->tableWidget_cabinet->removeItem(item);
            DbConsumerDevices::get()->remove(item.id);
        }
    }
}

//修改对应机柜
void Cab_CabinetWid::on_action_modify_triggered()
{
    if(m_deviceItem.id < 0)return;
    Cab_ModifyDeviceDlg dlg(this);
    dlg.init(m_deviceItem,ui->widget_left->portsCount(),ui->widget_right->portsCount(),m_cabinetitem.height);
    if(QDialog::Accepted == dlg.exec()){
        m_deviceItem = dlg.getItem();
        if(!ui->tableWidget_cabinet->updateItem(m_deviceItem)){
            QMessageBox::warning(this,tr("警告"),tr("当前机架占用范围(位置:%1,高度:%2)有机架设备！").arg(m_deviceItem.address).arg(m_deviceItem.occupy_height));
        }
    }
}

void Cab_CabinetWid::tableAddressChange(int address)
{
    //更新当前Item
    m_deviceItem = ui->tableWidget_cabinet->deviceItem(address);
    update();
}


void Cab_CabinetWid::buttonClicked(int index, bool opened)
{
    Cab_PduWid* w = qobject_cast<Cab_PduWid*>(sender());
    if(w == ui->widget_left) {
        if(!setPortSwitch(m_cabinetitem.main_ip,m_cabinetitem.main_num,index,opened))
            w->port(index)->setSwitchState(!opened);
    } else if(w == ui->widget_right) {
        if(setPortSwitch(m_cabinetitem.spare_ip,m_cabinetitem.spare_num,index,opened))
            w->port(index)->setSwitchState(!opened);
    }
}

/**
 * @brief Cab_CabinetWid::paintEvent
 * 绘制机架到对应端口的线
 * @param e
 */
void Cab_CabinetWid::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    if(m_deviceItem.id < 0)return;//判断当前鼠标下的机架是否有效

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(cm_color(m_deviceItem.address));//设置为对应机架的颜色

    QRect rectDev = ui->tableWidget_cabinet->itemRect(m_deviceItem);//机架位置
    rectDev.moveTopLeft(ui->tableWidget_cabinet->viewport()->mapTo(this,rectDev.topLeft()));

    //绘制主pdu端口到机架设备的映射
    Cab_PortWid* port = ui->widget_left->port(m_deviceItem.main_pdu_port-1);
    if(port){
        QRect rectPort = port->button()->rect();
        rectPort.moveTopLeft(port->button()->mapTo(this,rectPort.topLeft()));
        QPoint portCenter = rectPort.center();
        QPoint deviceLeft = QPoint(rectDev.left(),(rectDev.top()+rectDev.bottom())/2);
        painter.drawLine(portCenter,deviceLeft);
        painter.drawRect(rectPort);
    }
    //绘制备pdu端口到机架设备的映射
    port = ui->widget_right->port(m_deviceItem.spare_pdu_port-1);
    if(port){
        QRect rectPort = port->button()->rect();
        rectPort.moveTopLeft(port->button()->mapTo(this,rectPort.topLeft()));
        QPoint portCenter = rectPort.center();
        QPoint deviceRight = QPoint(rectDev.right(),(rectDev.top()+rectDev.bottom())/2);
        painter.drawLine(portCenter,deviceRight);
        painter.drawRect(rectPort);
    }
}



extern bool set_pdu_outputSw(const QString &ip,int devNum, int bit, bool sw);
bool Cab_CabinetWid::setPortSwitch(const QString &ip, const QString &num,int index,bool opened)
{
    bool ok;
    QString str = num.right(1);
    int ret = str.toInt(&ok);
    if(!ok) ret = 0;

    return set_pdu_outputSw(ip, ret, index+1,opened); // Lzy
}
