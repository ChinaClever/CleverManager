#ifndef SYSCONFIGFILE_H
#define SYSCONFIGFILE_H
#include <QtCore>


extern bool Sys_ConfigFile_Open(void);
extern void Sys_ConfigFile_Close(void);
extern  QString Sys_ConfigFile_ReadStr(QString, QString strGroup="System");
extern int Sys_ConfigFile_ReadInt(QString, QString strGroup="System");
extern double Sys_ConfigFile_ReadDouble(QString, QString strGroup="System");
extern void Sys_ConfigFile_Write(QString, QString, QString strGroup="System");

extern void Sys_ConfigFile_WriteParam(QString, QString);

#endif // SYSCONFIGFILE_H

