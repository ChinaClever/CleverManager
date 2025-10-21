
INCLUDEPATH += $$PWD

#win32:LIBS += -L$$PWD/lib/ -lWMIHWLib
#CONFIG += qaxcontainer

# Location of REGISTER Library
REGISTER_LIBRARY_LOCATION = $$PWD/lib

win32:CONFIG(release, debug|release): LIBS += -L$$REGISTER_LIBRARY_LOCATION/release/ -lregisterLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$REGISTER_LIBRARY_LOCATION/debug/ -lregisterLib
#else:unix: LIBS += -L$$REGISTER_LIBRARY_LOCATION/linux -lregisterLib

INCLUDEPATH += $$REGISTERLIB_LIBRARY
DEPENDPATH += $$REGISTERLIB_LIBRARY


HEADERS += \
    $$PWD/registrykey.h \
    $$PWD/machinecode.h \
    $$PWD/softwareregiste.h \
    $$PWD/activecode.h \
    $$PWD/activeprocess.h \
    $$PWD/registerbox.h \
    $$PWD/registerinfo.h \
    $$PWD/sharpness.h


SOURCES += \
    $$PWD/registrykey.cpp \
    $$PWD/machinecode.cpp \
    $$PWD/activecode.cpp \
    $$PWD/activeprocess.cpp \
    $$PWD/registerbox.cpp \
    $$PWD/registerinfo.cpp \
    $$PWD/sharpness.cpp


    

FORMS += \
    $$PWD/activeprocess.ui \
    $$PWD/registerbox.ui \
    $$PWD/registerinfo.ui

