#ifndef PDU_SETHUMDLG_H
#define PDU_SETHUMDLG_H

#include "pdu_settemdlg.h"

class Pdu_SetHumDlg : public Pdu_SetThresholdDlg
{
    Q_OBJECT
public:
    explicit Pdu_SetHumDlg(QWidget *parent = nullptr);

    void init(int id, sDataPacket *packet);
};

#endif // PDU_SETHUMDLG_H
