#ifndef TG_DATATHREAD_H
#define TG_DATATHREAD_H

#include <QObject>
#include "topologywidget.h"
#include "tg_datadsplay.h"
#include "tg_devnumgraph.h"
#include "storage/cloudroomtg.h"
#include "storage/cloudcabinettg.h"

typedef struct
{
    TG_DataType *data;
    pdu_devNumStr *num;
}sTGDataSt;

typedef struct
{
    QString ip;
    int num;
}sTGListSt;
Q_DECLARE_METATYPE(sTGListSt)

class TG_DataThread : public QThread
{
    Q_OBJECT
public:
    explicit TG_DataThread(QObject *parent = 0);
    ~TG_DataThread();

    sTGDataSt *set(int id);

protected:
    void run(void);
    void allTG(void);
    void clearList(void);
    void appendList(const CabinetItem &cItem, QList<sTGListSt *> &listSt);
    void readDataBase(int id);
    bool getCabinetById(int id);    

private:
    void initData(sTGDataSt *tgDataSt);
    void getTgData(QList<sTGListSt *> &listSt, sTGDataSt *tgDataSt);
    TgRoomItem getTgItem(int id, sTGDataSt *tgDataSt);
    void saveTgData(void);
//    int getTime(QString str);

    void cabinetTg(int flag);
    void cabinetTg(QString &room);
    void cabinetTg(const CabinetItem &cItem, TgCabItem &cabItem);

signals:

public slots:
    void timeoutDone();
    void dbTimeoutDone();
    void itemChangedSlot(int,int);

private:    
    int mID;
    bool isRun, isSlave;
    QReadWriteLock *mLock;
    int mTimeCount;

    PduDataTgThread *m_pTgThread;
    QList<sTGListSt *> mList;
    sTGDataSt *mTGDataSt;
    QTimer *timer;

    CloudModule *mModule;
    CloudRoomTg *mRoomTg;
    CloudCabinetTg *mCabinetTg;
};

#endif // TG_DATATHREAD_H
