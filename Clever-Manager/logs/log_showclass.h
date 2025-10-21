#ifndef LOG_SHOWCLASS_H
#define LOG_SHOWCLASS_H

#include <QGroupBox>
#include <QScrollArea>
#include <QHBoxLayout>
#include "log_tablewidget.h"
#include "export/elec_export/log_elewid.h"
#include "export/ex_hdata/log_historydatawid.h"
#include "log_headwid.h"
#include "qcharts/diagramwid.h"
#include "log_historywid.h"

class LOG_ShowClass : public QWidget
{
    Q_OBJECT
public:
    explicit LOG_ShowClass(QWidget *parent = 0);
    ~LOG_ShowClass();

protected:
    void initAlarmTable(void); //初始化报警表
    void initRecordTable(void); //初始化记录表
    void initOffLineTable(void); //初始化脱机表
    void initLandTable(void);  //
    void initRoomTable(void);  //初始化机房表
    void initCabTable(void);

    void initEleWid();
    void initDevWid();
    void initHistoryWid();

private:
    void initLayout(void);
    void initTitle();


signals:

protected slots:
    void initFunSLot();

private:
    LOG_HeadWid *mHeadWid;
    QTabWidget *tabWidget;

    int index;
    LOG_TableWidget *alarmTable;
    LOG_TableWidget *offLineTable;
    LOG_TableWidget *recordTable;
    LOG_TableWidget *landTable;
    LOG_TableWidget *roomTable;
    LOG_TableWidget *cabTable;
    Log_EleWid * eleWid;
    Log_HistoryDataWid *devWid;
    QWidget *mWidget;
};

#endif // LOG_SHOWCLASS_H
