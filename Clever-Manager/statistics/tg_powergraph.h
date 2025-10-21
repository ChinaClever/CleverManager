#ifndef TG_POWERGRAPH_H
#define TG_POWERGRAPH_H

#include "drawgraphic.h"
#include "titlebar.h"

class TG_PowerGraph : public QWidget
{
    Q_OBJECT
public:
    explicit TG_PowerGraph(QWidget *parent = 0);
    ~TG_PowerGraph();

    void addPowData(qint64, int);

protected:
    double getData(qint64);
    void initFun(void);
    void setAutoRange(int);
    void initView();

signals:

public slots:
    void initSlot(void);

private:
    DrawGraphic *m_pDraw;
    QGroupBox *groupBox;
    QGridLayout *m_pLayout;

    QCPGraph *m_pGraph;
    TitleBar *mBar;
    int mTick,mTrange;
    double mRange;
    bool isBig;
};

#endif // TG_POWERGRAPH_H
