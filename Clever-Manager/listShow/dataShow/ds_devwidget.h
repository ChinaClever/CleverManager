#ifndef DS_DEVWIDGET_H
#define DS_DEVWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QGridLayout>
#include "ds_datagraph.h"
#include "ds_datashowwid.h"

//#include "devdata/ds_datashow.h"
//#include "devinfotable/ds_devinfotable.h"

class DS_DevWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DS_DevWidget(QWidget *parent = 0);

    void selectDev(QString &ip,int num);

protected:
    void initSplitter(void);
    void initLayout(void);

signals:

public slots:

private:
    QSplitter *rightSplitter;
    QSplitter *splitter;
    QGridLayout *layout;

    DS_DataShowWid *m_dataShow;
    DS_DataGraph *m_graph;
//    DS_DevInfoTable *m_devInfo;
};

#endif // DS_DEVWIDGET_H
