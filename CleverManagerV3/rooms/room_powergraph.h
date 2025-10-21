#ifndef TG_POWERGRAPH_H
#define TG_POWERGRAPH_H

#include "room_vcgraph.h"

class Room_PowerGraph : public QWidget
{
    Q_OBJECT
public:
    explicit Room_PowerGraph(QWidget *parent = 0);
    ~Room_PowerGraph();

protected:
    double getData(qint64);
    void initFun();
    void setAutoRange(int);
    void initView();
    void addPowData(qint64, int);
    bool checkPack();

signals:

public slots:
    void initSlot();
    void timeoutDone();
    void packetSlot(sRoomPacket *p);

private:
    Line_DrawGraphic *m_pDraw;

    QTimer *timer;
    QCPGraph *m_pGraph;
    Room_TitleBar *mBar;
    int mTick,mTrange;
    double mRange;
    bool isBig;
    sRoomPacket *mPacket;
};

#endif // TG_POWERGRAPH_H
