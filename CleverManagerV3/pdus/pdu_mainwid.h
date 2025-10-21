#ifndef PDU_MAINWID_H
#define PDU_MAINWID_H

#include "pdu_line/pdu_linewid.h"
#include "pdu_env/pdu_envwid.h"
#include "pdu_list/pdu_listwid.h"
#include "pdu_set/pdu_setmainwid.h"

namespace Ui {
class Pdu_MainWid;
}

class Pdu_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_MainWid(QWidget *parent = 0);
    ~Pdu_MainWid();


protected slots:
    void initWidSlot();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Pdu_MainWid *ui;

    Pdu_ListWid *mList;
    Pdu_LineWid *mLineWid;
    Pdu_EnvWid *mEnvWid;
    Pdu_OutputWid *mOutputWid;
    Pdu_SetMainWid *mSetWid;
};

#endif // PDU_MAINWID_H
