#ifndef LOG_EXPORTBARWID_H
#define LOG_EXPORTBARWID_H

#include <QWidget>
#include "common.h"
#include"qcharts/diagram.h"
#include<qcharts/diascatter.h>
#include "qcharts/diagram.h"

namespace Ui {
class Log_ExportBarWid;
}
enum {
    LOG_EXPORT_DEV,  // PDU设备导出
    LOG_EXPORT_ROOM, // 机房电能导出
    LOG_EXPORT_CAB, // 机柜电能导出
    LOG_EXPORT_ALLBYROOM // 通过机房查询，按机房AB路导出PDU电能
};

class Log_ExportBarWid : public QWidget
{
    Q_OBJECT

public:
    explicit Log_ExportBarWid(QWidget *parent = 0);
    ~Log_ExportBarWid();

    void setDevMode(int index = 0);

protected:
    void initWidget();
    bool inputCheck();
    QDate getCalDate();
    void getValue();

signals:
    void findSig(int);
    void exportSig(int);
    void clearSig();

private slots:
    void on_roomRadio_clicked();
    void on_devRadio_clicked();
    void on_findBtn_clicked();
    void on_exportBtn_clicked();
    void on_startDateBtn_clicked();
    void on_endDateBtn_clicked();
    void on_pushButton_clicked();

    void on_cleverBtn_clicked();
    void on_cabRadio_clicked();
    void on_modeBox_currentIndexChanged(int index);

protected:
    Ui::Log_ExportBarWid *ui;

public:
    QString room, ip, cab;
    QDate startDate, endDate;
    QTime startTime, endTime;
    int slaveNum;
   diagram *box;
   DiaScatter *scatter;

};

#endif // LOG_EXPORTBARWID_H
