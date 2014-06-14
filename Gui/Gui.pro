CONFIG += 
TEMPLATE +=
QT +=
win32 {
	QT += winextras
	SOURCES += microsoft.cpp
	HEADERS += microsoft.h
}
macx {
	QT += macextras
	SOURCES += apple.cpp
	HEADERS += apple.h
}

SOURCES += main.cpp \
	window.cpp 

HEADERS += window.h

DESTDIR = ..
TARGET = OpenCorsairLinkGui

