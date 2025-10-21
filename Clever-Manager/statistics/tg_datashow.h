#ifndef TG_DATASHOW_H
#define TG_DATASHOW_H

#include "tg_datadsplay.h"
#include "tg_devnumgraph.h"
#include "tg_powergraph.h"
#include "topologywidget.h"
#include "tg_datathread.h"

class TG_DataShow : public QWidget
{
    Q_OBJECT
public:
    explicit TG_DataShow(QWidget *parent = 0);
    ~TG_DataShow();

    void init(int id);

protected:
    void initFunction(void);
    void initLayout(void);
    void initUpSpl(void);
    void initDownSpl(void);

signals:

public slots:
    void initSlot();
    void timeoutDone(void);

private:
    TG_DataDsplay *m_pDataDsp;
    TG_PowerGraph *m_pPower;
    TG_CurVolGraph *m_pCurVOl;
    TG_DevNumGraph *m_pDevNum;
    TG_DataThread *m_pTgThread;
    sTGDataSt *mTGDataSt;
    QTimer *timer;

private:
    QSplitter *mainSplitter;
    QSplitter *upSplitter;
    QSplitter *downSplitter;
    QGridLayout *layout;
};

#endif // TG_DATASHOW_H
