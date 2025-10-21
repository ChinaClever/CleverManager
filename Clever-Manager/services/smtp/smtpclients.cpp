#include "smtpclients.h"
#include <QHostInfo>

SmtpClients::SmtpClients(QObject *parent) : QObject(parent)
{
    smtp = NULL;
}


void SmtpClients::setSmtp(const QString &user, const QString &pass, const QString &host, bool ssl, int port)
{
    if(ssl) {
        smtp = new SmtpClient(host, port, SmtpClient::TlsConnection);
    } else {
        smtp = new SmtpClient(host, port, SmtpClient::TcpConnection);
    }

    if(smtp!=NULL)
    {
        smtp->setName(QHostInfo::localHostName());
        smtp->setUser(user);
        smtp->setPassword(pass);        
        smtp->setAuthMethod(SmtpClient::AuthLogin);
    }
}


SmtpClients::~SmtpClients()
{
}

int SmtpClients::sendMail( const QString &from, const QStringList &to,
                           const QString &subject, const QString &body )
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
    }
    else
    {
        qDebug() << "send mail!__________________________________________" << endl;
    }

    smtp->quit();

    if(smtp!=NULL)
    {
        delete smtp;
        smtp = NULL;
    }
    return 0;
}
