#ifndef CURVOLGRAPHIC_H
#define CURVOLGRAPHIC_H

#include "line_drawgraphic.h"

class Line_VcGraphic : public QWidget
{
    Q_OBJECT
public:
    explicit Line_VcGraphic(QWidget *parent = 0);
    ~Line_VcGraphic();

    void initCurVolGraph(void);

    void setAutoRange(int, int);
    void addCurData(double,bool alarm=false);
    void addVolData(int,bool alarm=false);
    void clearData(void);

protected:
    bool setVolRange(int);
    bool setCurRange(double);
    void initData(void);

signals:

public slots:

private:
    QGridLayout *layout;
    Line_DrawGraphic *draw;
    int m_tick;
    int mRange;

    int mVolRange;
    int mCurRange;

    QCPGraph *m_pCurGraph,*m_pCurAlarmGraph;
    QCPGraph *m_pVolGraph,*m_pVolAlarmGraph;
};

#endif // CURVOLGRAPHIC_H
