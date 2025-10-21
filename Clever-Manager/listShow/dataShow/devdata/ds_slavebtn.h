#ifndef DS_SLAVEBTN_H
#define DS_SLAVEBTN_H

#include "ds_datadsplay.h"
#include "QComboBox"
#include "QPushButton"
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>

class DS_SlaveBtn : public QWidget
{
    Q_OBJECT
public:
    explicit DS_SlaveBtn(QWidget *parent = 0);
    ~DS_SlaveBtn();

signals:
    void btnClickedSig(int);

public slots:
     void updateBtn(QString &, int num);
     void comChangeSlot(int);

protected:
     int initBtn(QString &);
     void initComboBox(int);
     void allHidden(bool);

private:
     void initWidget(void);
     void initLayout(void);

private:
    QPushButton *masterBtn;
    QPushButton *slaveBtn_1;
    QPushButton *slaveBtn_2;
    QPushButton *slaveBtn_3;
    QPushButton *slaveBtn_4;
    QComboBox *comboBox;

    QSignalMapper *m;
    QLabel *lab;
};

#endif // DS_SLAVEBTN_H
