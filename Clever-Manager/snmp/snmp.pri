INCLUDEPATH += $$PWD

include($$PWD/lib/snmp.pri)

HEADERS += \
    $$PWD/common_snmp.h \
    $$PWD/mibreader.h \
    $$PWD/traversalsnmp.h \
    $$PWD/snmpsetting.h \
    $$PWD/snmptrap.h

SOURCES += \
    $$PWD/mibreader.cpp \
    $$PWD/traversalsnmp.cpp \
    $$PWD/common_snmp.cpp \
    $$PWD/snmpsetting.cpp \
    $$PWD/snmptrap.cpp
