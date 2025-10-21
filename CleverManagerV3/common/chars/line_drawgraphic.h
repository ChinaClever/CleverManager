#ifndef DRAWGRAPHIC_H
#define DRAWGRAPHIC_H

#include <QtCore>
#include <QWidget>
#include "qcustomplot.h"

class Line_DrawGraphic : public QWidget
{
    Q_OBJECT
public:
    explicit Line_DrawGraphic(QWidget *parent = 0);
    ~Line_DrawGraphic();

    void setTitle(const QString &);
    void setName(const QString &);

    void addData(QCPGraph *,double,bool onlyEnlarge=true);
    void clearData(QCPGraph *);

    QCPGraph *addVoltageGraph(const QString & );
    QCPGraph *addVolAlarmGraph(void);

    QCPGraph *addCurrentGraph(const QString & );
    QCPGraph *addCurAlarmtGraph(void);

    QCPGraph *addPowerGraph(const QString & );

    void initCurVolFun(void);

    void initxAxis(void); //下轴
    void setxAxis(int);
    void addxAxis(int);

    void inityAxis(const QString &);  //左轴
    void setyAxis(double,double);

    void initxAxis2(void); // 上轴

    void inityAxis2(const QString &); //右轴
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
