<<<<<<< HEAD:Clever-Manager/listShow/devSetting/outher/set_devdlg.h
﻿#ifndef SET_DEVDLG_H
#define SET_DEVDLG_H

#include <QGroupBox>
#include <QStackedWidget>
#include <QListWidget>
#include "set_devip.h"

class SET_DevDlg : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevDlg(QWidget *parent = 0);
    ~SET_DevDlg();

protected:
    void initFunction(void);
    void initWidget(void);
    void initLayout(void);

    void initDev(void);

signals:

public slots:
    void updateDev(QStringList &);
    void updateGroup(QString);
    void updateIp(QString);
    void selectSlot(QString,short);

private:
    SET_DevName *m_pDevName;
    SET_DevAlarm *m_pDevAlarm;
    SET_DevLand *m_pDevLand;
    SET_DevEmail *m_pDevEmail;
    SET_DevMode *m_pDevMode;
    SET_DevNet *m_pDevNet;
    SET_DevSNMP *m_PDevSNMP;
    SET_DevIP *m_pDevIP;
    SET_DevRes *m_pDevRes;

    QListWidget *m_pListWidget;
    QStackedWidget *m_pStack;
    QHBoxLayout *hLayout;
    QGridLayout *m_layout;
    QGroupBox *m_groupBox, *box;
};

#endif // SET_DEVDLG_H
=======
﻿#ifndef SET_DEVDLG_H
#define SET_DEVDLG_H

#include <QGroupBox>
#include <QStackedWidget>
#include <QListWidget>
#include "set_devip.h"

class SET_DevDlg : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevDlg(QWidget *parent = 0);
    ~SET_DevDlg();

protected:
    void initFunction(void);
    void initWidget(void);
    void initLayout(void);

    void initDev(void);

signals:

public slots:
    void updateDev(QStringList &);
    void updateGroup(QString);
    void updateIp(QString);
    void selectSlot(QString,short);

private:
    SET_DevName *m_pDevName;
    SET_DevAlarm *m_pDevAlarm;
    SET_DevLand *m_pDevLand;
    SET_DevEmail *m_pDevEmail;
    SET_DevMode *m_pDevMode;
    SET_DevNet *m_pDevNet;
    SET_DevSNMP *m_PDevSNMP;
    SET_DevIP *m_pDevIP;
    SET_DevRes *m_pDevRes;

    QListWidget *m_pListWidget;
    QStackedWidget *m_pStack;
    QHBoxLayout *hLayout;
    QGridLayout *m_layout;
    QGroupBox *m_groupBox, *box;
};

#endif // SET_DEVDLG_H
>>>>>>> develop_beijing:Clever-Manager/listShow/devSetting/set_devdlg.h
