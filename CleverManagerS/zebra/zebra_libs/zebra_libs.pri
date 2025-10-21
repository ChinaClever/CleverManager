INCLUDEPATH += $$PWD

LIBS +=  -L$$PWD
LIBS +=  -ltorrent-rasterbar
LIBS +=  -lcryptopp-msr
LIBS +=  -lmaidsafe_common-msr
LIBS +=  -lmaidsafe_passport-msr
LIBS +=  -lpeersafe_channel-msr
LIBS +=  -lpeersafe_core-msr
LIBS +=  -lpeersafe_dht-msr
LIBS +=  -lpeersafe_imapi-msr
LIBS +=  -lpeersafe_peer-msr
LIBS +=  -lpeersafe_visit-msr
LIBS +=  -lproto_compiler-msr
LIBS +=  -lprotobuf_lite-msr
LIBS +=  -lprotobuf-msr
LIBS +=  -lsqlite-msr
LIBS +=  -llibboost_chrono-vc140-mt-1_64
LIBS +=  -llibboost_date_time-vc140-mt-1_64
LIBS +=  -llibboost_filesystem-vc140-mt-1_64
LIBS +=  -llibboost_program_options-vc140-mt-1_64
LIBS +=  -llibboost_regex-vc140-mt-1_64
LIBS +=  -llibboost_system-vc140-mt-1_64
LIBS +=  -llibboost_thread-vc140-mt-1_64

SOURCES += \
   

HEADERS  += \
    $$PWD/im.h \
    $$PWD/event.h
    
