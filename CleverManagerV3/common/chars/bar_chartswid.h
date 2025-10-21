#ifndef BARCHARTS_H
#define BARCHARTS_H

#include "bar_drawchart.h"

struct Bar_sChartData
{
    ushort number;
    ushort line;
    ushort alarm;
    ushort offLine;
};

class Bar_ChartsWid : public QWidget
{
    Q_OBJECT
public:
    explicit Bar_ChartsWid(QWidget *parent = 0);
    ~Bar_ChartsWid();

    void setData(Bar_sChartData &);

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
    Bar_DrawChart *m_drawChart;

    QCPBars *numberBar;
    QCPBars *lineBar;
    QCPBars *alarmBar;
    QCPBars *offLineBar;
};

#endif // BARCHARTS_H
