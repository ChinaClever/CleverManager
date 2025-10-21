#ifndef DIALOGCHANNELMANAGER_H
#define DIALOGCHANNELMANAGER_H

#include <QDialog>

namespace Ui {
class DialogChannelManager;
}

class DialogChannelManager : public QDialog
{
    Q_OBJECT
    void init();
public:
    explicit DialogChannelManager(const QString &ipAddr,int num);
    ~DialogChannelManager();

private slots:
    void on_pushButton_ok_clicked();
    void updatePacket(class PduDataPacket* packet);
    void on_checkBox_clever_clicked();

    void on_checkBox_snmp_clicked();

    void on_checkBox_zebra_clicked();

    void update_zpdu_channel_state(QString ip, int set_chennel, int trap_chennel);

private:
    Ui::DialogChannelManager *ui;
    int m_num;

#if defined(ZEBRA_MODULE)
    int zpdu_set_channel;
    int zpdu_trap_channel;
    int send_set_channel_info(int set_channel);
#endif
};

#endif // DIALOGCHANNELMANAGER_H
