#ifndef LOG_DATETIMEBTN_H
#define LOG_DATETIMEBTN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include<QDateEdit>
#include<QTimeEdit>
#include "common.h"

class LOG_DateTimeBtn : public QWidget
{
    Q_OBJECT
public:
    explicit LOG_DateTimeBtn(QWidget *parent = 0);
    ~LOG_DateTimeBtn();

    void initDTWdiget(void);
    void initDTLayout(void);

    void initDate(void);
    void initTime(void);

signals:
    void refreshSig();
    void delBtnSig();
    void querySig(QString);

public slots:
    void dateBtnSlot(void);
    void timeBtnSlot(void);

public:
    QHBoxLayout *hLayout;
    QPushButton *refreshBtn;
    QPushButton *delBtn;

    QLabel *dateLab;
//    QLineEdit *dateEdit;
     QDateEdit *dateEdit;
    QPushButton *dateBtn;

    QLabel *timeLab;
//    QLineEdit *timeEdit;
    QTimeEdit *timeEdit;
    QPushButton *timeBtn;

};

#endif // LOG_DATETIMEBTN_H
