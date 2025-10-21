#ifndef SIM900A_SERIALDLG_H
#define SIM900A_SERIALDLG_H


#include <QGroupBox>
#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QGridLayout>

#include "common.h"
#include "common/msgBox/msgbox.h"
#include "sim900a_serial.h"

class SIM900A_SerialDlg : public QWidget
{
    Q_OBJECT
public:
    explicit SIM900A_SerialDlg(QWidget *parent = 0);
    ~SIM900A_SerialDlg();

protected:
    void initWidget(void);
    void initLayout(void);
    void initSerialPort(void);

signals:

public slots:
    void serialPortChanged(QString com);
    void workBtnSlot(void);
    void updateBtnSlot(void);
    void openSerialSlot(void);

private:
    QFont ft;
    QLabel *selectLab;
    QComboBox *comBox;

    QLabel *workLab;
    QPushButton *workBtn;

    QLabel *updateLab;
    QPushButton *updateBtn;

    QLabel *lab;
    QLabel *stateLab;

};

#endif // SIM900A_SERIALDLG_H
