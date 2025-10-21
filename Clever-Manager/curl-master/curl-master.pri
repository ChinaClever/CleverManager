

win32: {
    INCLUDEPATH += $$PWD
    DEPENDPATH += $$PWD
    LIBS += -L$$PWD/lib/ -llibcurl_imp
} else: {
#linux中libcurl库安装方法
#sudo apt-get install libcurl4-openssl-dev
    unix: LIBS += -lcurl
}
