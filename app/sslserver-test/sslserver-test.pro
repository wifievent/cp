CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH = $${PWD}/../../../opensocket/src
INCLUDEPATH += $${PWD}/../../../opensocket/external

LIBS += -L../../opensocket/lib -lopensocket -lssl -lcrypto -lpthread

SOURCES += \
        main.cpp
