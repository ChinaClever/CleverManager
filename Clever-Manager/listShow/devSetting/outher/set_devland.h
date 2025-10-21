<<<<<<< HEAD:Clever-Manager/listShow/devSetting/outher/set_devland.h
﻿#ifndef SET_DEVLAND_H
#define SET_DEVLAND_H

#include "set_devnet.h"

class SET_DevLand : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevLand(QWidget *parent = 0);
    ~SET_DevLand();

    void updateIp(QString &);
    void updateDev(QStringList &);

protected:
    bool initConfigFile(void);
    void initWidget(void);
    void initLayout(void);

    bool setLand(void);

signals:

public slots:
    void buttonClicked(void);

private:
    QStringList m_ip;
    QString m_usr,m_pwd;

    QLabel *titleLab;
    QLabel *usrLab,*pwdLab;
    QLineEdit *usrEdit, *pwdEdit;
    QPushButton *btn;
};

#endif // SET_DEVLAND_H
=======
﻿#ifndef SET_DEVLAND_H
#define SET_DEVLAND_H

#include "set_devnet.h"

class SET_DevLand : public QWidget
{
    Q_OBJECT
public:
    explicit SET_DevLand(QWidget *parent = 0);
    ~SET_DevLand();

    void updateIp(QString &);
    void updateDev(QStringList &);

protected:
    bool initConfigFile(void);
    void initWidget(void);
    void initLayout(void);

    bool setLand(void);

signals:

public slots:
    void buttonClicked(void);

private:
    QStringList m_ip;
    QString m_usr,m_pwd;

    QLabel *titleLab;
    QLabel *usrLab,*pwdLab;
    QLineEdit *usrEdit, *pwdEdit;
    QPushButton *btn;
};

#endif // SET_DEVLAND_H
>>>>>>> develop_beijing:Clever-Manager/listShow/devSetting/set_devland.h
