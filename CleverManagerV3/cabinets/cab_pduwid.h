#ifndef WIDGETPDUDEVICE_H
#define WIDGETPDUDEVICE_H

#include <QWidget>
#include <QSignalMapper>
#include "cab_portwid.h"
#include "datapacket.h"

class QPushButton;

namespace Ui {
class Cab_PduWid;
}

class Cab_PduWid : public QWidget
{
    Q_OBJECT
public:
    explicit Cab_PduWid(QWidget *parent = 0);
    ~Cab_PduWid();
    void setAlignType(bool left);
    void updatePacket(sDataPacket *packet);

    int  portsCount();
    Cab_PortWid* port(int index);
    void setEditable(bool b);
    void setTitle(bool is_left);

protected slots:
    void buttonClick(bool checked);
    void onSwitchBtnClick(); //切换按钮L1-L6
    void timeoutDone();
    void updateLine(sDataPacket *packet);
    void updateOutput(sDataPacket *packet);

protected:
    void setIpAddr(const QString& ip, const QString &num);
    void setPortsCount(int count);
    void setZero(QLabel *label);
    void setZeroOffine();
signals:
    void buttonClicked(int index,bool checked);

protected:
    Ui::Cab_PduWid *ui;
    QWidget* m_portWidget;
    QSignalMapper m_sigMapper;
    bool m_bLeft;
    int mLPage;
    sDataPacket *mPacket;
    QTimer *mTimer;
};

#endif // WIDGETPDUDEVICE_H
