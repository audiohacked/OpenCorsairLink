QT -= core gui
VERSION = 0.9
unix {
	TEMPLATE = lib
}
win32 {
	TEMPLATE = vclib
}
macx {
	TEMPLATE = lib
	CONFIG += lib_bundle
}

CONFIG += staticlib debug
!macx:LIBS += -lhidapi-hidraw
macx:LIBS += -lhidapi
macx:INCLUDEPATH += /usr/local/include
SOURCES += \
	Fan.cpp \
	Led.cpp \
	Link.cpp \
	Temperature.cpp

HEADERS += \
	Fan.h \
	Led.h \
	Link.h \
	Proto.h \
	Temperature.h

DESTDIR = ..
TARGET = CorsairLink

