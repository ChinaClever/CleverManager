#ifndef DBTGROOMTHREAD_H
#define DBTGROOMTHREAD_H

#include <QThread>

class DbTgRoomThread : public QThread
{
    Q_OBJECT
public:
    explicit DbTgRoomThread(QObject *parent = 0);

protected:
    void saveTgData(void);

signals:

public slots:
};

#endif // DBTGROOMTHREAD_H
