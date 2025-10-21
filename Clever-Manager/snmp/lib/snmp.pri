

win32:{
    INCLUDEPATH += $$PWD
    DEPENDPATH += $$PWD
    DEFINES += WIN32_LEAN_AND_MEAN HAVE_WSTCPIP_H HAVE_TIME_H HAVE_WINSOCK2_H HAVE_WS2TCPIP_H HAVE_WSPIAPI_H STDCXX_98_HEADERS HAVE_SYS_TIMEB_H

    CONFIG(release, debug|release){
        LIBS += -L$$PWD/win32/opt/ -lsnmp_pp
        LIBS += -lntdll -lwinmm -lws2_32
    } else {
        LIBS += -L$$PWD/win32/dbg -lsnmp_pp
        LIBS += -lntdll -lwinmm -lws2_32
    }
} else: {
    unix: LIBS += -lsnmp++ -ldes
}




