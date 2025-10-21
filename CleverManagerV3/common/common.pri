
INCLUDEPATH += $$PWD

include(json/json.pri)
include(excel/excel.pri)
include(chars/chars.pri)
include(dbcom/dbcom.pri)
include(sound/sound.pri)
include(sqlcom/sqlcom.pri)
include(cfgcom/cfgcom.pri)
include(msgcom/msgcom.pri)
include(logcom/logcom.pri)
include(elecom/elecom.pri)
include(tabcom/tabcom.pri)
include(network/network.pri)
include(websocket/websocket.pri)
include(smtpclient/smtpclient.pri)
include(snmpclient/snmpclient.pri)
include(datapacket/datapacket.pri)
include(backcolour/backcolour.pri)
include(qtsingleapplication/qtsingleapplication.pri)

HEADERS += \	
    $$PWD/common.h
	
SOURCES += \	
    $$PWD/common.cpp

FORMS += \




