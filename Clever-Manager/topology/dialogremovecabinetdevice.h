#ifndef DIALOGREMOVECABINETDEVICE_H
#define DIALOGREMOVECABINETDEVICE_H

#include <QDialog>

namespace Ui {
class DialogRemoveCabinetDevice;
}

class DialogRemoveCabinetDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRemoveCabinetDevice(QWidget *parent = 0);
    ~DialogRemoveCabinetDevice();

private:
    Ui::DialogRemoveCabinetDevice *ui;
};

#endif // DIALOGREMOVECABINETDEVICE_H
