QT -= core gui
TEMPLATE = app
CONFIG -= app_bundle
SOURCES += cli.cpp options.cpp
INCLUDEPATH += ../CorsairLinkLib
macx:INCLUDEPATH += /usr/local/include
LIBS += -L.. -lCorsairLink
macx:LIBS += -L/usr/local/lib -lhidapi
!macx:LIBS += -lhidapi-hidraw

DESTDIR = ..
TARGET = OpenCorsairLinkCli

