#ifndef DRAWCHART_H
#define DRAWCHART_H

#include <QtCore>
#include <QWidget>
#include "qcustomplot.h"

class Bar_DrawChart : public QWidget
{
    Q_OBJECT
public:
    explicit Bar_DrawChart(QWidget *parent = 0);
    ~Bar_DrawChart();

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
