#ifndef ADDFRIENDTHR_H
#define ADDFRIENDTHR_H

#include <QObject>
#include <QThread>

class AddFriendThr : public QThread
{
    Q_OBJECT
signals:
    void add_friend_result(bool);

public:
    AddFriendThr(const QString invite_code, const QString friend_name);

private:
    void run();
    QString m_code;
    QString m_name;
};

#endif // ADDFRIENDTHR_H
