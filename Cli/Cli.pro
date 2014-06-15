QT -= core gui
TEMPLATE = app
CONFIG -= app_bundle
SOURCES += cli.cpp
INCLUDEPATH += . ../CorsairLinkLib
LIBS += -L.. -lCorsairLink -lhidapi-hidraw

DESTDIR = ..
TARGET = OpenCorsairLinkCli

