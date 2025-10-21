#ifndef CAB_MAINWID_H
#define CAB_MAINWID_H

#include "cab_treewid.h"
#include "cab_cabinetwid.h"

namespace Ui {
class Cab_MainWid;
}

class Cab_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Cab_MainWid(QWidget *parent = 0);
    ~Cab_MainWid();

signals:
    void selectedSig(int);

private:
    Ui::Cab_MainWid *ui;

    Cab_TreeWid *mTree;
    Cab_CabinetWid *mCab;
};

#endif // CAB_MAINWID_H
