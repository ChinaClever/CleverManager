#ifndef PDUDEVADDRTHREAD_H
#define PDUDEVADDRTHREAD_H

#include <QObject>
#include "dbroomlist.h"
#include "dbcabinetlist.h"

class PduDevAddrThread : public QThread
{
    Q_OBJECT
public:
    explicit PduDevAddrThread(QObject *parent = 0);

protected:
    void run(void);
    void clearAllPduAddr(void);
    void setPduAddr(void);
    void setPduAddr(const QString &room,const CabinetItem& cItem);
    void setPduAddr(int type, const QString &ip, int num, const QString &room, const QString& cab);
signals:

public slots:
    void initFunSLot();
    void itemChangedSlot(int,int);

private:
    bool isRun;
};

#endif // PDUDEVADDRTHREAD_H
