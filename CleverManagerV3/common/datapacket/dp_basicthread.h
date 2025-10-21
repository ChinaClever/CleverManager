#ifndef DP_BASICTHREAD_H
#define DP_BASICTHREAD_H
#include "basicsql.h"

template <typename T>
class Dp_BasicThread : public QThread
{
public:
    Dp_BasicThread(QObject *parent = nullptr): QThread(parent) {isRun=true; start();}
    ~Dp_BasicThread() { isRun = false; wait();}

protected:
    void run() {
        mdelay(1980);
        initFun();
        while (isRun) {
            mdelay(1000 + rand()%245);
            static QReadWriteLock lock;
            QWriteLocker locker(&lock);
            if(isRun) workDown();
            //qDebug() << mHash.size() <<mHash.keys();
        }
    }

    void mdelay(int d) {
        for(int i=0; i<d; ++i) {
            if(isRun)msleep(1);
        }
    }

    void workDown() {
        Db_Tran db;
        QHashIterator<QString, T> iter(mHash);
        while(iter.hasNext())
        {
            T pack = iter.next().value();
            if(pack) {
                if(pack->en) {
                    pack->count++;
                    workDown(pack);
                }
            }
        }
    }

    virtual void initFun()=0;
    virtual void workDown(T pack)=0;


protected:
    virtual T newDataPacket()=0;
    T insert(const QString &key) {
        T node = find(key);
        if(!node) {
            node = newDataPacket();
            mHash.insert(key, node);
        }

        return node;
    }

    T find(const QString &key) {
        T node = nullptr;
        bool ret = contains(key);
        if(ret) {
            auto index = mHash.find(key);
            if(index != mHash.end()) {
                node = index.value();
            }
        }
        return node;
    }

    void remove(const QString &key) {
        T node = find(key);
        if(node) {
            mHash.remove(key);
            delete node;
        }
    }

    bool contains(const QString &key) {return mHash.contains(key);}

protected:
    bool isRun;
    QHash<QString, T> mHash;
};

#endif // DP_BASICTHREAD_H
