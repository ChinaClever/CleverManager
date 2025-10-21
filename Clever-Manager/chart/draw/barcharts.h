#ifndef BARCHARTS_H
#define BARCHARTS_H

#include "drawchart.h"

typedef struct
{
    ushort number;
    ushort line;
    ushort alarm;
    ushort offLine;
}BarChart_Data;

class BarCharts : public QWidget
{
    Q_OBJECT
public:
    explicit BarCharts(QWidget *parent = 0);
    ~BarCharts();

    void setData(BarChart_Data *);

protected:
    void initBars(void);
    void setColors(void);
    void initXAxis(void);
    void setRange(int);

    void setNumData(int);
    void setLineData(int);
    void setAlarmData(int);
    void setOffLineData(int);

signals:

public slots:

private:
    QGridLayout *layout;
    DrawChart *m_drawChart;

    QCPBars *numberBar;
    QCPBars *lineBar;
    QCPBars *alarmBar;
    QCPBars *offLineBar;
};

#endif // BARCHARTS_H
