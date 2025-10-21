#ifndef PDUDMMANAGE_H
#define PDUDMMANAGE_H
#include <QtCore>

bool pdu_dm_delDev(QString &ip);
bool pdu_dm_del(int type, const QString &ip);

void pdu_dm_renameGroup(QString &ip, QString &group);

#endif // PDUDMMANAGE_H
