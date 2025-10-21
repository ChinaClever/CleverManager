/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "pdu_sethumdlg.h"

Pdu_SetHumDlg::Pdu_SetHumDlg(QWidget *parent) : Pdu_SetThresholdDlg(parent)
{

}

void Pdu_SetHumDlg::init(int id, sDataPacket *packet)
{
    setMode(SET_CMD_ENV_HUM, id, packet);
}
