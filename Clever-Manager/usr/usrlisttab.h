#ifndef USRLISTTAB_H
#define USRLISTTAB_H

#include <QTableWidget>
#include "editusrdlg.h"
#include <QGridLayout>

class UsrListTab : public QWidget
{
    Q_OBJECT
public:
    explicit UsrListTab(QWidget *parent = 0);
    ~UsrListTab();

    void upListTab(void);
    bool getSelectUsr(QString &);
    bool editSelectUsr(void);

protected:
    void initWidget(void);
    void addRowContent(QStringList &);
    void addItemContent(int, int, QString &);

signals:

public slots:
    void doubleClickedSlot(QTableWidgetItem *);

private:
    QTableWidget *m_tab;
    QGridLayout *layout;

    UsrMaptype *m_usrMap;
};

#endif // USRLISTTAB_H
