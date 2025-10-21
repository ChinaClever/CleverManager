#ifndef UI_OUTPUTWIDGET_H
#define UI_OUTPUTWIDGET_H

#include <QGroupBox>
#include "ui_outputtable.h"

class UI_OutputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UI_OutputWidget(QWidget *parent = 0);

protected:
    void initLayout(void);
    void initWidget(void);

signals:

public slots:
    void updateWidget(QString &ip, int num);

private:
    QTabWidget *tabWidget;
    QGroupBox *mGroupBox;
    UI_OutputTable *mOutputTable;
    QWidget *little_widget;
    QVBoxLayout *little_layout;
};

#endif // UI_OUTPUTWIDGET_H
