<<<<<<< HEAD:Clever-Manager/listShow/devSetting/outher/set_devmode.h
﻿#ifndef SET_DEVMODE_H
#define SET_DEVMODE_H

#include <QComboBox>
#include "set_devemail.h"

class SET_DevMode : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevMode(QWidget *parent = 0);
    ~SET_DevMode();

    void updateIp(QString &);
    void updateDev(QStringList &);

protected:
    bool initConfigFile(void);
    void initWidget(void);
    void initLayout(void);
    bool setMode(char);

signals:

public slots:
    void buttonClicked(void);

private:
    char m_mode;
    QStringList m_ip;

    QLabel *lab;
    QComboBox *comBox;
    QPushButton *btn;
    QGroupBox *groupBox;
};

#endif // SET_DEVMODE_H
=======
﻿#ifndef SET_DEVMODE_H
#define SET_DEVMODE_H

#include <QComboBox>
#include "set_devemail.h"

class SET_DevMode : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevMode(QWidget *parent = 0);
    ~SET_DevMode();

    void updateIp(QString &);
    void updateDev(QStringList &);

protected:
    bool initConfigFile(void);
    void initWidget(void);
    void initLayout(void);
    bool setMode(char);

signals:

public slots:
    void buttonClicked(void);

private:
    char m_mode;
    QStringList m_ip;

    QLabel *lab;
    QComboBox *comBox;
    QPushButton *btn;
    QGroupBox *groupBox;
};

#endif // SET_DEVMODE_H
>>>>>>> develop_beijing:Clever-Manager/listShow/devSetting/set_devmode.h
