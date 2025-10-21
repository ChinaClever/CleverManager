#ifndef PDU_SETTEMDLG_H
#define PDU_SETTEMDLG_H

#include "pdu_set/pdu_setthresholddlg.h"

class Pdu_SetTemDlg : public Pdu_SetThresholdDlg
{
    Q_OBJECT
public:
    explicit Pdu_SetTemDlg(QWidget *parent = nullptr);

    void init(int id, sDataPacket *packet);

};

#endif // PDU_SETTEMDLG_H
