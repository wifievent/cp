QT += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TEMPLATE = app
CONFIG += qt

include(../../../g/g.pri)
include(../../cp.pri)

DESTDIR = $${PWD}/../../bin

#RESOURCES += image.qrc

SOURCES += \
    ../../src/socket.cpp \
    ../../src/udpclient.cpp \
    ../../src/udpsocket.cpp \
    cpudpclient.cpp \
    cpudpserver.cpp \
    mainwindow.cpp \
    captiveportal.cpp \
    main.cpp

HEADERS += \
    ../../src/socket.h \
    ../../src/udpclient.h \
    ../../src/udpsocket.h \
    ../../src/udp.h \
    cpudpclient.h \
    cpudpserver.h \
    mainwindow.h \
    captiveportal.h \
    stdafx.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
