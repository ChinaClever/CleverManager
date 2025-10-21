#ifndef LOG_UPDATETHREAD_H
#define LOG_UPDATETHREAD_H

#include <QThread>
#include "ex_common/log_exportbarwid.h"
#include "log_elechange.h"
#include "log_eletabview.h"
#include "export/elec_export/log_tgcabdb.h"

class Log_UpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit Log_UpdateThread(QObject *parent = 0);

    void setBar(Log_ExportBarWid *bar) {mBarWid = bar;}
    void setView(Log_EleTabView *view) {mTabView = view;}
    void startUpdate(int mode);

protected:
    void run();

     void getDbRoomEle(const QString &name);
     void findRoomEle();
     void getDbDevElc();
     void getDbCabEle();
     void getDbOneCabEle();

signals:
    void updateSig();

public slots:
    void updateSlot();

private:
    Log_ExportBarWid *mBarWid;
    Log_Elechange *mElechange;
    LOG_TgRoomDb *mTgRoomDb;
    LOG_PduDevDb *mPduDevDb;
    Log_EleTempTable *mTempTable;
    Log_EleTabView *mTabView;
    LOG_TgCabDb *mTgCabDb;

    int mode;
};

#endif // LOG_UPDATETHREAD_H
