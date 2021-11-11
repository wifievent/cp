QT += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TEMPLATE = app
CONFIG += qt

include(../../../g/g.pri)

DESTDIR = $${PWD}/../../bin

SOURCES += \
    mainwindow.cpp \
    captiveportal.cpp \
    main.cpp

HEADERS += \
    mainwindow.h \
    captiveportal.h \
    stdafx.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
