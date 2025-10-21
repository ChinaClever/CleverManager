#ifndef TG_CURVOLGRAPH_H
#define TG_CURVOLGRAPH_H

#include "curvolgraphic.h"
#include "titlebar.h"
class TG_CurVolGraph : public QWidget
{
    Q_OBJECT
public:
    explicit TG_CurVolGraph(QWidget *parent = 0);
    ~TG_CurVolGraph();

    void addData(int, qint64, int);

protected:
    void initView();
    void setAutoRange(int);
     void addCurData(double);
     void addVolData(int);
     double getCurData(qint64);


signals:

public slots:
     void initSlot(void);

private:
    CurVolGraphic *m_pGraph;
    QGroupBox *groupBox;

    TitleBar *mBar;
    QGridLayout *m_pLayout;
    bool isBig;
};

#endif // TG_CURVOLGRAPH_H
