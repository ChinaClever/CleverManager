#ifndef WIDGETPDUDEVICE_H
#define WIDGETPDUDEVICE_H

#include <QWidget>
#include <QSignalMapper>
#include "formpowerport.h"
#include "pdudatapacket.h"
#include"pdu/dev/devType/pdudtname.h"

class QPushButton;

namespace Ui {
class WidgetPduDevice;
}

class WidgetPduDevice : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetPduDevice(QWidget *parent = 0);
    ~WidgetPduDevice();
    void setAlignType(bool left);
    void updatePacket(PduDataPacket *packet);
    void setIpAddr(const QString& ip, int num);
    int  portsCount();
    FormPowerPort* port(int index);
    void setEditable(bool b);
    void setTitle(bool is_left);
protected slots:
    void buttonClick(bool checked);
    void onSwitchBtnClick(); //切换按钮L1-L6
protected:
    void setPortsCount(int count);
    void setZero(QLabel *label);
    void setZeroOffine();
signals:
    void buttonClicked(int index,bool checked);
    void buttonChannelManager_clicked();
protected:
    Ui::WidgetPduDevice *ui;
    QWidget* m_portWidget;
    QSignalMapper m_sigMapper;
    bool m_bLeft;
    int mLPage;
    PduDataPacket *mPacket;
};

#endif // WIDGETPDUDEVICE_H
