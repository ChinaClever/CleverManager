#ifndef AUTOADDFRIEND_H
#define AUTOADDFRIEND_H

#include <QVariant>
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include <QThread>

#include <stdint.h>
#include <list>

const int32_t c_short_code_sock_port = 21010;

/***********************************************************************************
* Function describe：用于自动获取加好友所需的PDU的短码
* Matters attention：
* Input parameter：
* Output parameter：
* Author：zjm : )
* Creation time：
* Modify log：zjm => 2018-07-03 => add describe
***********************************************************************************/
class ShortCodeSocket : public QThread
{
    Q_OBJECT

public:
    bool distory_flag;

    ShortCodeSocket(QString zpdu_ip);
    ~ShortCodeSocket();

private slots:
    void read_message();

private:
    QString m_zpdu_ip;
    QString mShort_code;
    QTcpSocket* m_shortcode_socket;
    void run();

};

/***********************************************************************************
* Function describe：用于自动加好友
* Matters attention：
* Input parameter：
* Output parameter：
* Author：zjm : )
* Creation time：
* Modify log：zjm => 2018-07-03 => add describe
***********************************************************************************/
class AutoAddFriend : public QObject
{
    Q_OBJECT
    AutoAddFriend();
public:
    ~AutoAddFriend();
    static AutoAddFriend* get_instance();
    int addFriend(const QString &ip);
    bool check_exist(const QString &ip);

private slots:
    void timeout_to_free_socket();

private:
    static AutoAddFriend* m_auto_addfiend;
    QMutex m_mutex;
    QTimer* m_shortcode_timer;
    std::list<ShortCodeSocket*> m_shordcode_sock;
    QString mIp;

};



#endif //AUTOADDFRIEND_H


