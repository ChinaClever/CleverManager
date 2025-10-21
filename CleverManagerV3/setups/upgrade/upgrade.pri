
INCLUDEPATH += $$PWD

include(up_ips/up_ips.pri)
include(up_work/up_work.pri)

HEADERS += \
    $$PWD/up_maindlg.h \
    $$PWD/up_devselectwid.h \
    $$PWD/up_datapacket.h
	
SOURCES += \
    $$PWD/up_maindlg.cpp \
    $$PWD/up_datapacket.cpp \
    $$PWD/up_devselectwid.cpp

FORMS += \
    $$PWD/up_maindlg.ui \
    $$PWD/up_devselectwid.ui




