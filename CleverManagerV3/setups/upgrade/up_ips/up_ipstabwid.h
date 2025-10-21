#ifndef IPSTABWID_H
#define IPSTABWID_H

#include "comtablewid.h"
#include "up_datapacket.h"

class Up_IpsTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Up_IpsTabWid(QWidget *parent = nullptr);

protected slots:
   virtual void timeoutDone();

protected:
    virtual void initWid();
    virtual void updateData(QStringList &lst);

protected:
    Up_sDataPacket *mData;
};

#endif // IPSTABWID_H
