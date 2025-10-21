#ifndef DRAWCHART_H
#define DRAWCHART_H

#include "drawgraphic.h"

class DrawChart : public QWidget
{
    Q_OBJECT
public:
    explicit DrawChart(QWidget *parent = 0);
    ~DrawChart();

    QCPBars *createBar(QString);
    void initXAxis(QVector<QString> &);
    void setYLabel(QString lab);
    void initLegend(void);

    void setRange(int);

protected:
    void initYAxis(void);

signals:

public slots:

public:
    QCustomPlot *customPlot;
    QGridLayout *layout;
};

#endif // DRAWCHART_H
