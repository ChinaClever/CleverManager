#ifndef DS_DATAGRAPH_H
#define DS_DATAGRAPH_H

#include "line_vcgraphic.h"
#include "datapacket.h"

class Pdu_LineGraph : public QWidget
{
    Q_OBJECT
public:
    explicit Pdu_LineGraph(QWidget *parent = 0);
    ~Pdu_LineGraph();

protected:
    void updateGraph();
    bool checkDev();

signals:

public slots:
    void timeoutDone();
    void packetSlot(sDataPacket *p, int line);

private:
    QGridLayout *mLayout;
    Line_VcGraphic *graph;

    short mLine;
    QTimer *timer;
    sDataPacket *mPacket;
};

#endif // DS_DATAGRAPH_H
