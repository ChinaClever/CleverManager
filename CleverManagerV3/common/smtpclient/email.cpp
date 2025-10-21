/*
 * email.cpp
 *  发送邮件
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "email.h"
#include <QHostInfo>

Email::Email(QObject *parent) : QObject(parent)
{
    mSet = email_get_setInfo();
}

Email::~Email()
{

}


int Email::sendMail(const QString &from, const QStringList &to, const QString &subject, const QString &body )
{
    MimeMessage message;

    EmailAddress sender(from);
    message.setSender(&sender);

    for(int i=0; i<to.size(); ++i)
        message.addRecipient(new EmailAddress(to.at(i)));
    message.setSubject(subject);

    MimeText text;
    text.setText(body);
    message.addPart(&text);

    if (!smtp->connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
        return -1;
    }

    if (!smtp->login()) {
        qDebug() << "Failed to login!" << endl;
        return -2;
    }

    if (!smtp->sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;
        return -3;
    } else {
        qDebug() << "send mail!" << endl;
    }

    smtp->quit();
    if(smtp!=NULL) {
        delete smtp;
        smtp = NULL;
    }

    return 0;
}


void Email::setSmtp()
{
    smtp = new SmtpClient;
    smtp->setHost(mSet->server);
    smtp->setPort(mSet->port);
    smtp->setName(QHostInfo::localHostName());
    smtp->setUser(mSet->usr);
    smtp->setPassword(mSet->pwd);
    smtp->setAuthMethod(SmtpClient::AuthLogin);

    switch (mSet->ssl) {
    case 0: smtp->setConnectionType(SmtpClient::TcpConnection); break;
    case 1: smtp->setConnectionType(SmtpClient::SslConnection); break;
    case 2: smtp->setConnectionType(SmtpClient::TlsConnection); break;
    }
}


bool Email::sentEmail(QStringList &recipient,QString &subject,QString &body)
{
    bool res = false;
    if(mSet)
    {
        setSmtp();
        int ret = sendMail(mSet->usr, recipient, subject,body);
        if(0 == ret) { // 发送失败
            res = true;
        }
        emit finishedSig(ret);
    }

    return res;
}



