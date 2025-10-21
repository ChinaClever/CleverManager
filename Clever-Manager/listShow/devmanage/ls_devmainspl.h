#ifndef LS_DEVMAINSPL_H
#define LS_DEVMAINSPL_H


#include "devlist/ls_mdevwidget.h"
#include "group/ls_mgroupwidget.h"
#include "type/ls_mtypewidget.h"
#include "common.h"

class LS_DevMainSpl : public QWidget
{
    Q_OBJECT
public:
    explicit LS_DevMainSpl(QWidget *parent = 0);

protected:
    void initLayout(void);
    void initSpl(void);

signals:

public slots:
    void initFunSLot();
//    void timeoutDone();

private:
    QSplitter *splitter;
    QGridLayout *layout;



    LS_MTypeWidget *mType;
    ls_MGroupWidget *m_mgroupList;
    ls_MDevWidget *m_mdevTable;
};

#endif // LS_DEVMAINSPL_H
