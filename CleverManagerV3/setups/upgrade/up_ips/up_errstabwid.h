#ifndef ERRSTABWID_H
#define ERRSTABWID_H

#include "up_ipstabwid.h"

class Up_ErrsTabWid : public Up_IpsTabWid
{
    Q_OBJECT
public:
    explicit Up_ErrsTabWid(QWidget *parent = nullptr);

protected slots:
   virtual void timeoutDone();

protected:
    virtual void initWid();
};

#endif // ERRSTABWID_H
