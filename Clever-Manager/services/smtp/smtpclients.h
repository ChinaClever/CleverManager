#ifndef SMTPCLIENTS_H
#define SMTPCLIENTS_H

#include <QObject>
#include "SmtpClient/src/SmtpMime.h"

class SmtpClients : public QObject
{
    Q_OBJECT
public:
    explicit SmtpClients(QObject *parent = 0);
    ~SmtpClients();

    void setSmtp(const QString &user, const QString &pass,
                 const QString &host, bool ssl, int port = 465);

    int sendMail( const QString &from, const QStringList &to,
                   const QString &subject, const QString &body );
signals:

public slots:

private:
    SmtpClient *smtp;
};

#endif // SMTPCLIENTS_H
