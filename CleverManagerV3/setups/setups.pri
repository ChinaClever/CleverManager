
INCLUDEPATH += $$PWD

include(users/users.pri)
include(upgrade/upgrade.pri)
include(dingtalk/dingtalk.pri)

HEADERS += \	
    $$PWD/pdudevices/setup_pdusquerydlg.h \
    $$PWD/pdudevices/setup_pdusbtnbar.h \
    $$PWD/pdudevices/setup_pdustablewid.h \
    $$PWD/pdudevices/setup_cabquerydlg.h \
    $$PWD/pdudevices/setup_roomquerydlg.h \
    $$PWD/push/setup_pushwid.h \
    $$PWD/setup_mainwid.h
	
SOURCES += \	
    $$PWD/pdudevices/setup_pdusquerydlg.cpp \
    $$PWD/pdudevices/setup_pdusbtnbar.cpp \
    $$PWD/pdudevices/setup_pdustablewid.cpp \
    $$PWD/pdudevices/setup_cabquerydlg.cpp \
    $$PWD/pdudevices/setup_roomquerydlg.cpp \
    $$PWD/push/setup_pushwid.cpp \
    $$PWD/setup_mainwid.cpp

FORMS += \
    $$PWD/pdudevices/setup_pdusquerydlg.ui \
    $$PWD/pdudevices/setup_pdustablewid.ui \
    $$PWD/pdudevices/setup_cabquerydlg.ui \
    $$PWD/push/setup_pushwid.ui \
    $$PWD/setup_mainwid.ui




