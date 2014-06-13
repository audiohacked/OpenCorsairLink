VERSION = 0.9
win32 {
	TEMPLATE = vclib
}
unix {
	TEMPLATE = lib
}
macx {
	TEMPLATE = lib
	CONFIG += lib_bundle
}

CONFIG += staticlib debug
LIBS += -lhidapi-hidraw

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

