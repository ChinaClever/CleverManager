#ifndef SER_SERVICDLG_H
#define SER_SERVICDLG_H

#include <QSplitter>
#include "usr/usrmanagedlg.h"
#include "services/sim900a/sim900a_interface.h"
#include "services/smtp/email_testdlg.h"
#include "services/dingtalk/dt_mainwid.h"

class Ser_ServiceDlg : public QWidget
{
    Q_OBJECT
public:
    explicit Ser_ServiceDlg(QWidget *parent = 0);
    ~Ser_ServiceDlg();

protected:
    void initWidget(void);
    void initLayout(void);
    void initTitle();

private:
    void initLeftSpl(void);
    void initRightSpl(void);

signals:

public slots:
    void indexChangeDone(int str);

private:
    QSplitter *mainSplitter;
    QSplitter *leftSplitter;
    QSplitter *rightSplitter;


private:
    UsrManageDlg *m_usrDlg;
    SIM900A_Interface *m_simDlg;    
    SIM900A_SerialDlg *m_serialDlg;
    Email_TestDlg *m_emil;
    DT_MainWid *m_dingTalk;

    QWidget *mWidget;
    QComboBox *combox;
};

extern int getTime();
#endif // SER_SERVICDLG_H
