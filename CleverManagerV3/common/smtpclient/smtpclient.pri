
include(smtplib/smtplib.pri)

INCLUDEPATH += $$PWD

HEADERS +=  \
    $$PWD/email.h \
    $$PWD/email_setdlg.h \
    $$PWD/emailthread.h
	
SOURCES +=  \
    $$PWD/email.cpp \
    $$PWD/email_setdlg.cpp \
    $$PWD/emailthread.cpp

FORMS += \
    $$PWD/email_setdlg.ui




