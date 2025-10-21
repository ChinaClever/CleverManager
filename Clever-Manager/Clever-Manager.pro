#-------------------------------------------------
#
# Project created by QtCreator 2016-06-16T10:02:46
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
QT       += serialport
QT       += printsupport
QT       += charts
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Clever-Manager


TEMPLATE = app

#TRANSLATIONS = English.ts
TRANSLATIONS = English2.ts

QMAKE_CXXFLAGS_RELEASE+=$$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE +=$$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

#配置生成执行程序位置
DESTDIR = $$PWD/bin

INCLUDEPATH += $$PWD \

include(common/common.pri)
include(chart/chart.pri)
include(usr/usr.pri)
include(sql/sql.pri)
include(logs/logs.pri)
include(services/services.pri)
include(details/details.pri)
include(statistics/statistics.pri)
include(net/net.pri)
include(listShow/listShow.pri)
include(pdu/pdu.pri)
include(topology/topology.pri)
include(update/update.pri)
include(curl-master/curl-master.pri)
include(snmp/snmp.pri)
include(zebra/zebra.pri)
include(excel/excel.pri)
include(register/register.pri)
include(qcharts/qcharts.pri)
include(export/export.pri)
include(autoaddfriend/autoaddfriend.pri)


SOURCES += main.cpp\
        mainwindow.cpp \
    titlebar.cpp \
    versionselect.cpp


HEADERS  += mainwindow.h \
    titlebar.h \
    versionselect.h


FORMS    += mainwindow.ui \
    titlebar.ui \
    versionselect.ui

RESOURCES += \
    images/image.qrc\

#DEFINES += ZEBRA_MODULE
#DEFINES += SNMP_MODULE   /////=======  lzy 190216
#DEFINES += DEFINE_MODULE
#DEFINES += SQL_THROW_ERROR


#LIBS += -lDbgHelp
# 只用来做试验 验证、测试用
#DEFINES += CLOUD_MODULE
RC_ICONS = logo.ico
