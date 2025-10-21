#ifndef TG_DEVNUMGRAPH_H
#define TG_DEVNUMGRAPH_H

#include <QLabel>
#include "barcharts.h"
#include "pdu/dev/pdudevspied.h"

class TG_DevNumGraph : public QWidget
{
    Q_OBJECT
public:
    explicit TG_DevNumGraph(QWidget *parent = 0);
    ~TG_DevNumGraph();

    void updateData(pdu_devNumStr *);

protected:
    void initWidget(void);
    void initLayout(void);

signals:

public slots:
    void timeoutDone(void);
    void initSlot(void);

private:
    QTimer *timer;
    BarCharts *m_barChart;
    QGridLayout *m_pLayout;

    QGroupBox *groupBox;
    QLabel *lab;
    BarChart_Data *mData;
};

#endif // TG_DEVNUMGRAPH_H
