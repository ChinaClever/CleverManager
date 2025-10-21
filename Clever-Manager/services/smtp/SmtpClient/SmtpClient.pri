
INCLUDEPATH += $$PWD


# Location of SMTP Library
#SMTP_LIBRARY_LOCATION = $$PWD/lib

#win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -lSMTPEmail
#else:unix: LIBS += -L$$SMTP_LIBRARY_LOCATION/linux/ -lSMTPEmail

#INCLUDEPATH += $$SMTP_LIBRARY_LOCATION
#DEPENDPATH += $$SMTP_LIBRARY_LOCATION
	
INCLUDEPATH += $$PWD


HEADERS += \
    $$PWD/src/emailaddress.h \
    $$PWD/src/mimeattachment.h \
    $$PWD/src/mimecontentformatter.h \
    $$PWD/src/mimefile.h \
    $$PWD/src/mimehtml.h \
    $$PWD/src/mimeinlinefile.h \
    $$PWD/src/mimemessage.h \
    $$PWD/src/mimemultipart.h \
    $$PWD/src/mimepart.h \
    $$PWD/src/mimetext.h \
    $$PWD/src/quotedprintable.h \
    $$PWD/src/smtpclient.h\
    $$PWD/src/SmtpMime.h


SOURCES += \
    $$PWD/src/emailaddress.cpp \
    $$PWD/src/mimeattachment.cpp \
    $$PWD/src/mimecontentformatter.cpp \
    $$PWD/src/mimefile.cpp \
    $$PWD/src/mimehtml.cpp \
    $$PWD/src/mimeinlinefile.cpp \
    $$PWD/src/mimemessage.cpp \
    $$PWD/src/mimemultipart.cpp \
    $$PWD/src/mimepart.cpp \
    $$PWD/src/mimetext.cpp \
    $$PWD/src/quotedprintable.cpp \
    $$PWD/src/smtpclient.cpp



	
