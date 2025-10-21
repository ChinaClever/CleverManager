#ifndef DS_DATASHOW_H
#define DS_DATASHOW_H

#include "ds_linebtn.h"
#include "ds_slavebtn.h"
#include "ds_webbtn.h"

class DS_DataShow : public QWidget
{
    Q_OBJECT
public:
    explicit DS_DataShow(QWidget *parent = 0);
    ~DS_DataShow();

    void devAlarmInfo(DS_DsyDev &, QString &);

public slots:
     void updateData(QString &, int);
     void updateData(void);

signals:
     void selectDevSig(DS_DsyDev &);
     void updateSig(void);

protected slots:
    void slaveBtnSlot(int);
    void lineBtnSlot(int);
    void timeoutDone(void);

protected:
     bool checkAlarm(void);

private:
    void initFunction(void);
    void createWidget(void);
    void initLayout(void);
    void initClass(void);
    void createLayout(void);

public:
    DS_DataDsplay *m_dataDsy;
    DS_LineBtn *m_lineBtn;
    DS_SlaveBtn *m_slaveBtn;
    DS_WebBtn *m_webBtn;

    QGroupBox *groupBox;
    QWidget *mainWidget;
    QWidget *lineWidget;
    QWidget *slaveWidget;
    QWidget *webWidget;
    QWidget *my_widget;

    QTimer *timer;
    DS_DsyDev m_dsyDeV;
};

#endif // DS_DATASHOW_H
