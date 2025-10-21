#ifndef SIGNALDISPATCH_H
#define SIGNALDISPATCH_H

#include <QObject>

#include "autoaddfriend/zebrasecuritymanager.h"

class PduDataPacket;

class SignalDispatch : public QObject
{
    Q_OBJECT
    explicit SignalDispatch(QObject *parent = 0);
public:
    static SignalDispatch* get();
    void dispatch(PduDataPacket* packet);
    void changeLoginState();
    void get_search_channel_key(QString channel_name);
    void update_zebra_security_list(std::map<QString, zebra_security_data> map_data);
    void update_zpdu_channel_state(QString dev_ip, int set_chennel, int trap_chennel);

signals:
    void dispatchChange(PduDataPacket* packet);
    void loginStateChange();
    void get_search_channel_key_sig(QString channel_name);
    void update_zebra_security_list_sig(std::map<QString, zebra_security_data> map_data);
    void update_zpdu_channel_state_sig(QString dev_ip, int set_chennel, int trap_chennel);

};

#endif // SIGNALDISPATCH_H
