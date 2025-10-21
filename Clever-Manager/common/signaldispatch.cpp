#include "signaldispatch.h"
#include <QDebug>

SignalDispatch::SignalDispatch(QObject *parent) : QObject(parent)
{

}

SignalDispatch *SignalDispatch::get()
{
    static SignalDispatch *sington = NULL;
    if(sington == NULL)
        sington= new SignalDispatch();
    return sington;
}

void SignalDispatch::dispatch(PduDataPacket *packet)
{
    emit dispatchChange(packet);
}

void SignalDispatch::changeLoginState()
{
    emit loginStateChange();
}

void SignalDispatch::get_search_channel_key(QString channel_name)
{
    emit get_search_channel_key_sig(channel_name);
}

void SignalDispatch::update_zebra_security_list(std::map<QString, zebra_security_data> map_data)
{
    emit update_zebra_security_list_sig(map_data);
}

void SignalDispatch::update_zpdu_channel_state(QString dev_ip, int set_chennel, int trap_chennel)
{
    emit update_zpdu_channel_state_sig(dev_ip, set_chennel, trap_chennel);
}
