#ifndef DS_DATAGRAPH_H
#define DS_DATAGRAPH_H

#include "curvolgraphic.h"
#include "devdata/ds_datadsplay.h"

class DS_DataGraph : public QWidget
{
    Q_OBJECT
public:
    explicit DS_DataGraph(QWidget *parent = 0);
    ~DS_DataGraph();

    void updateGraph(void);
protected:
    bool isNewDev(DS_DsyDev &dev);
    bool checkAlarm(short ,int);
    bool checkCurAlarm(int);
    bool checkVolAlarm(int);

    void checkStatus(void);
    void readData(void);
    bool checkDev(void);
signals:

public slots:
    void initFun(void);
    void timeoutDone(void);
    void updateGraph(DS_DsyDev &);

private:
    QGridLayout *mLayout;
    CurVolGraphic *graph;
    DS_DsyDev mDeV;

    short mAlarm;
    QTimer *timer;
    PduDataPacket *mDataPacket;
    PduPacketTG *mPduTG;
    sPduTGDataStr *mPduData;
};

#endif // DS_DATAGRAPH_H
