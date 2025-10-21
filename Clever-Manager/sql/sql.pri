
include(dc/dc.pri)

INCLUDEPATH += $$PWD \
    $$PWD/records \
    $$PWD/database \

HEADERS += \
    $$PWD/records/db_sqlquery.h \
    $$PWD/records/db_sqltablemodel.h \
    $$PWD/db_devList/db_devlist.h \
    $$PWD/db_usr/db_usr.h \    
    $$PWD/database/basicsql.h \
    $$PWD/database/dbcabinetlist.h \
    $$PWD/database/dbroomlist.h \
    $$PWD/database/dbconsumerdevices.h \
    $$PWD/database/dbpdudevices.h \
    $$PWD/database/dboidtable.h \
    $$PWD/database/dbpdutypelist.h \
    $$PWD/database/dbzebrakeydata.h \
    $$PWD/database/dbzebrachannels.h \
    $$PWD/database/dbpducommandchannel.h \
    $$PWD/db_export/records/dbtgroom.h \
    $$PWD/db_export/records/dbpduenvsensor.h \
    $$PWD/db_export/records/dbpduelecparams.h \
    $$PWD/db_export/records/dbpdudevrecord.h \
    $$PWD/db_export/records/dbslaverecordsthread.h \
    $$PWD/db_export/realTime/dbrtpduenvsensor.h \
    $$PWD/db_export/realTime/dbrtpduelecparams.h \
    $$PWD/db_export/realTime/dbrtpdudevindex.h \
    $$PWD/db_export/realTime/dbrtpdudevrecord.h \
    $$PWD/db_export/realTime/dbrttgroom.h \
    $$PWD/db_export/realTime/dbrtpduthreshold.h \
    $$PWD/db_export/records/dbpdudevindex.h \
    $$PWD/db_export/records/dbtgroomthread.h \
    $$PWD/db_export/realTime/dbrtpdusavethread.h \
    $$PWD/db_export/records/dbtgcabinet.h

SOURCES += \   
    $$PWD/records/db_sqlquery.cpp \
    $$PWD/records/db_sqltablemodel.cpp \
    $$PWD/db_devList/db_devlist.cpp \
    $$PWD/db_usr/db_usr.cpp \
    $$PWD/database/basicsql.cpp \
    $$PWD/database/dbcabinetlist.cpp \
    $$PWD/database/dbroomlist.cpp \
    $$PWD/database/dbconsumerdevices.cpp \
    $$PWD/database/dbpdudevices.cpp \
    $$PWD/database/dboidtable.cpp \
    $$PWD/database/dbpdutypelist.cpp \
    $$PWD/database/dbzebrakeydata.cpp \
    $$PWD/database/dbzebrachannels.cpp \
    $$PWD/database/dbpducommandchannel.cpp \
    $$PWD/db_export/records/dbtgroom.cpp \
    $$PWD/db_export/records/dbpduenvsensor.cpp \
    $$PWD/db_export/records/dbpduelecparams.cpp \
    $$PWD/db_export/records/dbpdudevindex.cpp \
    $$PWD/db_export/records/dbpdudevrecord.cpp \
    $$PWD/db_export/records/dbslaverecordsthread.cpp \
    $$PWD/db_export/realTime/dbrtpduenvsensor.cpp \
    $$PWD/db_export/realTime/dbrtpduelecparams.cpp \
    $$PWD/db_export/realTime/dbrtpdudevindex.cpp \
    $$PWD/db_export/realTime/dbrtpdudevrecord.cpp \
    $$PWD/db_export/realTime/dbrttgroom.cpp \
    $$PWD/db_export/realTime/dbrtpduthreshold.cpp \
    $$PWD/db_export/records/dbtgroomthread.cpp \
    $$PWD/db_export/realTime/dbrtpdusavethread.cpp \
    $$PWD/db_export/records/dbtgcabinet.cpp

	
