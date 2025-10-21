#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tg_widgetmain.h"
#include "details/dt_mainwidget.h"
#include "listShow/ls_mainspl.h"
#include "logs/log_showclass.h"
#include "netdataanalyze.h"
#include "pdu/dev/pdudevspied.h"
#include "services/ser_servicdlg.h"
#include "topologywidget.h"
#include "usr/usrlanddlg.h"
#include "QProcess"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool num_out;//判断pdu数量是否超过管理上限


protected:
    void initWidget(void);
    void initLayout(void);
    void initFunction(void);

    void landWidget(void);
    void usrQuit(void);
    void enableBtn(bool);

    void setBtndefault(QToolButton *btn, const QString &str);
    void setBtnClicked(QToolButton *btn, const QString &str);

    void addToolBtnIcon(QToolButton *btn, const QString &icon);
    void initBtnColour();

    void initZebra();

    void closeEvent(QCloseEvent *event);

protected slots:
    void initFunSLot();
    void timeoutDone();
    void selectSlot(QString ip,short num);

private slots:
    void on_TGBtn_clicked();

    void on_usrBtn_clicked();

    void on_DetailBtn_clicked();

    void on_devBtn_clicked();

    void on_LogBtn_clicked();

    void on_serBtn_clicked();

    void on_topoBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    /* 网络数据解析对象、它分创建UDP接收套接字，广播心跳包，接收数据并进行解析，把数据保存至Hash中 */
    NetDataAnalyze *mNetDataAnalyze;
    PduDevSpied *mDevSpied; // 设备监视

    QListWidget *m_pListWidget;
    QStackedWidget *mStack;
    QTimer *timer;

    TG_WidgetMain *mTGShow;
    DT_MainWidget *mDetailList;
    LS_MainSpl *mLSMainSpl;

    LOG_ShowClass *mLog;
    Ser_ServiceDlg *mServiceDlg;

    TopologyWidget *mTopology;
    UsrLandDlg *mUserDlg;
    QProcess* mProcess;
};

#endif // MAINWINDOW_H
