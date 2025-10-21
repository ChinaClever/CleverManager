#ifndef EDITUSRDLG_H
#define EDITUSRDLG_H
#include "newusrdlg.h"


class EditUsrDlg : public NewUsrDlg
{
    Q_OBJECT

public:
    EditUsrDlg(QWidget *parent = 0);
    ~EditUsrDlg();

    bool setUsrName(QString &);

protected:
     bool saveUsrInfo(UsrStruct *);

private:
     UsrClass *mUsr;
};

#endif // EDITUSRDLG_H
