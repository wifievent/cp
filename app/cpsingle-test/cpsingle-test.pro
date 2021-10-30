QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

include(../../../g/g.pri)

SOURCES += \
        main.cpp \
        myobj.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    myobj.h
