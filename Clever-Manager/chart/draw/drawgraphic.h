#ifndef DRAWGRAPHIC_H
#define DRAWGRAPHIC_H

#include <QtCore>
#include <QWidget>
#include "qcustomplot.h"
#include "common.h"

class DrawGraphic : public QWidget
{
    Q_OBJECT
public:
    explicit DrawGraphic(QWidget *parent = 0);
    ~DrawGraphic();

    void setTitle(QString);
    void setName(QString);

    void addData(QCPGraph *,double,bool onlyEnlarge=true);
    void clearData(QCPGraph *);

    QCPGraph *addVoltageGraph(QString );
    QCPGraph *addVolAlarmGraph(void);

    QCPGraph *addCurrentGraph(QString );
    QCPGraph *addCurAlarmtGraph(void);

    QCPGraph *addPowerGraph(QString );

    void initCurVolFun(void);

    void initxAxis(void); //下轴
    void setxAxis(int);
    void addxAxis(int);

    void inityAxis(QString);  //左轴
    void setyAxis(double,double);

    void initxAxis2(void); // 上轴

    void inityAxis2(QString); //右轴
    void setyAxis2(double,double);

protected:
    void setScatterStyle(QCPGraph *, int );
    void initAxis(QCPAxis *);

    void initLegende(void);
    void setBackground(void);

signals:

public slots:

private:
    QGridLayout *layout;
    QCustomPlot *customPlot;
    int m_xRange;

    int m_startTime;
};

#endif // DRAWGRAPHIC_H
