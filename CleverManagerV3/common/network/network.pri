QT      += network
QT      += websockets

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/udp/recv/udprecvsocket.h \
    $$PWD/udp/sent/udpbdsent.h \
    $$PWD/udp/sent/udpheartbeat.h \
    $$PWD/udp/sent/udpsentsocket.h \
    $$PWD/tcp/client/tcpclient.h \
    $$PWD/tftp/tftp.h \
    $$PWD/udp/recv/udpdataque.h \
    $$PWD/udp/recv/udpsocketlist.h \
    $$PWD/net_data/net_datasave.h \
    $$PWD/net_data/net_datacom.h \
    $$PWD/net_data/net_devdata.h \
    $$PWD/net_pack/net_dataformat.h \
    $$PWD/net_pack/net_analyzedata.h \
    $$PWD/net_pack/net_packdata.h \
    $$PWD/http/httpdaemon.h \
    $$PWD/net_dataanalyze.h


SOURCES += \
    $$PWD/udp/recv/udprecvsocket.cpp \
    $$PWD/udp/sent/udpbdsent.cpp \
    $$PWD/udp/sent/udpheartbeat.cpp \
    $$PWD/udp/sent/udpsentsocket.cpp \
    $$PWD/tcp/client/tcpclient.cpp \
    $$PWD/tftp/tftp.cpp \
    $$PWD/udp/recv/udpdataque.cpp \
    $$PWD/udp/recv/udpsocketlist.cpp \
    $$PWD/net_data/net_datasave.cpp \
    $$PWD/net_data/net_datacom.cpp \
    $$PWD/net_data/net_devdata.cpp \
    $$PWD/net_pack/net_analyzedata.cpp \
    $$PWD/net_pack/net_packdata.cpp \
    $$PWD/http/httpdaemon.cpp \
    $$PWD/net_dataanalyze.cpp

FORMS += \





