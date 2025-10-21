#ifndef LS_RIGHTSPL_H
#define LS_RIGHTSPL_H

#include "ls_listtabledlg.h"
#include "dataShow/ds_devwidget.h"
#include "output/ui_outputwidget.h"
#include "env/ui_envtable.h"
#include "net/tcp/client/tcpsent.h"
#include "devSetting/setdevmainwid.h"
#include "ls_rightheadwid.h"
#include"listShow/loop/loopwidget.h"

class LS_RightSpl : public QWidget
{
    Q_OBJECT
public:
    explicit LS_RightSpl(QWidget *parent = 0);


protected:
    void initGroupBox(void);
    void initWidget(void);

signals:
    void selectSig(QString &ip,int num);

public slots:
    void selectSlot(QString &ip,int num);

private:
    LS_RightHeadWid *mHeadWid;
    QTabWidget *mTabWidget;
    QGridLayout *m_layout;

    DS_DevWidget *mDevWidget;
    UI_OutputWidget *mOutput;
    UI_EnvTable *mEnvTable;
    SetDevMainWid *mSetDev;
    UI_LoopWidget *mLoopWidget;

    TcpSent *mTcp;
};

#endif // LS_RIGHTSPL_H
