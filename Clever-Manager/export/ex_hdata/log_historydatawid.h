#ifndef LOG_HISTORYDATAWID_H
#define LOG_HISTORYDATAWID_H

#include <QWidget>
#include "ex_common/log_exportbarwid.h"
#include "log_pduparamsview.h"

namespace Ui {
class Log_HistoryDataWid;
}

class Log_HistoryDataWid : public QWidget
{
    Q_OBJECT

public:
    explicit Log_HistoryDataWid(QWidget *parent = 0);
    ~Log_HistoryDataWid();

protected slots:
    void findSLot(int id);
    void exportSLot(int);
    void clearTableSlot();
    int getRow(const QString &name);

private:
    Ui::Log_HistoryDataWid *ui;
    Log_ExportBarWid *mBarWid;
    QString mSqlCmd;
    Log_PduDevTabView *mPduDevTabView;
    Log_PduParamsVIew *mPduParamsView;
};

#endif // LOG_HISTORYDATAWID_H
