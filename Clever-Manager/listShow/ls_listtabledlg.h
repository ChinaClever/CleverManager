#ifndef LS_LISTTABLEDLG_H
#define LS_LISTTABLEDLG_H

#include <QToolBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include "listdev/ls_treewidget.h"
#include "ls_listtablehead.h"

class LS_listTabledlg : public QWidget
{
    Q_OBJECT
public:
    explicit LS_listTabledlg(QWidget *parent = 0);
    ~LS_listTabledlg();

public slots:
    void alarmSlot(void);
    void updateWidget(void);

signals:
    void selectedSig(QString &, int);

protected:
    void createWidget(void);
    void initWidget(void);

    void initListTree(void);
    void initLineTree(void);
    void initOffTree(void);
    void initAlarmTree(void);

protected slots:
    void listSeletedSlot(QString &, int);
    void updateSlot(void);

private:
  QTabWidget *mTabWidget;
  QGridLayout *m_layout;

  LS_listTableHead *mTableHead;
  LS_treeWidget *mLineTree;
  LS_treeWidget *mOffLineTree;
  LS_treeWidget *mListTree;
  LS_treeWidget *mAlarmTree;
};

#endif // LS_LISTTABLEDLG_H
