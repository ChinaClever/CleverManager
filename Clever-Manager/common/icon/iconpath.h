#ifndef ICONPATH_H
#define ICONPATH_H
#include <QtCore>
#include "common.h"

bool icon_get_devStatus(int id, QString &path);
QString icon_get_devType(void);
QString icon_get_group(void);
QString icon_get_devIcon(void);
bool icon_get_opStatus(int id, QString &path);

#endif // ICONPATH_H
