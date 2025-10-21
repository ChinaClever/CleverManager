#include "dialogcabinetpostion.h"
#include "ui_dialogcabinetpostion.h"
#include "ui_widgetpdudevice.h"
#include "dialogaddcabinetdevice.h"
#include "dialogremoverack.h"
#include <QMessageBox>
#include "signaldispatch.h"
#include "common.h"
#include "data/hash/read/pduhashread.h"
#include "common_snmp.h"
#include "snmpsetting.h"
#include "dialogchannelmanager.h"
#include "dbpducommandchannel.h"
#include "zebra/zebra_client.h"
#include "sql/dc/storage/cloudserdev.h"
#include "net/datadone/netdataformat.h"
#include "pdu/data/hash/read/pduhashread.h"
#include "net/datadone/netpackdata.h"


DialogCabinetPostion::DialogCabinetPostion(const CabinetItem& item,QWidget *parent) :
    QDialog(parent),m_cabinetitem(item),
    ui(new Ui::DialogCabinetPostion)
{
    ui->setupUi(this);
     ui->widget_left->setTitle(true);
     ui->widget_right->setTitle(false);
    this->setWindowTitle(tr("机柜位置:(%1,%2)  名称:%3").arg(item.row+1).arg(item.column+1).arg(item.name));
    m_menuAdd.addAction(ui->action_add);
    m_menuModify.addAction(ui->action_modify);
    m_menuModify.addAction(ui->action_delete);
    const int rouCount = m_cabinetitem.height;
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

    ui->widget_left->setAlignType(true);
    ui->widget_right->setAlignType(false);
    if(m_cabinetitem.isValideMain())
        ui->widget_left->setIpAddr(m_cabinetitem.main_ip,m_cabinetitem.main_num);
    if(m_cabinetitem.isValideSpare())
        ui->widget_right->setIpAddr(m_cabinetitem.spare_ip,m_cabinetitem.spare_num);

    if(m_cabinetitem.main_ip.isEmpty())/////2019-3-13 pmd IP为空不能弹出切换通道对话框
        ui->widget_left->setEnabled(false);
    if(m_cabinetitem.spare_ip.isEmpty())
        ui->widget_right->setEnabled(false);

    connect(ui->tableWidget_cabinet,SIGNAL(currentAddressChange(int)),SLOT(tableAddressChange(int)));
    connect(ui->widget_left,SIGNAL(buttonClicked(int,bool)),SLOT(buttonClicked(int,bool)));
    connect(ui->widget_left,SIGNAL(buttonChannelManager_clicked()),SLOT(buttonChannelManager_clicke()));
    connect(ui->widget_right,SIGNAL(buttonClicked(int,bool)),SLOT(buttonClicked(int,bool)));
    connect(ui->widget_right,SIGNAL(buttonChannelManager_clicked()),SLOT(buttonChannelManager_clicke()));
    connect(SignalDispatch::get(),SIGNAL(dispatchChange(PduDataPacket*)),SLOT(updatePacket(PduDataPacket*)));

    if(item.isValideMain()){
        PduDataPacket *packet = ::pdu_hd_getPacket((QString &)item.main_ip,item.main_num);
        if(packet->offLine == 0)/////2019-3-19 pmd PDU离线不能弹出切换通道对话框
            ui->widget_left->setEnabled(false);
        updatePacket(packet);
    }
    if(item.isValideSpare()){
        PduDataPacket *packet = ::pdu_hd_getPacket((QString &)item.spare_ip,item.spare_num);
        if(packet->offLine == 0)
            ui->widget_right->setEnabled(false);
        updatePacket(packet);
    }
    setEditable(false);
}

void DialogCabinetPostion::setEditable(bool e)
{
    ui->widget_left->setEditable(e);
    ui->widget_right->setEditable(e);
    ui->tableWidget_cabinet->setEnabled(e);
}

DialogCabinetPostion::~DialogCabinetPostion()
{
    delete ui;
}
//弹出右键菜单
void DialogCabinetPostion::on_tableWidget_cabinet_customContextMenuRequested(const QPoint &pos)
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
void DialogCabinetPostion::on_action_add_triggered()
{
    ConsumerDeviceItem item;
    item.address = ui->tableWidget_cabinet->currentAddress();
    item.cabinet_id = m_cabinetitem.id;
    DialogAddCabinetDevice dlg(item,ui->widget_left->portsCount(),ui->widget_right->portsCount(),m_cabinetitem.height,this);
    if(QDialog::Accepted == dlg.exec()){
        try{
            item.id = DbConsumerDevices::get()->maxId()+1;
            if(ui->tableWidget_cabinet->addItem(item)){
                DbConsumerDevices::get()->insertItem(item);
            } else {
                QMessageBox::warning(this,tr("警告"),tr("当前机架占用范围(位置:%1,高度:%2)有机架设备！").arg(item.address).arg(item.occupy_height));
            }
        }catch(const SqlExecption& execption){
            qDebug() << execption.msg() << execption.type();
        }
    }
}

//删除对应机柜
void DialogCabinetPostion::on_action_delete_triggered()
{
    DialogRemoveRack dlg;
    dlg.setAddress(m_deviceItem.address);
    if(QDialog::Accepted == dlg.exec()){
        try{
            ConsumerDeviceItem item = ui->tableWidget_cabinet->deviceItem(dlg.address(),true);
            if(item.id < 0){
                QMessageBox::warning(this,tr("警告"),tr("位置%1未放置机架设备！").arg(dlg.address()));
            } else {
#if defined(CLOUD_MODULE)
                CloudSerDev::bulid()->del(item.id); ///=== 只用来做试验 验证、测试用
#endif
                ui->tableWidget_cabinet->removeItem(item);
                DbConsumerDevices::get()->removeItem(item);
            }
        }catch(const SqlExecption& execption){
            qDebug() << execption.msg() << execption.type();
        }
    }
}

//修改对应机柜
void DialogCabinetPostion::on_action_modify_triggered()
{
    try{
        if(m_deviceItem.id < 0)return;
        DialogModifyCabinetDevice dlg(m_deviceItem,ui->widget_left->portsCount(),ui->widget_right->portsCount(),m_cabinetitem.height,this);
        if(QDialog::Accepted == dlg.exec()){
            if(ui->tableWidget_cabinet->updateItem(m_deviceItem)){
                DbConsumerDevices::get()->updateItem(m_deviceItem);
            } else {
                QMessageBox::warning(this,tr("警告"),tr("当前机架占用范围(位置:%1,高度:%2)有机架设备！").arg(m_deviceItem.address).arg(m_deviceItem.occupy_height));
            }
        }
    }catch(const SqlExecption& execption){
        qDebug() << execption.msg() << execption.type();
    }
}

void DialogCabinetPostion::tableAddressChange(int address)
{
    //更新当前Item
    m_deviceItem = ui->tableWidget_cabinet->deviceItem(address);
    update();
}

void DialogCabinetPostion::updatePacket(PduDataPacket *packet)
{
    if(packet == nullptr)
    {
        return;
    }

    //主机时
    if(packet->ip->get() == m_cabinetitem.main_ip && packet->devNum == m_cabinetitem.main_num)
    {
        ui->widget_left->updatePacket(packet);
    }

    //备用机时
    if(packet->ip->get() == m_cabinetitem.spare_ip && packet->devNum == m_cabinetitem.spare_num)
    {
        ui->widget_right->updatePacket(packet);
    }
}

void DialogCabinetPostion::buttonClicked(int index, bool opened)
{
    WidgetPduDevice* w = qobject_cast<WidgetPduDevice*>(sender());
    try
    {
        if(w == ui->widget_left)
        {
            if(!setPortSwitch(m_cabinetitem.main_ip,m_cabinetitem.main_num,index,opened))
                w->port(index)->setSwitchState(!opened);
        }
        else if(w == ui->widget_right)
        {
            if(setPortSwitch(m_cabinetitem.spare_ip,m_cabinetitem.spare_num,index,opened))
                w->port(index)->setSwitchState(!opened);
        }
    } catch(const SqlExecption& exception) {
        qDebug() << exception.msg() << exception.type();
    }
}

/**
 * @brief DialogCabinetPostion::paintEvent
 * 绘制机架到对应端口的线
 * @param e
 */
void DialogCabinetPostion::paintEvent(QPaintEvent *e)
{
    QDialog::paintEvent(e);

    if(m_deviceItem.id < 0)return;//判断当前鼠标下的机架是否有效

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(cm::color(m_deviceItem.address));//设置为对应机架的颜色

    QRect rectDev = ui->tableWidget_cabinet->itemRect(m_deviceItem);//机架位置
    rectDev.moveTopLeft(ui->tableWidget_cabinet->viewport()->mapTo(this,rectDev.topLeft()));

    //绘制主pdu端口到机架设备的映射
    FormPowerPort* port = ui->widget_left->port(m_deviceItem.main_pdu_port-1);
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

void DialogCabinetPostion::buttonChannelManager_clicke()
{
    WidgetPduDevice* w = reinterpret_cast<WidgetPduDevice*>(sender());
    QString ipAddr;
    int num;
    if(w == ui->widget_left)
    {
        ipAddr = m_cabinetitem.main_ip;
        num = m_cabinetitem.main_num;
    }
    else if(w == ui->widget_right)
    {
        ipAddr = m_cabinetitem.spare_ip;
        num = m_cabinetitem.spare_num;
    }

    if(cm::isValidIpAddress(ipAddr))
    {
        DialogChannelManager dlg(ipAddr,num);
        dlg.exec();
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("非法IP地址(%1)").arg(ipAddr));
    }
}

extern bool set_pdu_outputSw(const QString &ip,int devNum, int bit, bool sw);
bool DialogCabinetPostion::setPortSwitch(const QString &ip, int num,int index,bool opened)
{
    return set_pdu_outputSw(ip, num, index+1,opened); // Lzy
}
