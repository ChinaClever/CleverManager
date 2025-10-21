#ifndef DIALOGADDCABINETDEVICE_H
#define DIALOGADDCABINETDEVICE_H

#include <QDialog>
#include "dbconsumerdevices.h"

namespace Ui {
class DialogAddCabinetDevice;
}

class DialogAddCabinetDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddCabinetDevice(ConsumerDeviceItem& item,int left,int right,int height,QWidget *parent = 0);
    ~DialogAddCabinetDevice();
public slots:
    void accept();
protected:
    Ui::DialogAddCabinetDevice *ui;
    ConsumerDeviceItem& m_item;
private slots:
    void on_spinBox_pos_valueChanged(int arg1);
};

class DialogModifyCabinetDevice : public DialogAddCabinetDevice
{
    Q_OBJECT
public:
    explicit DialogModifyCabinetDevice(ConsumerDeviceItem& item,int left,int right,int height,QWidget *parent = 0);
};

#endif // DIALOGADDCABINETDEVICE_H
