#include "dialogaddcabinet.h"
#include "ui_dialogaddcabinet.h"
#include <QMessageBox>
#include <QIntValidator>
#include "common.h"
#include "pdu/dev/devType/pdudtname.h"

#if defined(ZEBRA_MODULE)
#include "zebra/zebra_client.h"
#include "autoaddfriend/autoaddfriend.h"
#endif

DialogAddCabinet::DialogAddCabinet(CabinetItem &item,QTableWidget* parent) :
    QDialog(parent->parentWidget()),isAdd(true),m_item(item),
    ui(new Ui::DialogAddCabinet)
{
    isModfy=false;//初始化，最开始不是修改
    ui->setupUi(this);
    ui->lineEdit_name->setText(item.name);
    ui->lineEdit_ip->setText(item.main_ip);
    ui->lineEdit_ip_2->setText(item.spare_ip);
    ui->spinBox_deputy->setValue(item.main_num);
    ui->spinBox_deputy_2->setValue(item.spare_num);
    ui->spinBox_height->setValue(item.height);

    if(!item.main_ip.isEmpty())
    {
        ui->comboBox->setCurrentIndex(intToIndex(item.main_type));
    }
    if(!item.spare_ip.isEmpty())
    {
        ui->comboBox_2->setCurrentIndex(intToIndex(item.spare_type));
    }
    mainStype=ui->comboBox->currentText(); //如果是修改，那么将type取为combox未修改时的初始值
    spareStype=ui->comboBox_2->currentText();//必须保证修改时获取的数据包是修改之前的，而不能重新创建
    mainSip=ui->lineEdit_ip->text();
    spareSip=ui->lineEdit_ip_2->text();
    mainNum=ui->spinBox_deputy->text().toInt();
    spareNum=ui->spinBox_deputy_2->text().toInt();

    m_tableWidget = parent;
}

DialogAddCabinet::~DialogAddCabinet()
{
    delete ui;
}

void DialogAddCabinet::accept()
{
    QString ipAddr = ui->lineEdit_ip->text();
    if(!ipAddr.isEmpty()&&cm::isValidIpAddress(ipAddr)==false){
        CriticalMsgBox box(this,tr("主机IP地址无效(%1)!").arg(ipAddr));
        return;
    }

    ipAddr = ui->lineEdit_ip_2->text();
    if(!ipAddr.isEmpty()&&cm::isValidIpAddress(ipAddr)==false){
        CriticalMsgBox box(this,tr("备机IP地址无效(%1)!").arg(ipAddr));
        return;
    }
    QString name = ui->lineEdit_name->text();
    if(name.isEmpty()){
        CriticalMsgBox box(this,tr("机柜名称为空！请填写有效名称！"));
        return;
    }
    else if(name!=m_item.name && m_tableWidget->findItems(name,Qt::MatchExactly).size()>0)
    {
        CriticalMsgBox box(this,tr("机柜中已有对应名称机柜(%1),请重命名！").arg(name));
        return;
    }

    mainStype=ui->comboBox->currentText(); //主机设备类型
    spareStype=ui->comboBox_2->currentText(); //从机设备类型
    mainSip=ui->lineEdit_ip->text();
    spareSip=ui->lineEdit_ip_2->text();
    mainNum=ui->spinBox_deputy->text().toInt();
    spareNum=ui->spinBox_deputy_2->text().toInt();

#if defined(ZEBRA_MODULE)
    if(isModfy)
    {
        QString main_old_ip = m_item.main_ip;
        QString main_new_ip = ui->lineEdit_ip->text();
        if(0 != QString::compare(main_old_ip, main_new_ip))
        {
            int ret = -1;
            if(m_item.main_num == 0)// 2019-1-10 只有主机才删除添加好友  pmd
            {
                ret= zebra_client::get_instance()->delete_friend(main_old_ip);
                if(ret == -1)
                {
                    qDebug() << "修改机柜IP时未找到要删除的 " << main_old_ip << " 好友通道";
                }

                //AutoAddFriend::get_instance()->addFriend(main_new_ip);
            }
        }

        QString spare_old_ip = m_item.spare_ip;
        QString spare_new_ip = ui->lineEdit_ip_2->text();
        if(0 != QString::compare(spare_old_ip, spare_new_ip))
        {
            int ret = -1;
            if(m_item.spare_num == 0)// 2019-1-10 只有主机才删除添加好友  pmd
            {
                ret = zebra_client::get_instance()->delete_friend(spare_old_ip);
                if(ret == -1)
                {
                    qDebug() << "修改机柜IP时未找到要删除的 " << spare_old_ip << " 好友通道";
                }

                //AutoAddFriend::get_instance()->addFriend(spare_new_ip);
            }
        }
    }
#endif

    m_item.name = ui->lineEdit_name->text();
    m_item.main_ip = ui->lineEdit_ip->text();
    m_item.main_num = ui->spinBox_deputy->value();
    m_item.main_type = typeToInt(mainStype);

    m_item.spare_ip = ui->lineEdit_ip_2->text();
    m_item.spare_num = ui->spinBox_deputy_2->value();
    m_item.spare_type = typeToInt(spareStype);
    m_item.height = ui->spinBox_height->value();

    QDialog::accept();


    if(!m_item.main_ip.isEmpty())  //如果主机不为空，则创建
    {
        if(m_item.main_num) //创建数据包时，如果main_num（设备号）不为0即同一IP对应的主机不存在，那么数据报是不能存在的，所以会发生段错误，我们需要人为创建相应数据报
        {
            PduDataPacket * zeroDataPacket = ::pdu_hd_get(typeToInt(mainStype),mainSip,0);
            zeroDataPacket->devType=typeToInt(ui->comboBox->currentText());
            zeroDataPacket->ip->set(ui->lineEdit_ip->text());
        }

        PduDataPacket * dataPacket = ::pdu_hd_get(typeToInt(mainStype),mainSip,mainNum);
        dataPacket->devType = typeToInt(ui->comboBox->currentText());
        dataPacket->ip->set(ui->lineEdit_ip->text());
        m_item.main_ip=ui->lineEdit_ip->text(); //显示ip信息时，是根据m_item来设置的，这里不改，将找不到
        dataPacket->devNum=ui->spinBox_deputy->text().toInt();
        dl_save_add(m_item.main_ip);
    }

    if(!m_item.spare_ip.isEmpty())
    {
        if(m_item.spare_num)
        {
            PduDataPacket * zeroDataPacket = ::pdu_hd_get(typeToInt(spareStype),spareSip,0);
            zeroDataPacket->devType=typeToInt(ui->comboBox_2->currentText());
            zeroDataPacket->ip->set(ui->lineEdit_ip_2->text());
        }
        PduDataPacket * dataPacket = ::pdu_hd_get(typeToInt(spareStype),spareSip,spareNum);
        dataPacket->devType = typeToInt(ui->comboBox_2->currentText());
        dataPacket->ip->set(ui->lineEdit_ip_2->text());
        m_item.spare_ip=ui->lineEdit_ip_2->text(); //
        dataPacket->devNum=ui->spinBox_deputy_2->text().toInt();
        dl_save_add(m_item.spare_ip);
    }
}

DialogModifyCabinet::DialogModifyCabinet(CabinetItem &item,QTableWidget* parent) :
    DialogAddCabinet(item,parent)
{
    this->setWindowTitle(tr("修改机柜"));
    ui->label_describe->setText(tr("请填写修改机柜的详细信息："));
    isAdd = false;
    isModfy=true;
}

int DialogAddCabinet::typeToInt(QString type)  //判断combox中的设备类型
{
    int ret = PDU_TYPE_IP_PDU;
    if(type == "IP-PDU")
        ret = PDU_TYPE_IP_PDU;
    else if(type == "MPDU")
        ret = PDU_TYPE_M_PDU;
    else if(type == "RPDU")
        ret = PDU_TYPE_RPDU;
    else if(type == "NPM")
        ret = PDU_TYPE_NPM_PDU;
    else if(type == "ZPDU")
        ret = PDU_TYPE_ZPDU;

    return ret;
}

int DialogAddCabinet::intToIndex(int type)
{
    int ret = 0;
    switch (type)
    {
    case PDU_TYPE_IP_PDU:
        ret = 0;
        break;
    case PDU_TYPE_M_PDU:
        ret = 1;
        break;
    case PDU_TYPE_RPDU:
        ret = 2;
        break;
    case PDU_TYPE_NPM_PDU:
        ret = 3;
        break;
    case PDU_TYPE_ZPDU:
        ret = 4;
        break;
    default:
        break;
    }
    return ret;
}
