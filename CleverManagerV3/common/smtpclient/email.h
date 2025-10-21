#ifndef EMAIL_H
#define EMAIL_H

#include "email_setdlg.h"
#include "SmtpMime.h"

class Email : public QObject
{
    Q_OBJECT
public:
    explicit Email(QObject *parent = 0);
    ~Email();

    bool sentEmail(QStringList &, QString &, QString &);

protected:
    void setSmtp();
    int sendMail( const QString &from, const QStringList &to,
                  const QString &subject, const QString &body );

signals:
    void finishedSig(int ret);

public slots:

private:
    SmtpClient *smtp;
    email_setInfo *mSet;
};

extern void email_sent_message(QString &subject, QString &body);
extern int usr_email_list(QStringList &email);
extern email_setInfo *email_get_setInfo(void);
#endif // EMAIL_H
