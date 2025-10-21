#ifndef DS_DEVINFOTABLE_H
#define DS_DEVINFOTABLE_H

#include "ds_devstate.h"

class DS_DevInfoTable : public QWidget
{
    Q_OBJECT
public:
    explicit DS_DevInfoTable(QWidget *parent = 0);
    ~DS_DevInfoTable();

protected:
    void initLayout(void);
    void initWidget(void);

signals:

public slots:
     void updateWidget(QString &,int);

private:
    QTabWidget *tabWidget;
    QWidget *little_widget;
    QVBoxLayout*little_layout;

    DS_DevState *m_devState;
    DS_DevData *m_devData;
//    DS_DevInfo *m_devInfo;
    DS_Threshold *m_threshold;
};

#endif // DS_DEVINFOTABLE_H
