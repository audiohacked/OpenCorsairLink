CONFIG += debug
CONFIG += qt
QT += widgets
TEMPLATE += app
#win32 {
#	QT += winextras
#	SOURCES += microsoft.cpp
#	HEADERS += microsoft.h
#}
#macx {
#	QT += macextras
#	SOURCES += apple.cpp
#	HEADERS += apple.h
#}

SOURCES += main.cpp \
	window.cpp \
	FanWidget.cpp \
	LedWidget.cpp \
	StatusWidget.cpp \
	TemperatureWidget.cpp

HEADERS += window.h \
	FanWidget.h \
	LedWidget.h \
	StatusWidget.h \
	TemperatureWidget.h

DESTDIR = ..
TARGET = OpenCorsairLinkGui

