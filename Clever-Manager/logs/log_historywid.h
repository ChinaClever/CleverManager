#ifndef LOG_HISTORYWID_H
#define LOG_HISTORYWID_H

#include <QDialog>
#include <QDateTime>
#include <QMessageBox>
#include "common.h"
#include "qcharts/diagram.h"
#include "qcharts/diagramwid.h"
#include "chartdata.h"

namespace Ui {
class LOG_HistoryWid;
}

class LOG_HistoryWid : public QDialog
{
    Q_OBJECT

public:
    explicit LOG_HistoryWid(QWidget *parent = 0);
    ~LOG_HistoryWid();

private slots:
    void on_pushButton_search_clicked();

    void on_lastDayBtn_clicked();

    void on_lastWeekBtn_clicked();

    void on_lastMonthBtn_clicked();

signals:
    void signalChart(chartData *data);

private:
    Ui::LOG_HistoryWid *ui;
    QDate mDatetime;
    chartData *mData;

private:
    void initDateTimeEdit();
    bool checkEnter();
    void setTime();
    void writeChartData();

};

#endif // LOG_HISTORYWID_H
