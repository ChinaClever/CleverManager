#ifndef ELECBTNBAR_H
#define ELECBTNBAR_H

#include "logcomwid.h"

class ElecBtnBar : public LogBtnBar
{
    Q_OBJECT
public:
    explicit ElecBtnBar(QWidget *parent = nullptr);

protected:
    virtual QString queryBtn();
};

#endif // ELECBTNBAR_H
