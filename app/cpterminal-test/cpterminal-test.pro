
TEMPLATE = app
CONFIG += c++17
CONFIG += qt

include(../../../opennet/opennet.pri)
include(../../cp.pri)
DESTDIR = $${PWD}/../../bin

SOURCES += \
        core.cpp \
        main.cpp

HEADERS += \
    core.h

