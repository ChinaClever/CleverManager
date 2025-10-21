#include "dialogchannelmanager.h"
#include "ui_dialogchannelmanager.h"
#include "common.h"
#include "dbpducommandchannel.h"
#include "signaldispatch.h"
#include "pdudatapacket.h"
#include "data/hash/read/pduhashread.h"
#include "zebra/zebra_client.h"
#include "zebra/common_debug.h"
#include <atomic>
extern std::atomic_int zebra_network_state;

DialogChannelManager::DialogChannelManager(const QString &ipAddr,int num) :
    ui(new Ui::DialogChannelManager),m_num(num)
{
    ui->setupUi(this);
    ui->label_ip->setText(ipAddr);
    init();
    connect(SignalDispatch::get(),SIGNAL(dispatchChange(PduDataPacket*)),SLOT(updatePacket(PduDataPacket*)));
    setWindowTitle(tr("通道设置"));
    setWindowIcon(QIcon(":/images/logo.jpg"));

#ifndef ZEBRA_MODULE
    ui->checkBox_zebra->setVisible(false);
    ui->radioButton_zebra->setVisible(false);
#else
    zpdu_set_channel = -1;
    zpdu_trap_channel = -1;
    connect(SignalDispatch::get(),
            SIGNAL(update_zpdu_channel_state_sig(QString, int, int)),
            this,
            SLOT(update_zpdu_channel_state(QString, int, int)));
#endif

#ifndef SNMP_MODULE
    ui->checkBox_snmp->setVisible(false);
    ui->radioButton_snmp->setVisible(false);
#endif

    ui->checkBox_snmp->hide();/////////////////////////////2018-11-20 pmd隐藏
    ui->radioButton_zebra->hide();
    ui->radioButton_snmp->hide();

}

DialogChannelManager::~DialogChannelManager()
{
    delete ui;
}

void DialogChannelManager::init()
{
    try
    {
        DbPduCommandChannelItem qvec = DbPduCommandChannel::get()->selectItemByIp(ui->label_ip->text());
        int getChannel = qvec.get_channel;
        int setChannel = qvec.set_channel;

        ui->radioButton_clever->setChecked(true);
        ui->checkBox_clever->setChecked(setChannel & CHANNEL_CLEVER);

#ifdef SNMP_MODULE
        ui->checkBox_snmp->setChecked(setChannel & CHANNEL_SNMP);
        ui->radioButton_snmp->setChecked(getChannel & CHANNEL_SNMP);
#endif

#ifdef ZEBRA_MODULE
        ui->checkBox_zebra->setChecked(setChannel & CHANNEL_ZEBRA);
        ui->radioButton_zebra->setChecked(getChannel & CHANNEL_ZEBRA);

        if((setChannel & CHANNEL_CLEVER) && (setChannel & CHANNEL_ZEBRA))
        {
            ui->zpdu_state_label->setText("TCP + ZEBRA");
        }
        else if(setChannel & CHANNEL_ZEBRA)
        {
            ui->zpdu_state_label->setText("ZEBRA");
        }
        else // if(setChannel & CHANNEL_CLEVER) ////====
        {
            ui->zpdu_state_label->setText("TCP");
        }
//        else
//        {
//            MY_DEBUG(2, YELLOW) << "setChannel = " << setChannel << DEBUG_END;
//        }

        if(getChannel & CHANNEL_ZEBRA)
        {
            ui->zpdu_trap_label->setText("ZEBRA");
        }
        else //if(getChannel & CHANNEL_CLEVER) ////===
        {
            ui->zpdu_trap_label->setText("TCP");
        }
//        else
//        {
//            MY_DEBUG(2, YELLOW) << "getChannel = " << getChannel << DEBUG_END;
//        }


        if((false == ui->checkBox_zebra->isChecked()) &&
            (false == ui->checkBox_snmp->isChecked()) &&
            (false == ui->checkBox_clever->isChecked()))
        {
            ui->checkBox_clever->setChecked(true);
        }
#endif
    }
    catch(const SqlExecption& exception)
    {
            qDebug() << exception.msg() << exception.type();
    }

    QString ip = ui->label_ip->text();
    PduDataPacket * packet  = ::pdu_hd_getPacket(ip,m_num);
    updatePacket(packet);
}

#if defined(ZEBRA_MODULE)
int DialogChannelManager::send_set_channel_info(int set_channel)
{
    QString channel_name;
    if((set_channel & CHANNEL_SNMP) && (set_channel & CHANNEL_ZEBRA))
    {
        channel_name = QString::number(9);
    }
    else if((set_channel & CHANNEL_CLEVER) && (set_channel & CHANNEL_ZEBRA))
    {
        channel_name = QString::number(TCP_ZEBRA_SET_CHANNEL);
    }
    else if(set_channel & CHANNEL_SNMP)
    {
        channel_name = QString::number(9);
    }
    else if(set_channel & CHANNEL_ZEBRA)
    {
        channel_name = QString::number(ZEBRA_SET_CHANNE);
    }
    else if(set_channel & CHANNEL_CLEVER)
    {
        channel_name = QString::number(TCP_SET_CHANNEL);
    }
    else
    {
        qDebug() << "[MYDEBUG : ERROR]DialogChannelManager::send_set_channel_info() -"
                 << " error set_channel = " << channel_name;
    }
    zebra_client::get_instance()->send_set_channel_mode(ui->label_ip->text(), channel_name);

    return 0;
}
#endif

void DialogChannelManager::on_pushButton_ok_clicked()
{
    if(zebra_network_state > 3)
    {
        try
        {
            QString channel_str;
            DbPduCommandChannelItem qvec = DbPduCommandChannel::get()->selectItemByIp(ui->label_ip->text());
            int setChannel_old = qvec.set_channel;
            int getChannel_old = qvec.get_channel;
            int setChannel_new = 0;
            setChannel_new = ui->checkBox_zebra->isChecked()?CHANNEL_ZEBRA:0;
            setChannel_new |= ui->checkBox_clever->isChecked()?CHANNEL_CLEVER:0;
            setChannel_new |= ui->checkBox_snmp->isChecked()?CHANNEL_SNMP:0;
            //        if(qvec.id < 0)
            //        {
            //            setChannel_old = setChannel_new;
            //        }

            int getChannel_new = 0;
            if(ui->radioButton_zebra->isChecked())
            {
                getChannel_new = CHANNEL_ZEBRA;
                channel_str = QString::number(ZEBRA_TRAP_CHANNEL);
            }
            else if(ui->radioButton_clever->isChecked())
            {
                getChannel_new = CHANNEL_CLEVER;
                channel_str = QString::number(TCP_TRAP_CHANNEL);
            }
            else if(ui->radioButton_snmp->isChecked())
            {
                getChannel_new = CHANNEL_SNMP;
                channel_str = QString::number(-1);
            }
            else
            {
                getChannel_new = -1;
            }

            if(setChannel_old != setChannel_new)
            {
                QMessageBox::StandardButton button;
                button = QMessageBox::question(this,
                                               tr("注意"),
                                               QString(tr("指令下发的TCP通道与其他通道互相切换时，ZPDU将自动重启，以使对应的防火墙策略生效，确认是否切换通道？")),
                                               QMessageBox::Yes | QMessageBox::No);
                if (button == QMessageBox::No)
                {

                }
                else if(button == QMessageBox::Yes)
                {

                    if((false == ui->checkBox_zebra->isChecked()) &&
                            (false == ui->checkBox_snmp->isChecked()) &&
                            (false == ui->checkBox_clever->isChecked()))
                    {
                        ui->checkBox_clever->setChecked(true);
                    }

                    qvec.ip = ui->label_ip->text();

#ifdef ZEBRA_MODULE
                    qvec.set_channel = ui->checkBox_zebra->isChecked()?CHANNEL_ZEBRA:0;
                    if(ui->radioButton_zebra->isChecked())
                    {
                        qvec.get_channel = CHANNEL_ZEBRA;
                    }
#endif

#ifdef SNMP_MODULE
                    qvec.set_channel |= ui->checkBox_snmp->isChecked()?CHANNEL_SNMP:0;
                    if(ui->radioButton_snmp->isChecked())
                    {
                        qvec.get_channel = CHANNEL_SNMP;
                    }
#endif

                    qvec.set_channel |= ui->checkBox_clever->isChecked()?CHANNEL_CLEVER:0;
                    if(ui->radioButton_clever->isChecked())
                    {
                        qvec.get_channel = CHANNEL_CLEVER;
                    }

#if defined(ZEBRA_MODULE)
                    send_set_channel_info(qvec.set_channel);
                    if(getChannel_old != getChannel_new)
                    {
                        zebra_client::get_instance()->set_get_channel_mode(ui->label_ip->text(), channel_str);
                    }
#endif

                    if(qvec.id < 0) //insert
                    {
                        DbPduCommandChannel::get()->insertItem(qvec);
                    }
                    else
                    {
                        DbPduCommandChannel::get()->updateItem(qvec);
                    }
                }
            }
            else
            {
                if(getChannel_old != getChannel_new)
                {
                    qvec.ip = ui->label_ip->text();
                    qvec.set_channel = setChannel_old;
                    qvec.get_channel = getChannel_new;
                    if(qvec.id < 0) //insert
                    {
                        DbPduCommandChannel::get()->insertItem(qvec);
                    }
                    else
                    {
                        DbPduCommandChannel::get()->updateItem(qvec);
                    }

#if defined(ZEBRA_MODULE)
                    zebra_client::get_instance()->set_get_channel_mode(ui->label_ip->text(), channel_str);
#endif
                }
                else
                {
#if defined(ZEBRA_MODULE)
                    /* 如果管理软件与ZPDU通道状态不符，则管理端自动修正 */
                    if(((-1 != zpdu_set_channel) && (-1 != zpdu_trap_channel)) &&
                            ((zpdu_set_channel != setChannel_new) || (zpdu_trap_channel != getChannel_new)))
                    {
                        qvec.ip = ui->label_ip->text();
                        qvec.set_channel = zpdu_set_channel;
                        qvec.get_channel = zpdu_trap_channel;
                        if(qvec.id < 0) //insert
                        {
                            DbPduCommandChannel::get()->insertItem(qvec);
                        }
                        else
                        {
                            DbPduCommandChannel::get()->updateItem(qvec);
                        }
                    }
#endif
                }
            }
        }
        catch(const SqlExecption& exception)
        {
            qDebug() << exception.msg() << exception.type();
        }

        accept();
    }
    else
        qDebug()<<"组网还没有成功，不能切换通道";
}

void DialogChannelManager::updatePacket(PduDataPacket *packet)
{
    /*qDebug() << "[MYDEBUG : INFO] - DialogChannelManager::updatePacket() offLine = "
             << packet->offLine
             << " state = "
             << packet->state
             << " label_ip = "
             << ui->label_ip->text()
             << " packet_ip = "
             << packet->ip->get();*/
    static int count = 0;

    if(packet && ui->label_ip->text() == packet->ip->get())
    {
        if(packet->offLine <= 0)
        {
            count++;
            if(count < 5)
                return;
            count = 0;
            ui->label__state->setText(tr("离线"));
        }
        else if(packet->state==0)
        {
            count = 0;
            ui->label__state->setText(tr("在线"));
        }
        else
        {
            count = 0;
            ui->label__state->setText(tr("告警"));
        }
    }
}

void DialogChannelManager::on_checkBox_clever_clicked()
{
    if((false == ui->checkBox_zebra->isChecked()) &&
       (true == ui->checkBox_snmp->isChecked()) &&
       (true == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("不能同时选择SNMP通道和TCP通道！"));
        ui->checkBox_snmp->setChecked(false);
        ui->checkBox_clever->setChecked(false);
        return ;
    }
    else if((true == ui->checkBox_zebra->isChecked()) &&
            (true == ui->checkBox_snmp->isChecked()) &&
            (true == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("不能同时选择三个通道！"));
        ui->checkBox_zebra->setChecked(false);
        ui->checkBox_snmp->setChecked(false);
        ui->checkBox_clever->setChecked(false);
        return ;
    }
    else if((false == ui->checkBox_zebra->isChecked()) &&
            (false == ui->checkBox_snmp->isChecked()) &&
            (false == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("请至少选择一个通道，否则系统将默认自动选择TCP通道！"));
        return ;
    }
    else
    {
//        qDebug() << "[MYDEBUG : INFO] - DialogChannelManager::on_checkBox_clever_clicked() channel select ok" ;

        return ;
    }
}

void DialogChannelManager::on_checkBox_snmp_clicked()
{
    if((false == ui->checkBox_zebra->isChecked()) &&
       (true == ui->checkBox_snmp->isChecked()) &&
       (true == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("不能同时选择SNMP通道和TCP通道！"));
        ui->checkBox_snmp->setChecked(false);
        ui->checkBox_clever->setChecked(false);
        return ;
    }
    else if((true == ui->checkBox_zebra->isChecked()) &&
            (true == ui->checkBox_snmp->isChecked()) &&
            (true == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("不能同时选择三个通道！"));
        ui->checkBox_zebra->setChecked(false);
        ui->checkBox_snmp->setChecked(false);
        ui->checkBox_clever->setChecked(false);
        return ;
    }
    else if((false == ui->checkBox_zebra->isChecked()) &&
            (false == ui->checkBox_snmp->isChecked()) &&
            (false == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("请至少选择一个通道，否则系统将默认自动选择TCP通道！"));
        return ;
    }
    else
    {
//        qDebug() << "[MYDEBUG : INFO] - DialogChannelManager::on_checkBox_snmp_clicked() channel select ok" ;

        return ;
    }
}

void DialogChannelManager::on_checkBox_zebra_clicked()
{
    if((false == ui->checkBox_zebra->isChecked()) &&
       (true == ui->checkBox_snmp->isChecked()) &&
       (true == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("不能同时选择SNMP通道和TCP通道！"));
        ui->checkBox_snmp->setChecked(false);
        ui->checkBox_clever->setChecked(false);
        return ;
    }
    else if((true == ui->checkBox_zebra->isChecked()) &&
            (true == ui->checkBox_snmp->isChecked()) &&
            (true == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("不能同时选择三个通道！"));
        ui->checkBox_zebra->setChecked(false);
        ui->checkBox_snmp->setChecked(false);
        ui->checkBox_clever->setChecked(false);
        return ;
    }
    else if((false == ui->checkBox_zebra->isChecked()) &&
            (false == ui->checkBox_snmp->isChecked()) &&
            (false == ui->checkBox_clever->isChecked()))
    {
        CriticalMsgBox box(this,tr("请至少选择一个通道，否则系统将默认自动选择TCP通道！"));
        return ;
    }
    else
    {
//        qDebug() << "[MYDEBUG : INFO] - DialogChannelManager::on_checkBox_zebra_clicked() channel select ok" ;

        return ;
    }
}

void DialogChannelManager::update_zpdu_channel_state(QString ip, int set_chennel, int trap_chennel)
{
#if defined(ZEBRA_MODULE)
    if(0 == QString::compare(ip, ui->label_ip->text()))
    {
        zpdu_set_channel = set_chennel;
        zpdu_trap_channel = trap_chennel;

        if(set_chennel == TCP_ZEBRA_SET_CHANNEL)
        {
            ui->zpdu_state_label->setText("TCP + ZEBRA");
        }
        else if(set_chennel == ZEBRA_SET_CHANNE)
        {
            ui->zpdu_state_label->setText("ZEBRA");
        }
        else if(set_chennel == TCP_SET_CHANNEL)
        {
            ui->zpdu_state_label->setText("TCP");
        }
        else
        {
            MY_DEBUG(2, YELLOW) << "set_chennel = " << set_chennel << DEBUG_END;
        }

        if(trap_chennel == ZEBRA_TRAP_CHANNEL)
        {
            ui->zpdu_trap_label->setText("ZEBRA");
        }
        else if(trap_chennel == TCP_TRAP_CHANNEL)
        {
            ui->zpdu_trap_label->setText("TCP");
        }
        else
        {
            MY_DEBUG(2, YELLOW) << "trap_chennel = " << trap_chennel << DEBUG_END;
        }
    }
#endif
    return ;
}
