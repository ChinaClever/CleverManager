#ifndef PDUDGHASH_H
#define PDUDGHASH_H
#include "pdudevlist.h"

typedef QMultiHash<QString,QString> dg_groupHash;

class PduDgHash
{
public:
    PduDgHash();
    ~PduDgHash();

    int listGroup(QStringList &name);
    int listDev(QString &group,QStringList &devList);
    int listAllDev(QStringList &list);

    bool findGroup(QString &ip,QString &group);
    bool containsDev(QString &ip);
    bool containsDev(QString &group, QString &ip);
    bool containsGroup(QString &group);

    bool renameGroup(QString &oldName,QString &newName);
    void moveDev(QString &ip, QString &group);

    void insertGroup(QString &group);
    void add(QString &group, QString &ip);    
    void del(QString &group, QString &ip);

    void delDev(QString &ip);
    void delGroup(QString &group);

    void clear(void);

protected:
    void startGroup(int type);

private:
    QReadWriteLock *mLock;
    dg_groupHash *mGroupHash;
};

#endif // PDUDGHASH_H
