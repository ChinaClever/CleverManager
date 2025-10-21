#ifndef LS_MAINSPL_H
#define LS_MAINSPL_H

#include <QSplitter>
#include <QGridLayout>

//#include "ls_devstatusnum.h"
#include "ls_refreshbtn.h"
#include "ls_rightspl.h"

class LS_MainSpl : public QWidget
{
    Q_OBJECT
public:
    explicit LS_MainSpl(QWidget *parent = 0);
    ~LS_MainSpl();

protected:
    void initLeftSpl(void);
    void initRightSpl(void);
    void initFunction(void);

    void initLayout(void);

signals:

public slots:
     void selectSlot(QString, short);

private:
    QSplitter *mainSplitter;
    QSplitter *leftSplitter;
    QSplitter *rightSplitter;
    QSplitter *splitter;
    QGridLayout *layout;

private:
    LS_listTabledlg *mListTab;
    LS_RefreshBtn *mRefreshBtn;

    LS_RightSpl *mRightSpl;
};

#endif // LS_MAINSPL_H
