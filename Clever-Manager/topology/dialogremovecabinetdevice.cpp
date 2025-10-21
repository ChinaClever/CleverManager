#include "dialogremovecabinetdevice.h"
#include "ui_dialogremovecabinetdevice.h"

DialogRemoveCabinetDevice::DialogRemoveCabinetDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRemoveCabinetDevice)
{
    ui->setupUi(this);
}

DialogRemoveCabinetDevice::~DialogRemoveCabinetDevice()
{
    delete ui;
}
