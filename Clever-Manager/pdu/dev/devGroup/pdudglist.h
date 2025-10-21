#ifndef PDUDGLIST_H
#define PDUDGLIST_H
#include "pdudghash.h"

class PduDGList : public PduDgHash
{
public:
    PduDGList();
    ~PduDGList();

    void startListGroup(int type);
    void startLineGroup(int type);
    void startAlarmGroup(int type);
    void startOffLineGroup(int type);

protected:
    void filter(QStringList &ipList);

private:
    PduDevList *mDevList;
};

#endif // PDUDGLIST_H
