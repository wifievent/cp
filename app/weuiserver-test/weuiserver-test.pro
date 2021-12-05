CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH = $${PWD}/../../../opensocket/src
INCLUDEPATH += $${PWD}/../../../opensocket/external

include(../../cp.pri)

LIBS += -L../../../opensocket/lib -lopensocket -lssl -lcrypto -lpthread

SOURCES += \
        ../../src/socket.cpp \
        ../../src/udpclient.cpp \
        ../../src/udpsocket.cpp \
        ../../src/udpserver.cpp \
        ../../src/httprequest.cpp \
        ../../src/httpresponse.cpp \
        ../../src/tcpsocket.cpp \
        ../../src/tcpclient.cpp \
        main.cpp \
        weuiserver.cpp

HEADERS += \
        ../../src/socket.h \
        ../../src/udpclient.h \
        ../../src/udpsocket.h \
        ../../src/udpserver.h \
        ../../src/httprequest.h \
        ../../src/httpresponse.h \
        ../../src/tcpsocket.h \
        ../../src/tcpclient.h \
        ../../src/http.h \
        weuiserver.h
