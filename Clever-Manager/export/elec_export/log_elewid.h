#ifndef LOG_ELEWID_H
#define LOG_ELEWID_H

#include <QWidget>
#include "log_updatethread.h"
#include "log_pdudevdb.h"

namespace Ui {
class Log_EleWid;
}

class Log_EleWid : public QWidget
{
    Q_OBJECT

public:
    explicit Log_EleWid(QWidget *parent = 0);
    ~Log_EleWid();

protected:
    void initWidget();

protected slots:
    void findSLot(int);
    void exportSLot(int);

private:
    Ui::Log_EleWid *ui;
    Log_ExportBarWid *mBarWid;
    Log_EleTabView *mTabView;
    Log_UpdateThread *mUpdateThread;
};

#endif // LOG_ELEWID_H
