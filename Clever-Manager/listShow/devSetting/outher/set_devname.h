<<<<<<< HEAD:Clever-Manager/listShow/devSetting/outher/set_devname.h
﻿#ifndef SET_DEVNAME_H
#define SET_DEVNAME_H

#include <QTableWidget>
#include <QGridLayout>
#include <QInputDialog>
#include "set_cmdclass.h"
#include "dl_devlist.h"
#include "msgbox.h"
#include <QLabel>
#include <QGroupBox>
#include "db_sqlquery.h"

class SET_DevName : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevName(QWidget *parent = 0);
    ~SET_DevName();

    void updateSlave(QString &,short);
    void updateIp(QString &);
    void updateGroup(QString &);
    void updateWidget(void);
    void clearWidget(void);

protected:
    void initLayout(void);
    void initWidget(void);
    void addItemContent(int,int,QString &);
    void addRowContent(QStringList &);
    QString setDevName(QString &,QString &,QString &);
    short getDevNum(QString &);

signals:

public slots:
    void getItemSlot(QTableWidgetItem *);

private:
    DL_DevList m_devList;
    QTableWidget *m_tableWidget;
};

#endif // SET_DEVNAME_H
=======
﻿#ifndef SET_DEVNAME_H
#define SET_DEVNAME_H

#include <QTableWidget>
#include <QGridLayout>
#include <QInputDialog>
#include "set_cmdclass.h"
#include "dl_devlist.h"
#include "msgbox.h"
#include <QLabel>
#include <QGroupBox>
#include "db_sqlquery.h"

class SET_DevName : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevName(QWidget *parent = 0);
    ~SET_DevName();

    void updateSlave(QString &,short);
    void updateIp(QString &);
    void updateGroup(QString &);
    void updateWidget(void);
    void clearWidget(void);

protected:
    void initLayout(void);
    void initWidget(void);
    void addItemContent(int,int,QString &);
    void addRowContent(QStringList &);
    QString setDevName(QString &,QString &,QString &);
    short getDevNum(QString &);

signals:

public slots:
    void getItemSlot(QTableWidgetItem *);

private:
    DL_DevList m_devList;
    QTableWidget *m_tableWidget;
};

#endif // SET_DEVNAME_H
>>>>>>> develop_beijing:Clever-Manager/listShow/devSetting/set_devname.h
