#ifndef SETUP_PDUSTABLEWID_H
#define SETUP_PDUSTABLEWID_H

#include "setup_roomquerydlg.h"

namespace Ui {
class Setup_PdusTableWid;
}

class Setup_PdusTableWid : public QWidget
{
    Q_OBJECT

public:
    explicit Setup_PdusTableWid(QWidget *parent = 0);
    ~Setup_PdusTableWid();
    void initWid(BasicSql *db,  SqlQueryBase *btn);

private:
    Ui::Setup_PdusTableWid *ui;
    SqlTableWid *mSqlTableWid;
};

#endif // SETUP_PDUSTABLEWID_H
