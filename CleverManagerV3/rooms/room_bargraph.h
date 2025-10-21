#ifndef TG_DEVNUMGRAPH_H
#define TG_DEVNUMGRAPH_H

#include <QLabel>
#include "bar_chartswid.h"
#include "room_vcgraph.h"

class Room_BarGraph : public QWidget
{
    Q_OBJECT
public:
    explicit Room_BarGraph(QWidget *parent = 0);
    ~Room_BarGraph();

    void updateData(Bar_sChartData &data);

protected:
    void initWidget();

signals:

public slots:
    void timeoutDone();
    void packetSlot(sRoomPacket *p) {mPacket=p;}

private:
    QTimer *timer;
    QLabel *lab;
    sRoomPacket *mPacket;
    Bar_ChartsWid *m_barChart;
};

#endif // TG_DEVNUMGRAPH_H
