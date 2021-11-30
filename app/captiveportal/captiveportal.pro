QT += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TEMPLATE = app
CONFIG += qt

include(../../../opennet/opennet.pri)
include(../../cp.pri)

DESTDIR = $${PWD}/../../bin

LIBS += -LSS{PWD}/../../../opennet/bin -lopennet

INCLUDEPATH += $${PWD}/../../../opennet/src
INCLUDEPATH += $${PWD}/../../../opennet/include
PRE_TARGETDEPS += $${PWD}/../../../opennet/bin/libOpenNet.a

RESOURCES += image.qrc

SOURCES += \
    ../../src/socket.cpp \
    ../../src/udpclient.cpp \
    ../../src/udpsocket.cpp \
    ../../src/udpserver.cpp \
    ../../src/httprequest.cpp \
    ../../src/httpresponse.cpp \
    ../../src/tcpsocket.cpp \
    ../../src/tcpclient.cpp \
    mainwindow.cpp \
    captiveportal.cpp \
    main.cpp \
    weudpclient.cpp \
    weudpserver.cpp \
    weupdater.cpp

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
    mainwindow.h \
    captiveportal.h \
    stdafx.h \
    weudpclient.h \
    weudpserver.h \
    weupdater.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
