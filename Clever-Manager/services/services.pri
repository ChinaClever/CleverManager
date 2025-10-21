
INCLUDEPATH += $$PWD

include(smtp/SmtpClient/SmtpClient.pri)
include(dingtalk/dingtalk.pri)

HEADERS += \	
    $$PWD/smtp/email.h \
    $$PWD/smtp/email_testdlg.h \
    $$PWD/smtp/smtp.h \
    $$PWD/serial/serialdev.h \
    $$PWD/serial/serialport.h \
    $$PWD/sim900a/sim900a.h \
    $$PWD/sim900a/sim900a_call.h \
    $$PWD/sim900a/sim900a_err.h \
    $$PWD/sim900a/sim900a_interface.h \
    $$PWD/sim900a/sim900a_serial.h \
    $$PWD/sim900a/sim900a_serialdlg.h \
    $$PWD/sim900a/sim900a_sms.h \
    $$PWD/sim900a/sim900a_thread.h \
    $$PWD/config/sysconfigfile.h \
    $$PWD/ser_servicdlg.h \
    $$PWD/smtp/email_setdlg.h \
    $$PWD/smtp/emailthread.h \
    $$PWD/smtp/smtpssl.h \
    $$PWD/smtp/smtpclients.h

SOURCES += \	
    $$PWD/smtp/email.cpp \
    $$PWD/smtp/email_testdlg.cpp \
    $$PWD/smtp/smtp.cpp \
    $$PWD/serial/serialdev.cpp \
    $$PWD/serial/serialport.cpp \
    $$PWD/sim900a/sim900a.cpp \
    $$PWD/sim900a/sim900a_call.cpp \
    $$PWD/sim900a/sim900a_err.cpp \
    $$PWD/sim900a/sim900a_interface.cpp \
    $$PWD/sim900a/sim900a_serial.cpp \
    $$PWD/sim900a/sim900a_serialdlg.cpp \
    $$PWD/sim900a/sim900a_sms.cpp \
    $$PWD/sim900a/sim900a_thread.cpp \
    $$PWD/config/sysconfigfile.cpp \
    $$PWD/ser_servicdlg.cpp \
    $$PWD/smtp/email_setdlg.cpp \
    $$PWD/smtp/emailthread.cpp \
    $$PWD/smtp/smtpssl.cpp \
    $$PWD/smtp/smtpclients.cpp

FORMS += \	
    $$PWD/smtp/email_setdlg.ui \

	
	
	
	
