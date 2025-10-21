
INCLUDEPATH += $$PWD

HEADERS += \	
    $$PWD/tftpupdate/tftp.h \
    $$PWD/tftpupdate/tftpthread.h \
    $$PWD/tftpupdate/tftpwidget.h \
    $$PWD/tftpupdate/tftpdialog.h \
    $$PWD/tcpupdate/up_tcpclient.h \
    $$PWD/tcpupdate/up_tcpsentthread.h \
    $$PWD/tcpupdate/up_tcpsent.h \
    $$PWD/tcpupdate/up_tcpwidget.h \
    $$PWD/tcpupdate/up_tcpdialog.h \
    $$PWD/updatedevdlg.h
   

SOURCES += \	
    $$PWD/tftpupdate/tftp.cpp \
    $$PWD/tftpupdate/tftpthread.cpp \
    $$PWD/tftpupdate/tftpwidget.cpp \
    $$PWD/tftpupdate/tftpdialog.cpp \
    $$PWD/tcpupdate/up_tcpclient.cpp \
    $$PWD/tcpupdate/up_tcpsentthread.cpp \
    $$PWD/tcpupdate/up_tcpsent.cpp \
    $$PWD/tcpupdate/up_tcpwidget.cpp \
    $$PWD/tcpupdate/up_tcpdialog.cpp \
    $$PWD/updatedevdlg.cpp


FORMS += \
    $$PWD/tftpupdate/tftpwidget.ui \
    $$PWD/tftpupdate/tftpdialog.ui \
    $$PWD/tcpupdate/up_tcpwidget.ui \
    $$PWD/tcpupdate/up_tcpdialog.ui \
    $$PWD/updatedevdlg.ui
