QT += core

TEMPLATE = app
CONFIG += c++17 console
CONFIG += qt

LIBS *= -pthread

INCLUDEPATH *= $${PWD}/../../src

DESTDIR = $${PWD}/../../bin

SOURCES += \
        main.cpp
