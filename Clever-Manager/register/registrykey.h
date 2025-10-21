#ifndef REGISTRYKEY_H
#define REGISTRYKEY_H
#include <QObject>
#include <QSettings>
#include <QDebug>
#if defined(Q_OS_WIN32)
#include <QMessageBox>
#include <QAxObject>
#include <windows.h>
#include <tchar.h>
#include <QApplication>
#elif defined(Q_OS_LINUX)

#endif
#include <stdio.h>
#include <QUuid>
#include <QFile>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QClipboard>


class RegistryKey
{
public:
    RegistryKey();

    void writeDate(const QDate &date);
    void writeCount(int count);
    void writeMachineCode(const QString &code);
    void writeActiveCode(const QString &code);

    QDate readDate();
    int readCount();
    QString readMachineCode();
    QString readActiveCode();

protected:
    QString read(const QString &key);
    void write(const QString &key,const QString &str);
};

#endif // REGISTRYKEY_H
