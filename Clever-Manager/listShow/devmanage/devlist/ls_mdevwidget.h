#ifndef LS_MDEVWIDGET_H
#define LS_MDEVWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QSplitter>

#include "ls_mdevtable.h"
#include "ls_mdevbtn.h"

class ls_MDevWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ls_MDevWidget(QWidget *parent = 0);

protected:
    void initBtn(void);
    void initWidget(void);

signals:


public slots:
    void updateSlot(QString &);

protected slots:
    void addSlot();
    void delSlot();
    void moveSlot();
    void reloadSlot();

public:
    QSplitter *splitter;
    QGridLayout *layout;

    LS_MdevTable *mTable;
    LS_MDevBtn *mBtn;
};

#endif // LS_MDEVWIDGET_H
