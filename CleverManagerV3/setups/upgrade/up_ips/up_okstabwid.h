#ifndef OKSTABWID_H
#define OKSTABWID_H

#include "up_errstabwid.h"

class Up_OksTabWid : public Up_ErrsTabWid
{
    Q_OBJECT
public:
    explicit Up_OksTabWid(QWidget *parent = nullptr);

protected slots:
   virtual void timeoutDone();

protected:
    virtual void initWid();
};

#endif // OKSTABWID_H
