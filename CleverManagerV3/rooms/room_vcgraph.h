#ifndef TG_CURVOLGRAPH_H
#define TG_CURVOLGRAPH_H

#include "line_vcgraphic.h"
#include "room_titlebar.h"
#include "datapacket.h"

class Room_VcGraph : public QWidget
{
    Q_OBJECT
public:
    explicit Room_VcGraph(QWidget *parent = 0);
    ~Room_VcGraph();

protected:
    void initView();
    void addCurData(double);
    void addVolData(int);
    double getCurData(qint64);
    void addData(int, qint64);
    bool checkPack();

public slots:
    void clearSlot();
    void timeoutDone();
    void packetSlot(sRoomPacket *p);

private:
    Line_VcGraphic *m_pGraph;

    Room_TitleBar *mBar;
    bool isBig;

    QTimer *timer;
    sRoomPacket *mPacket;
};

#endif // TG_CURVOLGRAPH_H
