
QT      += network

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/AbstractJob.h \
    $$PWD/QtSnmpClient.h \
    $$PWD/QtSnmpData.h \
    $$PWD/RequestSubValuesJob.h \
    $$PWD/RequestValuesJob.h \
    $$PWD/Session.h \
    $$PWD/SetValueJob.h \
    $$PWD/defines.h \
    $$PWD/win_export.h


SOURCES += \
    $$PWD/AbstractJob.cpp \
    $$PWD/QtSnmpClient.cpp \
    $$PWD/QtSnmpData.cpp \
    $$PWD/RequestSubValuesJob.cpp \
    $$PWD/RequestValuesJob.cpp \
    $$PWD/Session.cpp \
    $$PWD/SetValueJob.cpp





