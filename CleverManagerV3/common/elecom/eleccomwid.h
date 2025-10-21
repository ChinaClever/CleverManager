#ifndef ELECCOMWID_H
#define ELECCOMWID_H

#include "elecbtnbar.h"

namespace Ui {
class ElecComWid;
}

class ElecComWid : public QWidget
{
    Q_OBJECT

public:
    explicit ElecComWid(QWidget *parent = 0);
    ~ElecComWid();
    void initWid(BasicSql *db,  SqlQueryBase *btn);

private:
    Ui::ElecComWid *ui;
    SqlTableWid *mSqlTableWid;
};

#endif // ELECCOMWID_H
